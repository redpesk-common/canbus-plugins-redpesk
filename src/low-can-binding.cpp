/*
 * Copyright (C) 2015, 2016 "IoT.bzh"
 * Author "Romain Forlot" <romain.forlot@iot.bzh>
 * Author "Loic Collignon" <loic.collignon@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "low-can-binding.hpp"

#include <queue>
#include <mutex>
#include <vector>
#include <thread>
#include <fcntl.h>
#include <linux/can.h>
#include <json-c/json.h>
#include <systemd/sd-event.h>

#include "timer.hpp"
#include "openxc.pb.h"
#include "can-utils.hpp"
#include "can-signals.hpp"
#include "openxc-utils.hpp"

extern "C"
{
	#include <afb/afb-service-itf.h>
};

/*
 *	Interface between the daemon and the binding
 */
const struct afb_binding_interface *binder_interface;

/********************************************************************************
*
*		Subscription and unsubscription
*
*********************************************************************************/

static int make_subscription_unsubscription(struct afb_req request, std::map<std::string, struct afb_event>::iterator& ss_i, bool subscribe)
{
	/* Make the subscription or unsubscription to the event */
	if (((subscribe ? afb_req_subscribe : afb_req_unsubscribe)(request, ss_i->second)) < 0)
	{
		ERROR(binder_interface, "Operation goes wrong for signal: %s", ss_i->first);
		return 0;
	}
	return 1;

}

static int create_event_handle(std::map<std::string, struct afb_event>::iterator& ss_i)
{
	ss_i->second = afb_daemon_make_event(binder_interface->daemon, ss_i->first.c_str());
	if (!afb_event_is_valid(ss_i->second)) 
	{
		ERROR(binder_interface, "Can't create an event, something goes wrong.");
		return 0;
	}

	return 1;
}

static int subscribe_unsubscribe_signal(struct afb_req request, bool subscribe, const CanSignal& sig)
{
	int ret;

	std::lock_guard<std::mutex> subscribed_signals_lock(get_subscribed_signals_mutex());
	auto ss_i = subscribed_signals.find(sig.genericName);
	if (ss_i != subscribed_signals.end() && !afb_event_is_valid(ss_i->second))
	{
		if(!subscribe)
		{
			NOTICE(binder_interface, "Event isn't valid, it can't be unsubscribed.");
			ret = -1;
		}
		else
		{
			/* Event it isn't valid annymore, recreate it */
			ret = create_event_handle(ss_i);
		}
	}
	else
	{
		/* Event don't exist , so let's create it */
		struct afb_event empty_event = {nullptr, nullptr};
		auto ss_i = subscribed_signals.insert(std::make_pair(sig.genericName, empty_event));
		ret = create_event_handle(ss_i.first);
	}

	/* Check whether or not the event handler has been correctly created and
	 * make the subscription/unsubscription operation is so.
	 */
	if (ret <= 0)
		return ret;
	return make_subscription_unsubscription(request, ss_i, subscribe);
}

static int subscribe_unsubscribe_signals(struct afb_req request, bool subscribe, const std::vector<CanSignal>& signals)
{
	int ret = 0;

	for(const auto& signal_i : signals)
	{
		ret = subscribe_unsubscribe_signal(request, subscribe, signal_i);
		if(ret == 0)
			return ret;
	}
	return ret;
}

static int subscribe_unsubscribe_all(struct afb_req request, bool subscribe)
{
	int e = 0;

	//for (const auto& sig : SIGNALS)
	//	e += !subscribe_unsubscribe_signals(request, subscribe, sig);
	e += !subscribe_unsubscribe_signals(request, subscribe, getSignals());
	
	return e == 0;
}

static int subscribe_unsubscribe_name(struct afb_req request, bool subscribe, const char *name)
{
	std::vector<CanSignal> sig;
	int ret = 0;

	if (!::strcmp(name, "*"))
		ret = subscribe_unsubscribe_all(request, subscribe);
	else
	{
		//if(obd2_handler_c.is_obd2_signal(name))
		if(false)
		{
		// TODO
		}
		else
		{
			openxc_DynamicField search_key = build_DynamicField(name);
			sig = find_can_signals(search_key);
			if (sig.empty())
				ret = 0;
		}
		ret = subscribe_unsubscribe_signals(request, subscribe, sig);
	}
	return ret;
}

static void subscribe_unsubscribe(struct afb_req request, bool subscribe)
{
	int ok, i, n;
	struct json_object *args, *a, *x;

	/* makes the subscription/unsubscription */
	args = afb_req_json(request);
	if (args == NULL || !json_object_object_get_ex(args, "event", &a)) {
		ok = subscribe_unsubscribe_all(request, subscribe);
	} else if (json_object_get_type(a) != json_type_array) {
		ok = subscribe_unsubscribe_name(request, subscribe, json_object_get_string(a));
	} else {
		n = json_object_array_length(a);
		ok = 0;
		for (i = 0 ; i < n ; i++) {
			x = json_object_array_get_idx(a, i);
			if (subscribe_unsubscribe_name(request, subscribe, json_object_get_string(x)))
				ok++;
		}
		ok = (ok == n);
	}

	/* send the report */
	if (ok)
		afb_req_success(request, NULL, NULL);
	else
		afb_req_fail(request, "error", NULL);
}

extern "C"
{
	static void subscribe(struct afb_req request)
	{
		subscribe_unsubscribe(request, true);
	}

	static void unsubscribe(struct afb_req request)
	{
		subscribe_unsubscribe(request, false);
	}

	static const struct afb_verb_desc_v1 verbs[]=
	{
		{ .name= "subscribe",	.session= AFB_SESSION_NONE, .callback= subscribe,	.info= "subscribe to notification of CAN bus messages." },
		{ .name= "unsubscribe",	.session= AFB_SESSION_NONE, .callback= unsubscribe,	.info= "unsubscribe a previous subscription." }
	};

	static const struct afb_binding binding_desc {
		AFB_BINDING_VERSION_1,
		{
			"CAN bus service",
			"can",
			verbs
		}
	};

	const struct afb_binding *afbBindingV1Register (const struct afb_binding_interface *itf)
	{
		binder_interface = itf;

		return &binding_desc;
	}

	/**
	* @brief Initialize the binding.
	* 
	* @param[in] service Structure which represent the Application Framework Binder.
	* 
	* @return Exit code, zero if success.
	*/
	int afbBindingV1ServiceInit(struct afb_service service)
	{
		int fd_conf;
		fd_conf = afb_daemon_rootdir_open_locale(binder_interface->daemon, "can_bus.json", O_RDONLY, NULL);

		/* Initialize the CAN bus handler */
		can_bus_t can_bus_handler(fd_conf);

		/* Open CAN socket */
		if(can_bus_handler.init_can_dev() == 0)
		{
			can_bus_handler.start_threads();
			return 0;
		}
		ERROR(binder_interface, "There was something wrong with CAN device Initialization. Check your config file maybe");
		return 1;
	}
};

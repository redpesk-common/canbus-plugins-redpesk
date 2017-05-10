/*
 * Copyright (C) 2015, 2016 "IoT.bzh"
 * Author "Romain Forlot" <romain.forlot@iot.bzh>
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

#pragma once

#include <vector>
#include <string>
#include <fnmatch.h>

#include "openxc.pb.h"
#include "../binding/configuration.hpp"
#include "../can/can-signals.hpp"
#include "../diagnostic/diagnostic-message.hpp"

#include "../binding/low-can-hat.hpp"

namespace utils
{
	struct signals_found
	{
		std::vector<std::shared_ptr<can_signal_t> > can_signals;
		std::vector<std::shared_ptr<diagnostic_message_t> > diagnostic_messages;
	};

	class signals_manager_t
	{
	private:
		std::mutex subscribed_signals_mutex_;
		std::map<std::string, struct afb_event> subscribed_signals_;

		signals_manager_t(); ///< Private constructor to make singleton class.

	public:
		static signals_manager_t& instance();

		std::mutex& get_subscribed_signals_mutex();
		std::map<std::string, struct afb_event>& get_subscribed_signals();

		struct signals_found find_signals(const openxc_DynamicField &key);
		void find_diagnostic_messages(const openxc_DynamicField &key, std::vector<std::shared_ptr<diagnostic_message_t> >& found_signals);
		void find_can_signals(const openxc_DynamicField &key, std::vector<std::shared_ptr<can_signal_t> >& found_signals);

		template <typename T>
		void lookup_signals_by_name(const std::string& key, std::vector<std::shared_ptr<T> > signals, std::vector<std::shared_ptr<T> > found_signals)
		{
			for(std::shared_ptr<T> s : signals)
			{
				if(::fnmatch(key.c_str(), s->get_generic_name().c_str(), FNM_CASEFOLD) == 0)
					found_signals.push_back(s);
				else if(::fnmatch(key.c_str(), s->get_name().c_str(), FNM_CASEFOLD) == 0)
					found_signals.push_back(s);
			}
		}

		template <typename T>
		void lookup_signals_by_id(const double key, std::vector<std::shared_ptr<T> > signals, std::vector<std::shared_ptr<T> > found_signals)
		{
			for(std::shared_ptr<T> s : signals)
			{
				if(configuration_t::instance().get_signal_id(*s) == key)
				{
					found_signals.push_back(s);
				}
			}
		}
	};
}
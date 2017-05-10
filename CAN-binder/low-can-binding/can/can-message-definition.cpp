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

#include "can-message-definition.hpp"

can_message_definition_t::can_message_definition_t(const std::string bus)
	: parent_{nullptr}, bus_{bus}, last_value_{CAN_MESSAGE_SIZE}
{}

can_message_definition_t::can_message_definition_t(
	const std::string bus,
	uint32_t id,
	frequency_clock_t frequency_clock,
	bool force_send_changed)
	: parent_{nullptr},
	  bus_{bus},
	  id_{id},
	  frequency_clock_{frequency_clock},
	  force_send_changed_{force_send_changed},
	  last_value_{CAN_MESSAGE_SIZE}
{}

can_message_definition_t::can_message_definition_t(
	const std::string bus,
	uint32_t id,
	can_message_format_t format,
	frequency_clock_t frequency_clock,
	bool force_send_changed)
	: parent_{nullptr},
	bus_{bus},
	id_{id},
	format_{format},
	frequency_clock_{frequency_clock},
	force_send_changed_{force_send_changed},
	last_value_{CAN_MESSAGE_SIZE}
{}

can_message_definition_t::can_message_definition_t(
	const std::string bus,
	uint32_t id,
	can_message_format_t format,
	frequency_clock_t frequency_clock,
	bool force_send_changed,
	std::vector<std::shared_ptr<can_signal_t> > can_signals)
	:  parent_{nullptr},
	bus_{bus},
	id_{id},
	format_{format},
	frequency_clock_{frequency_clock},
	force_send_changed_{force_send_changed},
	last_value_{CAN_MESSAGE_SIZE},
	can_signals_{std::move(can_signals)}
{
	for(auto& sig: can_signals_)
	{
		sig->set_parent(std::make_shared<can_message_definition_t>(*this));
	}
}

/*can_message_definition_t(const can_message_definition_t& b)
	:  parent_{b.parent_},
	bus_{b.bus_},
	id_{b.id_},
	format_{b.format_},
	frequency_clock_{b.frequency_clock_},
	force_send_changed_{b.force_send_changed_},
	last_value_{b.last_value_},
	can_signals_{b.can_signals_}
	{}*/

const std::string& can_message_definition_t::get_bus_name() const
{
	return bus_;
}

uint32_t can_message_definition_t::get_id() const
{
	return id_;
}

std::vector<std::shared_ptr<can_signal_t> > can_message_definition_t::get_can_signals()
{
	return can_signals_;
}

void can_message_definition_t::set_parent(std::shared_ptr<can_message_set_t> parent)
{
	parent_= parent;
}

void can_message_definition_t::set_last_value(const can_message_t& cm)
{
	last_value_= cm.get_data_vector();
}
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

#include "can/can-signals.hpp"

#include <fnmatch.h>

#include "utils/signals.hpp"
#include "can/can-decoder.hpp"
#include "obd2/obd2-signals.hpp"
#include "low-can-binding.hpp"

can_message_definition_t& can_signal_t::get_message()
{
	return message_;
}

std::string& can_signal_t::get_generic_name()
{
	return generic_name_;
}

uint8_t can_signal_t::get_bit_position() const
{
	return bit_position_;
}

uint8_t can_signal_t::get_bit_size() const
{
	return bit_size_;
}

/**
 * @fn void find_can_signals(const openxc_DynamicField& key, std::vector<can_signal_t*>& found_signals)
 * @brief return signals name found searching through CAN_signals and OBD2 pid
 * 
 * @param[in] key - can contain numeric or string value in order to search against 
 *   can signals or obd2 signals name.
 * @param[out] found_signals - provided vector to fill with ponter to signals matched.
 *
 */
void find_can_signals(const openxc_DynamicField& key, std::vector<can_signal_t*>& found_signals)
{
	switch(key.type)
	{
		case openxc_DynamicField_Type::openxc_DynamicField_Type_STRING:
			lookup_signals_by_name(key.string_value, get_can_signals(), found_signals);
			break;
		case openxc_DynamicField_Type::openxc_DynamicField_Type_NUM:
			lookup_signals_by_id(key.numeric_value, get_can_signals(), found_signals);
			break;
		default:
			ERROR(binder_interface, "find_signals: wrong openxc_DynamicField specified. Use openxc_DynamicField_Type_NUM or openxc_DynamicField_Type_STRING type only.");
			break;
	}
	DEBUG(binder_interface, "Found %d signal(s)", (int)found_signals.size());
float can_signal_t::get_factor() const
{
	return factor_;
}

float can_signal_t::get_offset() const
{
	return offset_;
}

float can_signal_t::get_min_value() const
{
	return min_value_;
}	

float can_signal_t::get_max_value() const
{
	return max_value_;
}

FrequencyClock& can_signal_t::get_frequency()
{
	return frequency_;
}

bool can_signal_t::get_send_same() const
{
	return send_same_;
}

bool can_signal_t::get_force_send_changed() const
{
	return force_send_changed_;
}

std::map<int, std::string> can_signal_t::get_state() const
{
	return states_;
}

size_t can_signal_t::get_state_count() const
{
	return states_.size();
}

bool can_signal_t::get_writable() const
{
	return writable_;
}

SignalDecoder& can_signal_t::get_decoder()
{
	return decoder_;
}

SignalEncoder& can_signal_t::get_encoder()
{
	return encoder_;
}

bool can_signal_t::get_received() const
{
	return received_;
}
float can_signal_t::get_last_value() const
{
	return last_value_;
}

void can_signal_t::set_received(bool r)
{
	received_ = r;
}

void can_signal_t::set_last_value(float val)
{
	last_value_ = val;
}

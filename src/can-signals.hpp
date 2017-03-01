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

#include <map>
#include <mutex>
#include <queue>
#include <vector>
#include <string>

#include "timer.hpp"
#include "openxc.pb.h"
#include "can-bus.hpp"
#include "can-message.hpp"

extern "C"
{
	#include <afb/afb-binding.h>
	#include <afb/afb-event-itf.h>
}

#define MESSAGE_SET_ID 0

/** 
 * @brief Can signal event map making access to afb_event
 * externaly to an openxc existing structure.
 *
 * @desc Event map is making relation between CanSignal generic name
 * and the afb_event struct used by application framework to pushed
 * to the subscriber.
 */
static std::map<std::string, struct afb_event> subscribed_signals;

std::mutex& get_subscribed_signals_mutex();

/**
 * @brief The type signature for a CAN signal decoder.
 *
 * @desc A SignalDecoder transforms a raw floating point CAN signal into a number,
 * string or boolean.
 *
 * @param[in] CanSignal signal - The CAN signal that we are decoding.
 * @param[in] CanSignal signals - The list of all signals.
 * @param[in] int signalCount - The length of the signals array.
 * @param[in] float value - The CAN signal parsed from the message as a raw floating point
 *	value.
 * @param[out] bool send - An output parameter. If the decoding failed or the CAN signal should
 *	not send for some other reason, this should be flipped to false.
 *
 * @return a decoded value in an openxc_DynamicField struct.
 */
typedef openxc_DynamicField (*SignalDecoder)(struct CanSignal& signal,
		const std::vector<CanSignal>& signals, float value, bool* send);

/**
 * @brief: The type signature for a CAN signal encoder.
 *
 * @desc A SignalEncoder transforms a number, string or boolean into a raw floating
 * point value that fits in the CAN signal.
 *
 * @params[signal] - The CAN signal to encode. 
 * @params[value] - The dynamic field to encode.
 * @params[send] - An output parameter. If the encoding failed or the CAN signal should
 * not be encoded for some other reason, this will be flipped to false.
 */
typedef uint64_t (*SignalEncoder)(struct CanSignal* signal,
		openxc_DynamicField* value, bool* send);

/**
 * @struct CanSignalState
 *
 * @brief A state encoded (SED) signal's mapping from numerical values to
 * OpenXC state names.
 */
struct CanSignalState {
	const int value; /*!< int value - The integer value of the state on the CAN bus.*/
	const char* name; /*!< char* name  - The corresponding string name for the state in OpenXC. */
};
typedef struct CanSignalState CanSignalState;

/**
 * @struct CanSignal
 *
 * @brief A CAN signal to decode from the bus and output over USB.
 */
struct CanSignal {
	struct CanMessageDefinition* message; /*!< message	   - The message this signal is a part of. */
	const char* genericName; /*!< genericName - The name of the signal to be output over USB.*/
	uint8_t bitPosition; /*!< bitPosition - The starting bit of the signal in its CAN message (assuming
 						*	non-inverted bit numbering, i.e. the most significant bit of
 						*	each byte is 0) */
	uint8_t bitSize; /*!< bitSize - The width of the bit field in the CAN message. */
	float factor; /*!< factor - The final value will be multiplied by this factor. Use 1 if you
 				*	don't need a factor. */
	float offset; /*!< offset	   - The final value will be added to this offset. Use 0 if you
 				*	don't need an offset. */
	float minValue; /*!< minValue    - The minimum value for the processed signal.*/
	float maxValue; /*!< maxValue    - The maximum value for the processed signal. */
	FrequencyClock frequencyClock; /*!< frequencyClock - A FrequencyClock struct to control the maximum frequency to
 								*	process and send this signal. To process every value, set the
 								*	clock's frequency to 0. */
	bool sendSame; /*!< sendSame    - If true, will re-send even if the value hasn't changed.*/
	bool forceSendChanged; /*!< forceSendChanged - If true, regardless of the frequency, it will send the
 						*	value if it has changed. */
	const CanSignalState* states; /*!< states	   - An array of CanSignalState describing the mapping
 								*	between numerical and string values for valid states. */
	uint8_t stateCount; /*!< stateCount  - The length of the states array. */
	bool writable; /*!< writable    - True if the signal is allowed to be written from the USB host
 				*	back to CAN. Defaults to false.*/
	SignalDecoder decoder; /*!< decoder	   - An optional function to decode a signal from the bus to a human
 						*	readable value. If NULL, the default numerical decoder is used. */
	SignalEncoder encoder; /*!< encoder	   - An optional function to encode a signal value to be written to
 						*	CAN into a byte array. If NULL, the default numerical encoder
 						*	is used. */
	bool received; /*!< received    - True if this signal has ever been received.*/
	float lastValue; /*!< lastValue   - The last received value of the signal. If 'received' is false,
 					*	this value is undefined. */
};
typedef struct CanSignal CanSignal;

/* Public: Return signals from an signals array filtered on name.
 */
const std::vector<CanSignal> getSignals();

/* Public: Return the length of the array returned by getSignals(). */
size_t getSignalCount();

/**
 * @brief Find one or many signals based on its name or id
 * passed through openxc_DynamicField.
 *
 * @param[in] openxc_DynamicField& - a const reference with the key to search into signal.
 * Key is either a signal name or its CAN arbitration id.
 *
 * @return std::vector<std::string> return found CanSignal generic name vector.
 */
std::vector<CanSignal> find_can_signals(const openxc_DynamicField &key);

/**
 * @brief Retrieve can arbitration id of a given CanSignal
 *
 * @param[in] CanSignal& - a const reference to a CanSignal
 *
 * @return uint32_t - unsigned integer representing the arbitration id.
 */
inline uint32_t get_CanSignal_id(const CanSignal& sig);

/**
 * @brief return the subscribed_signals map.
 * 
 * return std::map<std::string, struct afb_event> - map of subscribed signals.
 */
const std::map<std::string, struct afb_event> get_subscribed_signals();
/* DO NOT MODIFY:  This source is generated by the scripts in the
 * vi-firmware repository.
 *
 * Generated for v7.x of the OpenXC VI firmware.
 */

#include "diagnostics.h"
#include "can/canread.h"
#include "can/canwrite.h"
#include "signals.h"
#include "obd2.h"
#include "util/log.h"
#include "config.h"
#include "shared_handlers.h"

namespace can = openxc::can;

using openxc::util::log::debug;
using openxc::pipeline::Pipeline;
using openxc::config::getConfiguration;
using openxc::can::read::booleanDecoder;
using openxc::can::read::stateDecoder;
using openxc::can::read::ignoreDecoder;
using openxc::diagnostics::obd2::handleObd2Pid;
using namespace openxc::signals::handlers;

#include "can/canread.h"

using openxc::can::read::publishNumericalMessage;

void handleSteeringWheelMessage(CanMessage* message,
        CanSignal* signals, int signalCount, Pipeline* pipeline) {
    publishNumericalMessage("latitude", 42.0, pipeline);
}

openxc_DynamicField handleInverted(CanSignal* signal, CanSignal* signals,
        int signalCount, float value, bool* send) {
    return openxc::payload::wrapNumber(value * -1);
}

void initializeMyStuff() { }

void initializeOtherStuff() { }

void myLooper() {
    // this function will be called once each time through the main loop, after
    // all CAN message processing has been completed
}

const int MESSAGE_SET_COUNT = 1;
CanMessageSet MESSAGE_SETS[MESSAGE_SET_COUNT] = {
    { 0, "example", 2, 1, 5, 1 },
};

const int MAX_CAN_BUS_COUNT = 2;
CanBus CAN_BUSES[][MAX_CAN_BUS_COUNT] = {
    { // message set: example
        { speed: 500000,
        address: 1,
        maxMessageFrequency: 0,
        rawWritable: false,
        passthroughCanMessages: false,
        bypassFilters: false,
        loopback: false
        },

        { speed: 125000,
        address: 2,
        maxMessageFrequency: 0,
        rawWritable: false,
        passthroughCanMessages: false,
        bypassFilters: false,
        loopback: false
        },

    },
};

const int MAX_MESSAGE_COUNT = 1;
CanMessageDefinition CAN_MESSAGES[][MAX_MESSAGE_COUNT] = {
    { // message set: example
        { bus: &CAN_BUSES[0][0], id: 0x128, format: STANDARD, frequencyClock: {0.000000}, forceSendChanged: true}, // ECM_z_5D2
    },
};

const int MAX_SIGNAL_STATES = 12;
const int MAX_SIGNALS_WITH_STATES_COUNT = 1;
const CanSignalState SIGNAL_STATES[][MAX_SIGNALS_WITH_STATES_COUNT][MAX_SIGNAL_STATES] = {
    { // message set: example
        { {value: 1, name: "FIRST"}, {value: 2, name: "SECOND"}, {value: 3, name: "THIRD"}, {value: 4, name: "FOURTH"}, {value: 5, name: "REVERSE"}, {value: 6, name: "NEUTRAL"}, { 0, NULL }, { 0, NULL }, { 0, NULL }, { 0, NULL }, { 0, NULL }, { 0, NULL }, },
    },
};

const int MAX_SIGNAL_COUNT = 5;
CanSignal SIGNALS[][MAX_SIGNAL_COUNT] = {
    { // message set: example
        {message: &CAN_MESSAGES[0][0], genericName: "GearshiftPosition", bitPosition: 41, bitSize: 3, factor: 1.000000, offset: 0.000000, minValue: 0.000000, maxValue: 0.000000, frequencyClock: {0.000000}, sendSame: true, forceSendChanged: false, states: SIGNAL_STATES[0][0], stateCount: 6, writable: false, decoder: stateDecoder, encoder: NULL}, // GrshftPos
        {message: &CAN_MESSAGES[0][0], genericName: "SteeringWheelAngle", bitPosition: 52, bitSize: 12, factor: 0.153920, offset: 0.000000, minValue: 0.000000, maxValue: 0.000000, frequencyClock: {0.000000}, sendSame: true, forceSendChanged: false, states: NULL, stateCount: 0, writable: false, decoder: handleUnsignedSteeringWheelAngle, encoder: NULL}, // StrAnglAct
        {message: &CAN_MESSAGES[0][0], genericName: "engine_speed", bitPosition: 12, bitSize: 8, factor: 1.000000, offset: 0.000000, minValue: 0.000000, maxValue: 0.000000, frequencyClock: {15.000000}, sendSame: true, forceSendChanged: false, states: NULL, stateCount: 0, writable: false, decoder: NULL, encoder: NULL}, // EngSpd
        {message: &CAN_MESSAGES[0][0], genericName: "steering_angle_sign", bitPosition: 52, bitSize: 12, factor: 1.000000, offset: 0.000000, minValue: 0.000000, maxValue: 0.000000, frequencyClock: {0.000000}, sendSame: true, forceSendChanged: false, states: NULL, stateCount: 0, writable: false, decoder: ignoreDecoder, encoder: NULL}, // StrAnglSign
        {message: &CAN_MESSAGES[0][0], genericName: "steering_wheel_angle_error", bitPosition: 44, bitSize: 12, factor: 1.000000, offset: 0.000000, minValue: 0.000000, maxValue: 0.000000, frequencyClock: {0.000000}, sendSame: true, forceSendChanged: false, states: NULL, stateCount: 0, writable: false, decoder: ignoreDecoder, encoder: NULL}, // StrAnglErr
    },
};

void openxc::signals::initialize(openxc::diagnostics::DiagnosticsManager* diagnosticsManager) {
    switch(getConfiguration()->messageSetIndex) {
    case 0: // message set: example
        initializeMyStuff();
        break;
    }
}

void openxc::signals::loop() {
    switch(getConfiguration()->messageSetIndex) {
    case 0: // message set: example
        myLooper();
        break;
    }
}

const int MAX_COMMAND_COUNT = 1;
CanCommand COMMANDS[][MAX_COMMAND_COUNT] = {
    { // message set: example
        { genericName: "turn_signal_status", handler: handleTurnSignalCommand },
    },
};

void openxc::signals::decodeCanMessage(Pipeline* pipeline, CanBus* bus, CanMessage* message) {
    switch(getConfiguration()->messageSetIndex) {
    case 0: // message set: example
        switch(bus->address) {
        case 1:
            switch (message->id) {
            case 0x128: // ECM_z_5D2
                handleSteeringWheelMessage(message, SIGNALS[0], getSignalCount(), pipeline);
                can::read::translateSignal(&SIGNALS[0][0], message, SIGNALS[0], getSignalCount(), pipeline); // GrshftPos
                can::read::translateSignal(&SIGNALS[0][1], message, SIGNALS[0], getSignalCount(), pipeline); // StrAnglAct
                can::read::translateSignal(&SIGNALS[0][2], message, SIGNALS[0], getSignalCount(), pipeline); // EngSpd
                can::read::translateSignal(&SIGNALS[0][3], message, SIGNALS[0], getSignalCount(), pipeline); // StrAnglSign
                can::read::translateSignal(&SIGNALS[0][4], message, SIGNALS[0], getSignalCount(), pipeline); // StrAnglErr
                break;
            }
            break;
        case 2:
            switch (message->id) {
            }
            break;
        }
        break;
    }
}


CanCommand* openxc::signals::getCommands() {
    return COMMANDS[getActiveMessageSet()->index];
}

int openxc::signals::getCommandCount() {
    return getActiveMessageSet()->commandCount;
}

CanMessageDefinition* openxc::signals::getMessages() {
    return CAN_MESSAGES[getActiveMessageSet()->index];
}

int openxc::signals::getMessageCount() {
    return getActiveMessageSet()->messageCount;
}

CanSignal* openxc::signals::get_can_signals() {
    return SIGNALS[getActiveMessageSet()->index];
}

int openxc::signals::getSignalCount() {
    return getActiveMessageSet()->signalCount;
}

CanBus* openxc::signals::getCanBuses() {
    return CAN_BUSES[getActiveMessageSet()->index];
}

int openxc::signals::getCanBusCount() {
    return getActiveMessageSet()->busCount;
}

CanMessageSet* openxc::signals::getActiveMessageSet() {
    return &MESSAGE_SETS[getConfiguration()->messageSetIndex];
}

CanMessageSet* openxc::signals::getMessageSets() {
    return MESSAGE_SETS;
}

int openxc::signals::getMessageSetCount() {
    return MESSAGE_SET_COUNT;
}


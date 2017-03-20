#include "configuration.hpp"
#include "can/can-decoder.hpp"

configuration_t::configuration_t()
	: can_message_set_{{0, "example", 0, 1, 5, 0, 19}}
	, can_message_definition_
	{
		{
			can_message_definition_t(0, "can0", 0x620, can_message_format_t::STANDARD, frequency_clock_t(0.00000f), true)
		}
	}
	, can_signals_
	{
		{
			{
				0,
				0,
				"doors.coffer.open",
				88,
				1,
				0.00000f, 
				0, 
				0,
				0,
				frequency_clock_t(0.00000f),
				true,
				false,
				{
				},
				false,
				decoder_t::booleanDecoder,
				nullptr,
				false
			},
			{
				0,
				0,
				"doors.driver.open",
				78,
				1,
				0.00000f, 
				0, 
				0,
				0,
				frequency_clock_t(0.00000f),
				true,
				false,
				{
				},
				false,
				decoder_t::booleanDecoder,
				nullptr,
				false
			},
			{
				0,
				0,
				"doors.passenger.open",
				79,
				1,
				0.00000f, 
				0, 
				0,
				0,
				frequency_clock_t(0.00000f),
				true,
				false,
				{
				},
				false,
				decoder_t::booleanDecoder,
				nullptr,
				false
			},
			{
				0,
				0,
				"doors.rearleft.open",
				86,
				1,
				0.00000f, 
				0, 
				0,
				0,
				frequency_clock_t(0.00000f),
				true,
				false,
				{
				},
				false,
				decoder_t::booleanDecoder,
				nullptr,
				false
			},
			{
				0,
				0,
				"doors.rearright.open",
				85,
				4,
				0.00000f, 
				0, 
				0,
				0,
				frequency_clock_t(0.00000f),
				true,
				false,
				{
				},
				false,
				decoder_t::booleanDecoder,
				nullptr,
				false
			}
		}
	}
	, diagnostic_messages_
	{
		{
			{
				4,
				"engine.load",
				0,
				0,
				UNIT::INVALID,
				5.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				5,
				"engine.coolant.temperature",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				10,
				"fuel.pressure",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				11,
				"intake.manifold.pressure",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				12,
				"engine.speed",
				0,
				0,
				UNIT::INVALID,
				5.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				13,
				"vehicle.speed",
				0,
				0,
				UNIT::INVALID,
				5.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				15,
				"intake.air.temperature",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				16,
				"mass.airflow",
				0,
				0,
				UNIT::INVALID,
				5.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				17,
				"throttle.position",
				0,
				0,
				UNIT::INVALID,
				5.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				31,
				"running.time",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				45,
				"EGR.error",
				0,
				0,
				UNIT::INVALID,
				0.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				47,
				"fuel.level",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				51,
				"barometric.pressure",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				76,
				"commanded.throttle.position",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				82,
				"ethanol.fuel.percentage",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				90,
				"accelerator.pedal.position",
				0,
				0,
				UNIT::INVALID,
				5.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				91,
				"hybrid.battery-pack.remaining.life",
				0,
				0,
				UNIT::INVALID,
				5.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				92,
				"engine.oil.temperature",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			},
			{
				99,
				"engine.torque",
				0,
				0,
				UNIT::INVALID,
				1.00000f,
				decoder_t::decode_obd2_response,
				nullptr,
				true
			}
		}
	}
{
}

const std::string configuration_t::get_diagnostic_bus() const
{
	return "can0";
}



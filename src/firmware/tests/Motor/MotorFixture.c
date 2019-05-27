#include <xc.h>
#include <unity.h>
#include <stdint.h>

#include "Event.h"
#include "VoltageRegulator.h"
#include "Motor.h"

#include "MotorFixture.h"

static void onMotorEnabled(const struct Event *event);
static void onMotorDisabled(const struct Event *event);
static void onVoltageRegulatorDisabled(const struct Event *event);

static uint8_t callSequence;
static uint8_t voltageRegulatorDisableIsStubbedForEvent;

uint8_t voltageRegulatorIsEnabledValue;
uint8_t voltageRegulatorEnableCalls;
uint8_t voltageRegulatorEnableSequence;
uint8_t voltageRegulatorDisableCalls;
uint8_t voltageRegulatorDisableSequence;
uint8_t onVoltageRegulatorDisabledCalls;
uint8_t onVoltageRegulatorDisabledSequence;
uint8_t onMotorEnabledCalls;
uint8_t onMotorEnabledSequence;
uint8_t onMotorDisabledCalls;
uint8_t onMotorDisabledSequence;

void motorFixtureSetUp(void)
{
	eventInitialise();

	callSequence = 1;
	voltageRegulatorDisableIsStubbedForEvent = 0;
	voltageRegulatorIsEnabledValue = 0;
	voltageRegulatorEnableCalls = 0;
	voltageRegulatorEnableSequence = 0;
	voltageRegulatorDisableCalls = 0;
	voltageRegulatorDisableSequence = 0;
	onVoltageRegulatorDisabledCalls = 0;
	onVoltageRegulatorDisabledSequence = 0;
	onMotorEnabledCalls = 0;
	onMotorEnabledSequence = 0;
	onMotorDisabledCalls = 0;
 	onMotorDisabledSequence = 0;

	static const struct EventSubscription onMotorEnabledSubscription =
	{
		.type = MOTOR_ENABLED,
		.handler = &onMotorEnabled,
		.state = (void *) 0
	};

	eventSubscribe(&onMotorEnabledSubscription);

	static const struct EventSubscription onMotorDisabledSubscription =
	{
		.type = MOTOR_DISABLED,
		.handler = &onMotorDisabled,
		.state = (void *) 0
	};

	eventSubscribe(&onMotorDisabledSubscription);
}

static void onMotorEnabled(const struct Event *event)
{
	TEST_ASSERT_NOT_NULL_MESSAGE(event->args, "MTR EN");
	onMotorEnabledCalls++;
	onMotorEnabledSequence = callSequence++;
}

static void onMotorDisabled(const struct Event *event)
{
	TEST_ASSERT_NOT_NULL_MESSAGE(event->args, "MTR DIS");
	onMotorDisabledCalls++;
	onMotorDisabledSequence = callSequence++;
}

void motorFixtureTearDown(void)
{
}

void publishVoltageRegulatorEnabled(void)
{
	static const struct VoltageRegulatorEnabled emptyEventArgs = { };
	eventPublish(VOLTAGE_REGULATOR_ENABLED, &emptyEventArgs);
}

void dispatchAllEvents(void)
{
	while (eventDispatchNext())
		;;
}

void publishVoltageRegulatorDisabled(void)
{
	static const struct VoltageRegulatorDisabled emptyEventArgs = { };
	eventPublish(VOLTAGE_REGULATOR_DISABLED, &emptyEventArgs);
}

void stubVoltageRegulatorIsEnabled(uint8_t value)
{
	voltageRegulatorIsEnabledValue = value;
}

uint8_t voltageRegulatorIsEnabled(void)
{
	return voltageRegulatorIsEnabledValue;
}

void voltageRegulatorEnable(void)
{
	voltageRegulatorEnableCalls++;
	voltageRegulatorEnableSequence = callSequence++;
}

void voltageRegulatorDisable(void)
{
	voltageRegulatorDisableCalls++;
	voltageRegulatorDisableSequence = callSequence++;
	if (voltageRegulatorDisableIsStubbedForEvent)
		publishVoltageRegulatorDisabled();
}

void mockOnVoltageRegulatorDisabled(void)
{
	static const struct EventSubscription onVoltageRegulatorDisabledSubscription =
	{
		.type = VOLTAGE_REGULATOR_DISABLED,
		.handler = &onVoltageRegulatorDisabled,
		.state = (void *) 0
	};

	eventSubscribe(&onVoltageRegulatorDisabledSubscription);
}

static void onVoltageRegulatorDisabled(const struct Event *event)
{
	TEST_ASSERT_NOT_NULL_MESSAGE(event->args, "VR DIS");
	onVoltageRegulatorDisabledCalls++;
	onVoltageRegulatorDisabledSequence = callSequence++;
}

void stubVoltageRegulatorDisableToPublishEvent(void)
{
	voltageRegulatorDisableIsStubbedForEvent = 1;
}

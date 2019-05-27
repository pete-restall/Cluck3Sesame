#include <xc.h>
#include <stdint.h>
#include <unity.h>

#include "Event.h"
#include "Motor.h"

#include "MotorFixture.h"

#include "../Fixture.h"
#include "../NonDeterminism.h"

TEST_FILE("Motor/MotorInitialise.c")
TEST_FILE("Motor/MotorEnableDisable.c")

void onBeforeTest(void)
{
	motorFixtureSetUp();
	motorInitialise();
}

void onAfterTest(void)
{
	motorFixtureTearDown();
}

void test_motorEnable_calledWhenVoltageRegulatorIsDisabled_expectVoltageRegulatorIsEnabled(void)
{
	stubVoltageRegulatorIsEnabled(0);
	motorEnable();
	TEST_ASSERT_EQUAL_UINT8(1, voltageRegulatorEnableCalls);
}

void test_motorEnable_calledWhenVoltageRegulatorIsEnabled_expectVoltageRegulatorIsEnabledAgain(void)
{
	stubVoltageRegulatorIsEnabled(1);
	motorEnable();
	TEST_ASSERT_EQUAL_UINT8(1, voltageRegulatorEnableCalls);
}

void test_motorEnable_calledMoreThanOnce_expectVoltageRegulatorIsEnabledSameNumberOfTimes(void)
{
	uint8_t numberOfCalls = anyByteLessThan(10);
	for (uint8_t i = 0; i < numberOfCalls; i++)
		motorEnable();

	TEST_ASSERT_EQUAL_UINT8(numberOfCalls, voltageRegulatorEnableCalls);
}

void test_motorEnable_calledForFirstTimeWhenVoltageRegulatorIsAlreadyEnabled_expectMotorEnabledEventIsPublished(void)
{
	stubVoltageRegulatorIsEnabled(1);
	motorEnable();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(1, onMotorEnabledCalls);
}

void test_motorEnable_calledForFirstTimeWhenVoltageRegulatorIsNotEnabled_expectMotorEnabledEventIsNotPublished(void)
{
	stubVoltageRegulatorIsEnabled(0);
	motorEnable();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(0, onMotorEnabledCalls);
}

void test_motorEnable_calledForSecondTimeWhenVoltageRegulatorIsAlreadyEnabled_expectMotorEnabledEventIsNotPublished(void)
{
	stubVoltageRegulatorIsEnabled(1);
	motorEnable();
	dispatchAllEvents();
	motorEnable();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(1, onMotorEnabledCalls);
}

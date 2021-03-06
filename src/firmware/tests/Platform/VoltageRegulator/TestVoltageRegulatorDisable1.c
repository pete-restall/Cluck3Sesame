#include <xc.h>
#include <stdint.h>
#include <unity.h>

#include "Mock_Event.h"
#include "Platform/VoltageRegulator.h"

#include "VoltageRegulatorFixture.h"

#include "../../Fixture.h"
#include "../../NonDeterminism.h"

TEST_FILE("Platform/VoltageRegulator.c")
TEST_FILE("Platform/VoltageRegulatorFixture.c")

const struct Event eventEmptyArgs = { };

void test_voltageRegulatorDisable_calledAfterEnabledOnce_expectEnablePinIsLow(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();
	voltageRegulatorDisable();
	TEST_ASSERT_FALSE(LATBbits.LATB2);

	callScheduleHandlerAndForget();
	TEST_ASSERT_FALSE(LATBbits.LATB2);

	callScheduleHandlerIfPresentAndForget();
	TEST_ASSERT_FALSE(LATBbits.LATB2);
}

void test_voltageRegulatorDisable_calledAfterVoltageRailStabilised_expectEnablePinIsLow(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();
	callScheduleHandlerAndForget();
	voltageRegulatorDisable();
	callScheduleHandlerAndForget();
	TEST_ASSERT_FALSE(LATBbits.LATB2);
}

void test_voltageRegulatorDisable_calledAfterFullyEnabledOnce_expectEnablePinIsLow(void)
{
	voltageRegulatorInitialise();
	fullyEnableVoltageRegulator();
	voltageRegulatorDisable();
	TEST_ASSERT_FALSE(LATBbits.LATB2);
}

void test_voltageRegulatorDisable_calledAfterEnabledOnce_expectVmcuSelPinIsLow(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();
	voltageRegulatorDisable();
	TEST_ASSERT_FALSE(LATBbits.LATB0);

	callScheduleHandlerAndForget();
	TEST_ASSERT_FALSE(LATBbits.LATB0);

	callScheduleHandlerIfPresentAndForget();
	TEST_ASSERT_FALSE(LATBbits.LATB0);
}

void test_voltageRegulatorDisable_calledAfterVoltageRailStabilised_expectVmcuSelPinIsLow(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();
	callScheduleHandlerAndForget();
	voltageRegulatorDisable();
	callScheduleHandlerAndForget();
	TEST_ASSERT_FALSE(LATBbits.LATB0);
}

void test_voltageRegulatorDisable_calledAfterFullyEnabledOnce_expectVmcuSelPinIsLow(void)
{
	voltageRegulatorInitialise();
	fullyEnableVoltageRegulator();
	voltageRegulatorDisable();
	TEST_ASSERT_FALSE(LATBbits.LATB0);
}

void test_voltageRegulatorDisable_calledOnceAfterEnabledTwice_expectEnablePinIsHigh(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();
	voltageRegulatorEnable();
	voltageRegulatorDisable();
	TEST_ASSERT_TRUE(LATBbits.LATB2);

	callScheduleHandlerAndForget();
	TEST_ASSERT_TRUE(LATBbits.LATB2);

	callScheduleHandlerAndForget();
	TEST_ASSERT_TRUE(LATBbits.LATB2);
}

void test_voltageRegulatorDisable_calledOnceAfterEnabledTwice_expectVmcuSelPinIsHigh(void)
{
	voltageRegulatorInitialise();
	fullyEnableVoltageRegulator();
	voltageRegulatorEnable();
	voltageRegulatorDisable();
	TEST_ASSERT_TRUE(LATBbits.LATB0);

	callScheduleHandlerIfPresentAndForget();
	TEST_ASSERT_TRUE(LATBbits.LATB0);

	callScheduleHandlerIfPresentAndForget();
	TEST_ASSERT_TRUE(LATBbits.LATB0);
}

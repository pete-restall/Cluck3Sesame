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

void test_voltageRegulatorDisable_calledMoreTimesThanEnable_expectEnableCountDoesNotGetOutOfSync(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();
	voltageRegulatorDisable();
	voltageRegulatorDisable();
	voltageRegulatorDisable();
	voltageRegulatorEnable();
	TEST_ASSERT_TRUE_MESSAGE(LATBbits.LATB2, "EN");

	voltageRegulatorDisable();
	TEST_ASSERT_FALSE_MESSAGE(LATBbits.LATB2, "DIS");
}

void test_voltageRegulatorDisable_calledWhenAlreadyDisabled_expectNoEventIsPublished(void)
{
	voltageRegulatorInitialise();

	eventPublish_StubWithCallback(assertEventPublishNotCalled);
	voltageRegulatorDisable();
}

void test_voltageRegulatorDisable_calledWhenStillEnabled_expectNoEventIsPublished(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();
	voltageRegulatorEnable();

	eventPublish_StubWithCallback(assertEventPublishNotCalled);
	voltageRegulatorDisable();
}

void test_voltageRegulatorDisable_calledWhenNotEnabled_expectVoltageRegulatorDisabledEventIsNotPublished(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();

	const struct VoltageRegulatorDisabled emptyArgs = { };
	eventPublish_StubWithCallback(assertEventPublishNotCalled);
	voltageRegulatorDisable();
}

void test_voltageRegulatorDisable_calledWhenVoltageRailHasStabilised_expectVoltageRegulatorDisabledEventIsNotPublished(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();
	callScheduleHandlerAndForget();

	const struct VoltageRegulatorDisabled emptyArgs = { };
	eventPublish_StubWithCallback(assertEventPublishNotCalled);
	voltageRegulatorDisable();
}

void test_voltageRegulatorDisable_calledWhenDisabled_expectVoltageRegulatorDisabledEventIsPublished(void)
{
	voltageRegulatorInitialise();
	fullyEnableVoltageRegulator();

	const struct VoltageRegulatorDisabled emptyArgs = { };
	eventPublish_Expect(VOLTAGE_REGULATOR_DISABLED, &emptyArgs);
	voltageRegulatorDisable();
}

void test_voltageRegulatorDisable_calledBeforeBeingFullyEnabled_expectNoEventPublished(void)
{
	voltageRegulatorInitialise();
	voltageRegulatorEnable();
	callScheduleHandlerAndForget();
	voltageRegulatorDisable();

	eventPublish_StubWithCallback(assertEventPublishNotCalled);
	callScheduleHandlerAndForget();
}

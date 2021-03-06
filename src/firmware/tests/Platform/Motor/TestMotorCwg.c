#include <xc.h>
#include <stdint.h>
#include <unity.h>

#include "Platform/Event.h"
#include "Platform/Motor.h"

#include "MotorFixture.h"

#include "../../Fixture.h"
#include "../../NonDeterminism.h"

TEST_FILE("Platform/Motor/MotorInitialise.c")
TEST_FILE("Platform/Motor/MotorEnableDisable.c")
TEST_FILE("Platform/Motor/MotorOnOff.c")

void onBeforeTest(void)
{
	motorFixtureSetUp();
	motorInitialise();
	PMD3bits.PWM4MD = 0;
}

void onAfterTest(void)
{
	motorFixtureTearDown();
}

void test_voltageRegulatorEnabled_onPublished_expectCwgIsEnabled(void)
{
	CWG1CON0 = anyByteWithMaskClear(_CWG1CON0_EN_MASK);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_TRUE(CWG1CON0bits.EN);
}

void test_voltageRegulatorEnabled_onPublished_expectDeadBandBufferLoadIsClear(void)
{
	CWG1CON0bits.EN = 1;
	CWG1CON0 = anyByteWithMaskSet(_CWG1CON0_LD_MASK);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_FALSE(CWG1CON0bits.LD);
}

void test_voltageRegulatorEnabled_onPublished_expectSynchronousSteeringMode(void)
{
	CWG1CON0 = anyByte();
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(1, CWG1CON0bits.MODE);
}

void test_voltageRegulatorEnabled_onPublished_expectManualShutdownIsClear(void)
{
	CWG1AS0 = anyByteWithMaskSet(_CWG1AS0_SHUTDOWN_MASK);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_FALSE(CWG1AS0bits.SHUTDOWN);
}

void test_voltageRegulatorEnabled_onPublished_expectAutoShutdownDoesNotRestart(void)
{
	CWG1AS0 = anyByteWithMaskSet(_CWG1AS0_REN_MASK);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_FALSE(CWG1AS0bits.REN);
}

void test_voltageRegulatorEnabled_onPublished_expectAutoShutdownForcesOnesOnBAndDOutputsForBraking(void)
{
	CWG1AS0 = anyByte();
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL(0b11, CWG1AS0bits.LSBD);
}

void test_voltageRegulatorEnabled_onPublished_expectAutoShutdownForcesOnesOnAAndCOutputsForBraking(void)
{
	CWG1AS0 = anyByte();
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL(0b11, CWG1AS0bits.LSAC);
}

void test_voltageRegulatorEnabled_onPublished_expectAutoShutdownOnClc2Output(void)
{
	CWG1AS1 = anyByteWithMaskClear(_CWG1AS1_AS4E_MASK);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_TRUE(CWG1AS1bits.AS4E);
}

void test_voltageRegulatorEnabled_onPublished_expectNoAutoShutdownOnComparator2Output(void)
{
	CWG1AS1 = anyByteWithMaskSet(_CWG1AS1_AS3E_MASK);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_FALSE(CWG1AS1bits.AS3E);
}

void test_voltageRegulatorEnabled_onPublished_expectNoAutoShutdownOnComparator1Output(void)
{
	CWG1AS1 = anyByteWithMaskSet(_CWG1AS1_AS2E_MASK);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_FALSE(CWG1AS1bits.AS2E);
}

void test_voltageRegulatorEnabled_onPublished_expectNoAutoShutdownOnTimer2(void)
{
	CWG1AS1 = anyByteWithMaskSet(_CWG1AS1_AS1E_MASK);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_FALSE(CWG1AS1bits.AS1E);
}

void test_voltageRegulatorEnabled_onPublished_expectNoAutoShutdownOnCwgInputPin(void)
{
	CWG1AS1 = anyByteWithMaskSet(_CWG1AS1_AS0E_MASK);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_FALSE(CWG1AS1bits.AS0E);
}

void test_voltageRegulatorEnabled_onPublished_expectSteeringOverridesAreZero(void)
{
	CWG1STR = anyByteWithMaskSet(0xf0);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(0, CWG1STR & 0xf0);
}

void test_voltageRegulatorEnabled_onPublished_expectCwgInterruptFlagIsClear(void)
{
	PIR7 = anyByteWithMaskSet(_PIR7_CWG1IF_MASK);
	uint8_t originalPir7 = PIR7;
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(originalPir7 & ~_PIR7_CWG1IF_MASK, PIR7);
}

void test_voltageRegulatorEnabled_onPublished_expectCwgInterruptFlagWakesDeviceFromSleep(void)
{
	PIE7 = anyByteWithMaskClear(_PIE7_CWG1IE_MASK);
	uint8_t originalPie7 = PIE7;
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(originalPie7 | _PIE7_CWG1IE_MASK, PIE7);
}

void test_voltageRegulatorEnabled_onPublished_expectAllOutputsAreOverridden(void)
{
	CWG1STR = anyByteWithMaskSet(0x0f);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(0, CWG1STR & 0x0f);
}

void test_voltageRegulatorEnabled_onPublished_expectClockSourceIsFosc(void)
{
	CWG1CLKCON = anyByteWithMaskClear(1);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(1, CWG1CLKCON);
}

void test_voltageRegulatorEnabled_onPublished_expectInputSourceIsPwm4(void)
{
	CWG1DAT = anyByteExcept(4);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(4, CWG1DAT);
}

void test_voltageRegulatorEnabled_onPublished_expectPwmAIsMappedToRc6(void)
{
	RC6PPS = anyByteExcept(0x05);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(0x05, RC6PPS);
}

void test_voltageRegulatorEnabled_onPublished_expectPwmBIsMappedToRc7(void)
{
	RC7PPS = anyByteExcept(0x06);
	publishVoltageRegulatorEnabled();
	dispatchAllEvents();
	TEST_ASSERT_EQUAL_UINT8(0x06, RC7PPS);
}

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
}

void onAfterTest(void)
{
	motorFixtureTearDown();
}

void test_motorInitialise_called_expectMotorPortCPinsAreAllOutputs(void)
{
	static const uint8_t usedPins =
		_TRISC_TRISC2_MASK |
		_TRISC_TRISC3_MASK |
		_TRISC_TRISC6_MASK |
		_TRISC_TRISC7_MASK;

	TRISC = anyByteWithMaskSet(usedPins);
	uint8_t originalTrisc = TRISC;
	motorInitialise();
	TEST_ASSERT_EQUAL_UINT8(originalTrisc & ~usedPins, TRISC);
}

void test_motorInitialise_called_expectMotorCurrentSensePinIsOutput(void)
{
	TRISB = anyByteWithMaskSet(_TRISB_TRISB1_MASK);
	uint8_t originalTrisb = TRISB;
	motorInitialise();
	TEST_ASSERT_EQUAL_UINT8(originalTrisb & ~_TRISB_TRISB1_MASK, TRISB);
}

void test_motorInitialise_called_expectMotorPortCPinsAreAllDigital(void)
{
	static const uint8_t usedPins =
		_ANSELC_ANSC2_MASK |
		_ANSELC_ANSC3_MASK |
		_ANSELC_ANSC6_MASK |
		_ANSELC_ANSC7_MASK;

	ANSELC = anyByteWithMaskSet(usedPins);
	uint8_t originalAnselc = ANSELC;
	motorInitialise();
	TEST_ASSERT_EQUAL_UINT8(originalAnselc & ~usedPins, ANSELC);
}

void test_motorInitialise_called_expectMotorCurrentSensePinIsAnalogue(void)
{
	ANSELB = anyByteWithMaskClear(_ANSELB_ANSB1_MASK);
	uint8_t originalAnselb = ANSELB;
	motorInitialise();
	TEST_ASSERT_EQUAL_UINT8(originalAnselb | _ANSELB_ANSB1_MASK, ANSELB);
}

void test_motorInitialise_called_expectMotorPortCPinsAreAllLow(void)
{
	static const uint8_t usedPins =
		_LATC_LATC2_MASK |
		_LATC_LATC3_MASK |
		_LATC_LATC6_MASK |
		_LATC_LATC7_MASK;

	LATC = anyByteWithMaskSet(usedPins);
	uint8_t originalLatc = LATC;
	motorInitialise();
	TEST_ASSERT_EQUAL_UINT8(originalLatc & ~usedPins, LATC);
}

void test_motorInitialise_called_expectMotorCurrentSensePinIsLow(void)
{
	LATB = anyByteWithMaskSet(_LATB_LATB1_MASK);
	uint8_t originalLATB = LATB;
	motorInitialise();
	TEST_ASSERT_EQUAL_UINT8(originalLATB & ~_LATB_LATB1_MASK, LATB);
}

#include <xc.h>
#include <stdint.h>

#include "Event.h"
#include "NearScheduler.h"
#include "VoltageRegulator.h"

static void onRegulatedVoltageRailStabilised(void *state);
static void onMcuVoltageRailStabilised(void *state);

static uint8_t enableCount;
static uint8_t fullyEnabled;

void voltageRegulatorInitialise(void)
{
	ANSELBbits.ANSB0 = 0;
	ANSELBbits.ANSB2 = 0;
	LATBbits.LATB0 = 0;
	LATBbits.LATB2 = 0;
	TRISBbits.TRISB0 = 0;
	TRISBbits.TRISB2 = 0;
	enableCount = 0;
	fullyEnabled = 0;
}

void voltageRegulatorEnable(void)
{
	LATBbits.LATB2 = 1;
	if (++enableCount > 1)
		return;

	static const struct NearSchedule waitForRegulatedVoltageRailToStabilise =
	{
		.ticks = MS_TO_TICKS(64),
		.handler = onRegulatedVoltageRailStabilised
	};

	nearSchedulerAdd(&waitForRegulatedVoltageRailToStabilise);
}

static void onRegulatedVoltageRailStabilised(void *state)
{
	if (!enableCount)
		return;

	LATBbits.LATB0 = 1;

	static const struct NearSchedule waitForMcuVoltageRailToStabilise =
	{
		.ticks = MS_TO_TICKS(4),
		.handler = onMcuVoltageRailStabilised
	};

	nearSchedulerAdd(&waitForMcuVoltageRailToStabilise);
}

static void onMcuVoltageRailStabilised(void *state)
{
	if (!enableCount)
		return;

	eventPublish(VOLTAGE_REGULATOR_ENABLED, &eventEmptyArgs);
	fullyEnabled = 1;
}

void voltageRegulatorDisable(void)
{
	if (enableCount == 0)
		return;

	if (--enableCount == 0 && fullyEnabled)
	{
		eventPublish(VOLTAGE_REGULATOR_DISABLED, &eventEmptyArgs);
		fullyEnabled = 0;
	}

	LATBbits.LATB0 = 0;
	LATBbits.LATB2 = 0;
}

uint8_t voltageRegulatorIsEnabled(void)
{
	return fullyEnabled;
}

#include <xc.h>
#include <stdint.h>

#include "../Platform/NvmSettings.h"
#include "../Platform/Motor.h"
#include "../ApplicationNvmSettings.h"

#include "Door.h"

void doorOnCloseScheduleActioned(const struct Event *event)
{
	switch (doorState.current)
	{
		case DoorState_Closed:
			doorState.transition = DoorTransition_Unchanged;
			break;

		case DoorState_Opened:
			motorEnable();
			doorStartClosing(
				DoorState_Closing,
				DoorState_Closing_WaitingForEnabledMotor);
			break;

		case DoorState_Unknown:
			motorEnable();
			doorStartFindingBottom();

		default:
			doorState.transition = DoorTransition_Close;
	};
}

void doorStartClosing(
	enum DoorState motorEnabledState,
	enum DoorState motorDisabledState)
{
	if (motorIsEnabled())
	{
		motorLimitIsNoLoad();
		motorOn(-nvmSettings.application.door.height);
		doorState.current = motorEnabledState;
	}
	else
		doorState.current = motorDisabledState;

	doorState.transition = DoorTransition_Close;
}

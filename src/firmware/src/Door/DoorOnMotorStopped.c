#include <xc.h>
#include <stdint.h>

#include "../Platform/NvmSettings.h"
#include "../Platform/Motor.h"
#include "../ApplicationNvmSettings.h"

#include "Door.h"

void doorOnMotorStopped(const struct Event *event)
{
	const struct MotorStopped *args = (const struct MotorStopped *) event->args;
	switch (doorState.current)
	{
		case DoorState_Opening:
			if (args->fault.any)
			{
				motorDisable();
				doorState.current = DoorState_Fault;
				doorState.aborted.fault.all =
					args->fault.currentLimited
						? DOOR_JAMMED
						: args->fault.encoderTimeout
							? ENCODER_BROKEN
							: args->fault.encoderOverflow
								? LINE_TOO_LONG
								: 0;

				eventPublish(DOOR_ABORTED, &doorState.aborted);
			}
			else if (doorState.transition != DoorTransition_Close)
			{
				motorDisable();
				doorState.current = DoorState_Opened;
				doorState.transition = DoorTransition_Unchanged;
				eventPublish(DOOR_OPENED, &doorState.opened);
			}
			else
			{
				eventPublish(DOOR_OPENED, &doorState.opened);
				doorStartClosing(DoorState_Closing, DoorState_Closing);
			}

			break;

		case DoorState_Closing:
			if (args->fault.any)
			{
				motorDisable();
				doorState.current = DoorState_Fault;
				doorState.aborted.fault.all =
					args->fault.currentLimited
						? DOOR_REVERSED
						: args->fault.encoderTimeout
							? ENCODER_BROKEN
							: args->fault.encoderOverflow
								? LINE_TOO_LONG
								: 0;

				eventPublish(DOOR_ABORTED, &doorState.aborted);
			}
			else if (doorState.transition != DoorTransition_Open)
			{
				motorDisable();
				doorState.current = DoorState_Closed;
				doorState.transition = DoorTransition_Unchanged;
				eventPublish(DOOR_CLOSED, &doorState.closed);
			}
			else
			{
				eventPublish(DOOR_CLOSED, &doorState.closed);
				doorStartOpening(DoorState_Opening, DoorState_Opening);
			}

			break;

		case DoorState_FindBottom:
			if (args->requestedCount < 0)
			{
				if (args->fault.any)
				{
					motorDisable();
					doorState.current = DoorState_Fault;
					doorState.aborted.fault.all =
						args->fault.currentLimited
							? DOOR_REVERSED
							: args->fault.encoderTimeout
								? ENCODER_BROKEN
								: args->fault.encoderOverflow
									? LINE_TOO_LONG
									: 0;

					eventPublish(DOOR_ABORTED, &doorState.aborted);
				}
				else
				{
					motorOn(FIND_BOTTOM_RAISING);
					doorState.findBottomIterations++;
				}
			}
			else
			{
				if (args->fault.currentLimited)
				{
					if (args->actualCount > 157)//FIND_BOTTOM_THRESHOLD) // TODO: ABOUT 10% OF THE FIND_BOTTOM_LOWERING
					{
						eventPublish(DOOR_CLOSED, &doorState.closed);
						if (doorState.transition != DoorTransition_Open)
						{
							motorDisable();
							doorState.current = DoorState_Closed;
							doorState.transition = DoorTransition_Unchanged;
						}
						else
						{
							doorStartOpening(DoorState_Opening, DoorState_Opening);
						}
					}
					else
					{
						if (doorState.findBottomIterations == MAX_FIND_BOTTOM_ITERATIONS)
						{
							motorDisable();
							doorState.current = DoorState_Fault;
							doorState.aborted.fault.all = LINE_TOO_LONG;
							eventPublish(DOOR_ABORTED, &doorState.aborted);
						}
						else
							motorOn(FIND_BOTTOM_LOWERING);
					}
				}
				else
				{
					motorDisable();
					doorState.current = DoorState_Fault;
					doorState.aborted.fault.all =
						(args->fault.all == 0 || args->fault.encoderOverflow)
							? LINE_SNAPPED
							: args->fault.encoderTimeout
								? ENCODER_BROKEN
								: 0;

					eventPublish(DOOR_ABORTED, &doorState.aborted);
				}
			}

			break;

		case DoorState_Fault:
		case DoorState_Unknown:
			break;

		case DoorState_Opening_WaitingForEnabledMotor:
		case DoorState_Closing_WaitingForEnabledMotor:
			motorDisable();

		default:
			doorState.current = DoorState_Unknown;
	};
}

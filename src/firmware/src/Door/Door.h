#ifndef __CLUCK3SESAME_SRC_DOOR_DOOR_H
#define __CLUCK3SESAME_SRC_DOOR_DOOR_H
#include "../Event.h"
#include "../Door.h"

#define FIND_BOTTOM_LOWERING -1576
#define FIND_BOTTOM_RAISING 15756

struct DoorStateInternal
{
	enum DoorState current;
	enum DoorTransition transition;
	struct DoorOpened opened;
	struct DoorClosed closed;
	struct DoorAborted aborted;
};

extern struct DoorStateInternal doorState;

extern void doorOnOpenScheduleActioned(const struct Event *event);
extern void doorStartOpening(
	enum DoorState motorEnabledState,
	enum DoorState motorDisabledState);

extern void doorStartFindingBottom(void);

extern void doorOnCloseScheduleActioned(const struct Event *event);
extern void doorStartClosing(
	enum DoorState motorEnabledState,
	enum DoorState motorDisabledState);

extern void doorOnMotorStopped(const struct Event *event);

#endif

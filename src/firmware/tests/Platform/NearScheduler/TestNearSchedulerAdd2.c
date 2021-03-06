#include <xc.h>
#include <stdint.h>
#include <unity.h>

#include "Platform/NearScheduler.h"

#include "NearSchedulerFixture.h"

#include "../../Fixture.h"
#include "../../NonDeterminism.h"

#define MAX_SCHEDULES 8

TEST_FILE("Platform/NearScheduler.c")
TEST_FILE("NearSchedulerFixture.c")

static void assertNcoInterruptFlagIsClear(void *state);
static void handlerAddingAnotherSchedule(void *state);

void test_nearSchedulerAdd_called_expectHandlerIsNotCalledOnTickRollover(void)
{
	struct NearSchedule schedule =
	{
		.ticks = 0,
		.handler = &spyHandler,
		.state = (void *) ((int) anyWord())
	};

	nearSchedulerAdd(&schedule);
	for (uint16_t i = 0; i < 513; i++)
		tick();

	assertHandlerCalledTimes(1);
}

void test_nearSchedulerAdd_called_expectHandlerIsCalledWithClearedNcoInterruptFlag(void)
{
	struct NearSchedule schedule =
	{
		.ticks = 0,
		.handler = &assertNcoInterruptFlagIsClear,
		.state = (void *) ((int) anyWord())
	};

	nearSchedulerAdd(&schedule);
	tick();
}

static void assertNcoInterruptFlagIsClear(void *state)
{
	TEST_ASSERT_FALSE(PIR7bits.NCO1IF);
}

void test_nearSchedulerAdd_calledMoreThanBufferSize_expectExistingHandlersAreNotOverwritten(void)
{
	struct NearSchedule schedules[MAX_SCHEDULES + 1];
	for (uint8_t i = 0; i < MAX_SCHEDULES + 1; i++)
	{
		schedules[i].ticks = 0;
		schedules[i].handler = &spyHandler;
		schedules[i].state = (void *) ((int) i);
		nearSchedulerAdd(&schedules[i]);
	}

	tick();
	assertHandlerCalledTimes(MAX_SCHEDULES);
	for (uint8_t i = 0; i < MAX_SCHEDULES; i++)
		assertHandlerCalledWith(schedules[i].state);
}

void test_nearSchedulerAdd_calledWithHandlerThatAddsAnotherScheduleWhenBufferExceeded_expectNewHandlerIsInserted(void)
{
	struct NearSchedule schedule =
	{
		.ticks = 0,
		.handler = &handlerAddingAnotherSchedule,
		.state = (void *) ((int) anyWord())
	};

	nearSchedulerAdd(&schedule);

	static const struct NearSchedule pendingSchedule =
	{
		.ticks = 3,
		.handler = &dummyHandler
	};

	for (uint8_t i = 0; i < MAX_SCHEDULES - 1; i++)
		nearSchedulerAdd(&pendingSchedule);

	tick();
	tick();
	assertHandlerCalledOnceWith(schedule.state);
}

static void handlerAddingAnotherSchedule(void *state)
{
	struct NearSchedule anotherSchedule =
	{
		.ticks = 0,
		.handler = &spyHandler,
		.state = state
	};

	nearSchedulerAdd(&anotherSchedule);
}

void test_nearSchedulerAdd_calledWhenPendingSchedules_expectNcoIsEnabled(void)
{
	struct NearSchedule schedule =
	{
		.ticks = 0,
		.handler = &dummyHandler
	};

	nearSchedulerAdd(&schedule);

	static const struct NearSchedule pendingSchedule =
	{
		.ticks = 2,
		.handler = &dummyHandler
	};

	nearSchedulerAdd(&pendingSchedule);

	uint8_t originalNco1con = NCO1CON;
	tick();
	TEST_ASSERT_EQUAL_UINT8(originalNco1con | _NCO1CON_N1EN_MASK, NCO1CON);
}

void test_nearSchedulerAdd_calledWhenPendingScheduleAddedFromLastHandler_expectNcoRemainsEnabled(void)
{
	static const struct NearSchedule schedule =
	{
		.ticks = 0,
		.handler = &handlerAddingAnotherSchedule
	};

	nearSchedulerAdd(&schedule);

	uint8_t originalNco1con = NCO1CON;
	tick();
	TEST_ASSERT_EQUAL_UINT8(originalNco1con | _NCO1CON_N1EN_MASK, NCO1CON);
}

void test_nearSchedulerAdd_calledWhenNoPendingSchedules_expectNcoIsDisabled(void)
{
	static const struct NearSchedule schedule =
	{
		.ticks = 0,
		.handler = &dummyHandler
	};

	nearSchedulerAdd(&schedule);

	uint8_t originalNco1con = NCO1CON;
	tick();
	TEST_ASSERT_EQUAL_UINT8(originalNco1con & ~_NCO1CON_N1EN_MASK, NCO1CON);
}

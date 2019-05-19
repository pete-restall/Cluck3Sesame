#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Clock.h"

static struct DateAndTimeGet clockNow;

void clockGetNowGmt(struct DateAndTimeGet *const now)
{
	clockNow.time.second = TMR0L;
	memcpy(now, &clockNow, sizeof(struct DateAndTimeGet));
}

void clockSetNowGmt(const struct DateAndTimeSet *const now)
{
	memcpy(&clockNow, now, sizeof(struct DateAndTimeSet));
	clockNow.date.flags.all = 0;
	clockNow.date.flags.isLeapYear = 1;
	TMR0L = clockNow.time.second;
	// TODO: SET isDaylightSavings, isLeapYear, dayOfYear CORRECTLY, ETC.
	// TODO: SET TMR0L WITH SECONDS
	// TODO: PUBLISH DATE_CHANGED EVENT
}

void clockTicked(void)
{
	if (++clockNow.time.minute == 60)
	{
		if (++clockNow.time.hour == 24)
		{
			uint8_t mask30Days = (clockNow.date.month & 0b1001);
			uint8_t daysInMonth;
			if (clockNow.date.month == 2)
			{
				if (clockNow.date.year & 3) // TODO: REFACTOR TO USE isLeapYear FLAG, WHEN THAT FUNCTIONALITY IS WRITTEN
					daysInMonth = 28;
				else
					daysInMonth = 29;
			}
			else if (mask30Days == 0b1001 || mask30Days == 0b0000)
				daysInMonth = 30;
			else
				daysInMonth = 31;

			if (clockNow.date.day == daysInMonth)
			{
				if (clockNow.date.month++ == 12)
				{
					if (++clockNow.date.year == 100)
						clockNow.date.year = 0;

					clockNow.date.month = 1;
					clockNow.date.dayOfYear = UINT16_MAX;
				}

				clockNow.date.day = 1;
			}
			else
				clockNow.date.day++;

			clockNow.date.dayOfYear++;
			clockNow.time.hour = 0;
		}

		clockNow.time.minute = 0;
	}
}

#include <xc.h>
#include <stdint.h>

#include "../Platform/Event.h"
#include "../Platform/Buttons.h"

#include "Ui.h"

#define LEFT_BUTTON 0x02
#define RIGHT_BUTTON 0x01

static void uiInputIncrementScreenCharacter(void);
static void uiInputToggleScreenCharacter(void);
static void uiInputMoveCursorToNextOption(void);
static void uiInputOnPreEnter(void);
static void uiInputOnEnter(void);

const struct ButtonBehaviour uiInputIgnore = { };

const struct ButtonBehaviour uiInputIncrementRange =
{
	.onPressed = &uiInputIncrementScreenCharacter
};

const struct ButtonBehaviour uiInputToggleRangeOfTwo =
{
	.onPressed = &uiInputToggleScreenCharacter
};

const struct ButtonBehaviour uiInputMoveToNextOption =
{
	.onPressed = &uiInputMoveCursorToNextOption
};

const struct ButtonBehaviour uiInputEntered =
{
	.onPressed = &uiInputOnPreEnter,
	.onReleased = &uiInputOnEnter
};

const struct ButtonsBehaviour uiInputIsUninitialised =
{
	.left = &uiInputIgnore,
	.right = &uiInputIgnore
};

const struct ButtonsBehaviour uiInputIsRange =
{
	.left = &uiInputIncrementRange,
	.right = &uiInputEntered
};

const struct ButtonsBehaviour uiInputIsRangeOfTwo =
{
	.left = &uiInputToggleRangeOfTwo,
	.right = &uiInputEntered
};

const struct ButtonsBehaviour uiInputIsOptions =
{
	.left = &uiInputMoveToNextOption,
	.right = &uiInputEntered
};

const struct ButtonsBehaviour uiInputIsStatusScreen =
{
	.left = &uiInputIgnore,
	.right = &uiInputEntered
};

void uiInputOnButtonsPressed(const struct Event *event)
{
	const struct ButtonsPressed *pressed = (const struct ButtonsPressed *) event->args;

	if (pressed->mask & LEFT_BUTTON)
		uiState.flags.bits.isLeftButtonPressed = 1;

	if (pressed->mask & RIGHT_BUTTON)
		uiState.flags.bits.isRightButtonPressed = 1;

	if (!uiState.flags.bits.isScreenOn)
	{
		uiScreenOn();
		uiState.flags.bits.isButtonPressTurningOnScreen = 1;
		uiState.flags.bits.isScreenTimeoutDisabled = 1;
		return;
	}

	uiState.flags.bits.isButtonPressTurningOnScreen = 0;
	uiState.flags.bits.isScreenTimeoutDisabled = 1;

	if ((pressed->mask & LEFT_BUTTON) && uiState.input.buttons->left->onPressed)
		uiState.input.buttons->left->onPressed();

	if ((pressed->mask & RIGHT_BUTTON) && uiState.input.buttons->right->onPressed)
		uiState.input.buttons->right->onPressed();
}

void uiInputOnButtonsReleased(const struct Event *event)
{
	const struct ButtonsReleased *released = (const struct ButtonsReleased *) event->args;

	if (released->mask & LEFT_BUTTON)
	{
		uiState.flags.bits.isLeftButtonPressed = 0;
		if (!uiState.flags.bits.isButtonPressTurningOnScreen && uiState.input.buttons->left->onReleased)
			uiState.input.buttons->left->onReleased();
	}

	if (released->mask & RIGHT_BUTTON)
	{
		uiState.flags.bits.isRightButtonPressed = 0;
		if (!uiState.flags.bits.isButtonPressTurningOnScreen && uiState.input.buttons->right->onReleased)
			uiState.input.buttons->right->onReleased();
	}

	if (!uiState.flags.bits.isLeftButtonPressed && !uiState.flags.bits.isRightButtonPressed)
		uiScreenStartTimeout();
}

static void uiInputIncrementScreenCharacter(void)
{
	if (uiState.input.cursorPosition >= sizeof(uiState.screen))
		return;

	if (++uiState.screen[uiState.input.cursorPosition] > uiState.input.menu.range.max)
		uiState.screen[uiState.input.cursorPosition] = uiState.input.menu.range.min;

	uiScreenBlit();
}

static void uiInputToggleScreenCharacter(void)
{
	if (uiState.input.cursorPosition >= sizeof(uiState.screen))
		return;

	if (uiState.screen[uiState.input.cursorPosition] == uiState.input.menu.range.max)
		uiState.screen[uiState.input.cursorPosition] = uiState.input.menu.range.min;
	else
		uiState.screen[uiState.input.cursorPosition] = uiState.input.menu.range.max;

	uiScreenBlit();
}

static void uiInputMoveCursorToNextOption(void)
{
	if (uiState.input.cursorPosition >= sizeof(uiState.screen))
		return;

	uiState.screen[uiState.input.cursorPosition] = ' ';

	if (++uiState.input.selectedOptionIndex >= sizeof(uiState.input.menu.options.cursorPositions))
		uiState.input.selectedOptionIndex = 0;

	uiState.input.cursorPosition = uiState.input.menu.options.cursorPositions[uiState.input.selectedOptionIndex];
	if (uiState.input.cursorPosition >= sizeof(uiState.screen))
	{
		uiState.input.cursorPosition = uiState.input.menu.options.cursorPositions[0];
		uiState.input.selectedOptionIndex = 0;
	}

	uiState.screen[uiState.input.cursorPosition] = '>';

	uiScreenBlit();
}

static void uiInputOnPreEnter(void)
{
	if (uiState.input.onPreEnter)
		uiState.input.onPreEnter();
}

static void uiInputOnEnter(void)
{
	if (uiState.input.onEnter)
		uiState.input.onEnter();
}

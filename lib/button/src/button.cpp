#include "button.h"
#include "bsp_gpio.h"

ButtonHandler::ButtonHandler(int pin, bool isActiveLow, bool usePullup)
    : buttonPin(pin), activeLow(isActiveLow), debounceDuration(50), doubleClickInterval(400),
      holdDuration(800), singleClickCallback(nullptr), doubleClickCallback(nullptr),
      holdStartCallback(nullptr), holdReleaseCallback(nullptr), lastButtonState(false), lastDebounceTime(0),
      lastClickTime(0), isHolding(false), holdStartTime(0), clickCount(0)
{
  if (pin >= 0)
  {
    bspGpioPinMode(pin, usePullup ? INPUT_PULLUP : INPUT);
  }
}

button_handler_error_t ButtonHandler::setDebounceDuration(unsigned int ms)
{
  if (ms < 1)
  {
    return BUTTON_ERR_INVALID_PARAM;
  }
  debounceDuration = ms;
  return BUTTON_OK;
}

button_handler_error_t ButtonHandler::setDoubleClickInterval(unsigned int ms)
{
  if (ms < 1)
  {
    return BUTTON_ERR_INVALID_PARAM;
  }
  doubleClickInterval = ms;
  return BUTTON_OK;
}

button_handler_error_t ButtonHandler::setHoldDuration(unsigned int ms)
{
  if (ms < 1)
  {
    return BUTTON_ERR_INVALID_PARAM;
  }
  holdDuration = ms;
  return BUTTON_OK;
}

button_handler_error_t ButtonHandler::attachSingleClickCallback(ButtonCallback callback)
{
  if (callback == nullptr)
  {
    return BUTTON_ERR_UNINITIALIZED;
  }
  singleClickCallback = callback;
  return BUTTON_OK;
}

button_handler_error_t ButtonHandler::attachDoubleClickCallback(ButtonCallback callback)
{
  if (callback == nullptr)
  {
    return BUTTON_ERR_UNINITIALIZED;
  }
  doubleClickCallback = callback;
  return BUTTON_OK;
}

button_handler_error_t ButtonHandler::attachHoldStartCallback(ButtonCallback callback)
{
  if (callback == nullptr)
  {
    return BUTTON_ERR_UNINITIALIZED;
  }
  holdStartCallback = callback;
  return BUTTON_OK;
}

button_handler_error_t ButtonHandler::attachHoldReleaseCallback(ButtonCallback callback)
{
  if (callback == nullptr)
  {
    return BUTTON_ERR_UNINITIALIZED;
  }
  holdReleaseCallback = callback;
  return BUTTON_OK;
}

void ButtonHandler::update()
{
  bool          isButtonPressed = bspGpioDigitalRead(buttonPin) == (activeLow ? LOW : HIGH);
  unsigned long currentTime     = millis();

  // FSM for state handling
  switch (currentState)
  {
    case STATE_IDLE:
      if (isButtonPressed)
      {
        // Transition to BUTTON_DOWN on press
        currentState   = STATE_BUTTON_DOWN;
        stateStartTime = currentTime;
        clickCount     = 1; // Start counting clicks
      }
      break;

    case STATE_BUTTON_DOWN:
      if (!isButtonPressed)
      {
        // Button released, move to WAIT_FOR_SECOND_CLICK
        currentState   = STATE_WAIT_FOR_SECOND_CLICK;
        stateStartTime = currentTime;
      }
      else if (hasElapsed(stateStartTime, holdDuration))
      {
        // Button held long enough, move to HOLDING
        currentState = STATE_HOLDING;
        isHolding    = true;
        if (holdStartCallback)
          holdStartCallback();
      }
      break;

    case STATE_WAIT_FOR_SECOND_CLICK:
      if (isButtonPressed)
      {
        // Second press detected
        clickCount++;
        currentState   = STATE_BUTTON_DOWN;
        stateStartTime = currentTime;
      }
      else if (hasElapsed(stateStartTime, doubleClickInterval))
      {
        // Timeout, determine single/double click
        if (clickCount == 1 && singleClickCallback)
        {
          singleClickCallback();
        }
        else if (clickCount == 2 && doubleClickCallback)
        {
          doubleClickCallback();
        }
        resetFSM();
      }
      break;

    case STATE_HOLDING:
      if (!isButtonPressed)
      {
        // Button released after holding, move to HOLD_RELEASE
        currentState   = STATE_HOLD_RELEASE;
        stateStartTime = currentTime;
      }
      break;

    case STATE_HOLD_RELEASE:
      if (holdReleaseCallback)
      {
        holdReleaseCallback();
      }
      resetFSM();
      break;

    default:
      resetFSM(); // Safety fallback for undefined states
      break;
  }

  // Track the last known state of the button
  lastButtonState = isButtonPressed;
}

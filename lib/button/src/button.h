#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

// Define error codes for button operations
typedef enum
{
  BUTTON_OK                = 0,
  BUTTON_ERR_INVALID_PARAM = 1,
  BUTTON_ERR_UNINITIALIZED = 2,
} button_handler_error_t;

// Callback function type
typedef void (*ButtonCallback)(void);

/**
 * @brief Manages button presses, handling debouncing, click detection, and hold events.
 *
 * The `ButtonHandler` class provides functionality for processing button interactions,
 * including single-click, double-click, and hold events. It utilizes a finite state machine (FSM)
 * to ensure reliable event detection, minimizing issues related to button bouncing.
 *
 * ### Features:
 *
 * - Configurable debounce duration for stable input.
 *
 * - Detection of single-click, double-click, and hold events.
 *
 * - Support for callback functions triggered by button events.
 *
 * - Internal state tracking for efficient processing.
 *
 * ### Usage:
 *
 * After instantiating the class, users should call the `update()` method periodically to process
 * button states. Callback functions can be attached for various button events to respond dynamically.
 *
 * ### Dependencies:
 *
 * - Requires connection to a digital input pin.
 *
 * - Optional internal pull-up resistor activation.
 *
 * - Callbacks should be defined before use.
 */
class ButtonHandler
{
public:
  /**
   * @brief  Initialize the ButtonHandler instance.
   *
   * @param[in]     pin             Digital pin connected to the button.
   * @param[in]     isActiveLow     True if the button's active state is LOW.
   * @param[in]     usePullup       True to enable internal pull-up resistor.
   *
   * @return
   *  - BUTTON_OK: Initialization successful.
   *
   *  - BUTTON_ERR_INVALID_PARAM: Invalid pin number.
   */
  ButtonHandler(int pin, bool isActiveLow = true, bool usePullup = true);

  /**
   * @brief  Set debounce duration in milliseconds.
   *
   * @param[in]     ms    Debounce duration.
   *
   * @return
   *  - BUTTON_OK: Parameter set successfully.
   *
   *  - BUTTON_ERR_INVALID_PARAM: Invalid duration value.
   */
  button_handler_error_t setDebounceDuration(unsigned int ms);

  /**
   * @brief  Set double-click maximum interval in milliseconds.
   *
   * @param[in]     ms    Maximum time between clicks.
   *
   * @return
   *  - BUTTON_OK: Parameter set successfully.
   *
   *  - BUTTON_ERR_INVALID_PARAM: Invalid duration value.
   */
  button_handler_error_t setDoubleClickInterval(unsigned int ms);

  /**
   * @brief  Set hold duration in milliseconds for triggering hold events.
   *
   * @param[in]     ms    Hold duration.
   *
   * @return
   *  - BUTTON_OK: Parameter set successfully.
   *
   *  - BUTTON_ERR_INVALID_PARAM: Invalid duration value.
   */
  button_handler_error_t setHoldDuration(unsigned int ms);

  /**
   * @brief  Attach a callback for single-click events.
   *
   * @param[in]     callback   Function to be called on single click.
   *
   * @return
   *  - BUTTON_OK: Callback attached successfully.
   *
   *  - BUTTON_ERR_UNINITIALIZED: Button not initialized.
   */
  button_handler_error_t attachSingleClickCallback(ButtonCallback callback);

  /**
   * @brief  Attach a callback for double-click events.
   *
   * @param[in]     callback   Function to be called on double click.
   *
   * @return
   *  - BUTTON_OK: Callback attached successfully.
   *
   *  - BUTTON_ERR_UNINITIALIZED: Button not initialized.
   */
  button_handler_error_t attachDoubleClickCallback(ButtonCallback callback);

  /**
   * @brief  Attach a callback for hold start events.
   *
   * @param[in]     callback   Function to be called when a hold starts.
   *
   * @return
   *  - BUTTON_OK: Callback attached successfully.
   *
   *  - BUTTON_ERR_UNINITIALIZED: Button not initialized.
   */
  button_handler_error_t attachHoldStartCallback(ButtonCallback callback);

  /**
   * @brief  Attach a callback for hold release events.
   *
   * @param[in]     callback   Function to be called when hold is released.
   *
   * @return
   *  - BUTTON_OK: Callback attached successfully.
   *
   *  - BUTTON_ERR_UNINITIALIZED: Button not initialized.
   */
  button_handler_error_t attachHoldReleaseCallback(ButtonCallback callback);

  /**
   * @brief  Periodically process the button state.
   */
  void update();

private:
  int          buttonPin;           // Pin connected to the button.
  bool         activeLow;           // True if button is active low.
  unsigned int debounceDuration;    // Debounce duration in milliseconds.
  unsigned int doubleClickInterval; // Maximum interval for double click.
  unsigned int holdDuration;        // Duration to trigger hold event.

  ButtonCallback singleClickCallback; // Single-click callback function.
  ButtonCallback doubleClickCallback; // Double-click callback function.
  ButtonCallback holdStartCallback;   // Hold-start callback function.
  ButtonCallback holdReleaseCallback; // Hold-release callback function.

  bool          lastButtonState;  // Last known button state.
  unsigned long lastDebounceTime; // Last time button state changed.
  unsigned long lastClickTime;    // Time of the last button release.
  bool          isHolding;        // True if button is currently held.
  unsigned long holdStartTime;    // Start time of the hold action.
  int           clickCount;       // Count of consecutive clicks.

  typedef enum
  {
    STATE_IDLE,
    STATE_BUTTON_DOWN,
    STATE_WAIT_FOR_SECOND_CLICK,
    STATE_HOLDING,
    STATE_HOLD_RELEASE
  } ButtonState;

  ButtonState currentState; // Current state of the FSM

  // Additional helper variables
  unsigned long stateStartTime; // Time when the state started

  inline bool hasElapsed(unsigned long start, unsigned long duration)
  {
    return (millis() - start) >= duration;
  }

  // Reset FSM state
  inline void resetFSM()
  {
    currentState   = STATE_IDLE;
    stateStartTime = 0;
    clickCount     = 0;
    isHolding      = false;
  }
};

#endif // BUTTON_H

/**
 * @file       mini_fan.h
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header for Mini Fan Module library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef MINI_FAN_H
  #define MINI_FAN_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Manages operations of a mini fan module, providing control over speed and state.
 *
 * The `MiniFan` class facilitates interaction with a mini fan module connected to an Arduino pin. It supports
 * setting the fan speed via PWM, toggling the fan state, and retrieving the current speed as a raw PWM value
 * or percentage. The class maintains the fan's state internally for consistent control.
 *
 * ### Features:
 *
 * - Sets fan speed using PWM values (0–255).
 *
 * - Sets and retrieves fan speed as a percentage (0–100%).
 *
 * - Toggles fan state between ON and OFF.
 *
 * - Checks if the fan is currently running.
 *
 * - Stores fan speed and state internally for consistent control.
 *
 * ### Usage:
 *
 * Instantiate the class with a PWM-capable pin. Use `setFanSpeed()` or `setFanSpeedPercentage()` to control
 * the fan speed, `toggleFan()` to switch the fan ON or OFF, and `isFanRunning()` to check the fan's state.
 * Retrieve the current speed with `getFanSpeed()` or `getFanSpeedPercentage()`.
 *
 * ### Dependencies:
 *
 * - Requires an Arduino-compatible board with PWM support.
 *
 * - Fan must be connected to a valid PWM-capable pin.
 *
 * - Depends on `bsp_gpio.h` for GPIO operations (`bspGpioAnalogWrite`, `bspGpioDigitalWrite`).
 */
class MiniFan
{
public:
  /**
   * @brief Constructor to initialize the MiniFan with a specified pin.
   *
   * Configures the MiniFan to use the provided PWM-capable pin as an output for controlling the fan.
   *
   * @param[in] pin The pin number connected to the fan's control signal.
   *
   * @attention Ensure the pin is PWM-capable and properly connected to the fan.
   */
  MiniFan(int pin);

  /**
   * @brief Sets the fan speed using a PWM value.
   *
   * Writes a PWM value to the control pin to set the fan speed and updates the internal speed and state.
   *
   * @param[in] speed The fan speed as a PWM value (0–255).
   *
   * @attention Ensure the speed is within the valid range (0–255).
   */
  void setFanSpeed(int speed);

  /**
   * @brief Sets the fan speed as a percentage.
   *
   * Converts the percentage to a PWM value, writes it to the control pin, and updates the internal speed and
   * state.
   *
   * @param[in] percentage The fan speed as a percentage (0–100%).
   *
   * @attention Ensure the percentage is within the valid range (0–100).
   */
  void setFanSpeedPercentage(int percentage);

  /**
   * @brief Retrieves the current fan speed.
   *
   * Returns the current fan speed as a PWM value stored internally.
   *
   * @param[in] None
   *
   * @return int The current fan speed as a PWM value (0–255).
   */
  int getFanSpeed();

  /**
   * @brief Retrieves the current fan speed as a percentage.
   *
   * Returns the current fan speed as a percentage stored internally.
   *
   * @param[in] None
   *
   * @return int The current fan speed as a percentage (0–100%).
   */
  int getFanSpeedPercentage();

  /**
   * @brief Toggles the fan state between ON and OFF.
   *
   * Switches the fan state: if ON, turns it OFF by setting the PWM to 0; if OFF, turns it ON by restoring the
   * previous speed or defaulting to maximum (PWM 255, 100%) if no prior speed was set.
   *
   * @param[in] None
   *
   * @attention The fan speed is preserved when turning OFF and restored when turning ON, unless no prior
   * speed was set, in which case maximum speed is used.
   */
  void toggleFan();

  /**
   * @brief Checks if the fan is currently running.
   *
   * Returns the current state of the fan based on the internally stored status.
   *
   * @param[in] None
   *
   * @return bool `true` if the fan is running, `false` otherwise.
   */
  bool isFanRunning();

private:
  int  _pin;      /** SIG pin of the Mini Fan */
  int  _speed[2]; /** Speed of the fan */
  bool status;    /**< Status of the fan (ON/OFF) */
};

#endif // MINI_FAN_H

/* End of file -------------------------------------------------------- */
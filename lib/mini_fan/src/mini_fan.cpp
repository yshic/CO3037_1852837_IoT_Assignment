/**
 * @file       mini_fan.cpp
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for Mini Fan Module Library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "mini_fan.h"
#include "bsp_gpio.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method Definitions ---------------------------------- */

// Constructor
MiniFan::MiniFan(int pin) : _pin(pin)
{
  bspGpioPinMode(_pin, OUTPUT);
  _speed[0] = 0;
  _speed[1] = 0;
  status    = false;
}

void MiniFan::setFanSpeed(int speed)
{
  _speed[0]      = speed;
  int percentage = map(_speed[0], 0, 255, 0, 100);
  _speed[1]      = constrain(percentage, 0, 100);

  // Change status of the fan
  status = (_speed[0] > 0);

  // Spin the fan
  bspGpioAnalogWrite(_pin, _speed[0]);
}

void MiniFan::setFanSpeedPercentage(int percentage)
{
  // Ensure the input percentage is within 0-100
  percentage = constrain(percentage, 0, 100);
  _speed[1]  = percentage;

  // Map the percentage to a range of 0-255
  _speed[0] = map(percentage, 0, 100, 0, 255);

  // Set the fan speed using the mapped PWM value
  bspGpioAnalogWrite(_pin, _speed[0]);

  // Update the fan state
  status = (_speed[0] > 0);
}

int MiniFan::getFanSpeed() { return _speed[0]; }

int MiniFan::getFanSpeedPercentage() { return _speed[1]; }

void MiniFan::toggleFan()
{
  if (isFanRunning())
  {
    bspGpioAnalogWrite(_pin, LOW);
    status = false;
  }
  else
  {
    // Restore previous speed or default to maximum if unset
    int targetSpeed = (_speed[0] > 0) ? _speed[0] : 255;
    bspGpioAnalogWrite(_pin, targetSpeed);
    // Update percentage if default speed is used
    if (_speed[0] == 0)
    {
      _speed[0] = 255;
      _speed[1] = 100;
    }
    status = true;
  }
}

bool MiniFan::isFanRunning() { return status; }

/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */

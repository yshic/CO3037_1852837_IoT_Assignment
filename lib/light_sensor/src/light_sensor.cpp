/**
 * @file       light_sensor.cpp
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for Light Sensor Library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "light_sensor.h"
#include "bsp_gpio.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method Definitions ---------------------------------- */

// Constructor
LightSensor::LightSensor(int pin) : _pin(pin)
{
  pinMode(_pin, INPUT);
  sensorValue[0] = 0;
  sensorValue[1] = 0;
  // Change the ADC resolution to 12 bits
  analogReadResolution(12);
}

light_sensor_error_t LightSensor::read()
{
  sensorValue[0] = bspGpioAnalogRead(_pin);
  sensorValue[1] = map(sensorValue[0], 0, 4095, 0, 100);
  sensorValue[1] = constrain(sensorValue[1], 0, 100);
  return LIGHT_SENSOR_OK;
}

int LightSensor::readAndMap(int minValue, int maxValue)
{
  // Read the raw sensor value
  read();

  // Map the sensor value from 0-4095 to a range of minValue - maxValue
  int mappedValue = map(sensorValue[0], 0, 4095, minValue, maxValue);
  mappedValue     = constrain(mappedValue, minValue, maxValue);
  return mappedValue;
}

int LightSensor::getLightValue() { return sensorValue[0]; }

int LightSensor::getLightValuePercentage() { return sensorValue[1]; }

bool LightSensor::isAboveThreshold(int threshold) { return sensorValue[0] > threshold; }

int LightSensor::getAverageReading(int samples)
{
  long total = 0;
  for (int i = 0; i < samples; i++)
  {
    read();
    total += sensorValue[0];
    delay(10); // Small delay between readings
  }
  return total / samples;
}

light_sensor_error_t LightSensor::onThresholdCross(int threshold, void (*callback)())
{
  _callback = callback;
  if (isAboveThreshold(threshold) && _callback != nullptr)
  {
    _callback();
  }
  return LIGHT_SENSOR_OK;
}

/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */

/**
 * @file       light_sensor.h
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header for Light Sensor Library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef LIGHT_SENSOR_H
  #define LIGHT_SENSOR_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

/* Public defines ----------------------------------------------------- */
typedef enum
{
  LIGHT_SENSOR_OK = 0,   /* No error */
  LIGHT_SENSOR_ERR,      /* Generic error */
  LIGHT_SENSOR_ERR_INIT, /* Initialization error */
  LIGHT_SENSOR_TIMEOUT,  /* Timeout error*/
} light_sensor_error_t;

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Manages light intensity measurements using an analog light sensor.
 *
 * The `LightSensor` class provides functionality for reading and processing light intensity data from an
 * analog light sensor connected to a specified Arduino pin. It supports raw analog readings with 12-bit ADC
 * resolution, mapped values, threshold detection, averaging, and callback triggering for threshold events.
 *
 * ### Features:
 *
 * - Reads raw analog light intensity values with 12-bit resolution.
 *
 * - Maps raw values to a user-defined range (e.g., 0–100).
 *
 * - Detects if light intensity exceeds a specified threshold.
 *
 * - Computes average readings over multiple samples.
 *
 * - Supports callback functions for threshold-crossing events.
 *
 * ### Usage:
 *
 * Instantiate the class with the appropriate analog pin. Call `read()` to retrieve raw sensor data or
 * `readAndMap()` for scaled values. Use `isAboveThreshold()` for conditional checks, `getAverageReading()`
 * for smoothed data, or `onThresholdCross()` to set up event-driven callbacks. Regularly call `read()` to
 * update sensor values.
 *
 * ### Dependencies:
 *
 * - Requires an Arduino-compatible board with analog input support (12-bit ADC).
 *
 * - Sensor must be connected to a valid analog pin.
 *
 * - Depends on `bsp_gpio.h` for analog reading (`bspGpioAnalogRead`).
 *
 * - Callback functions must be defined before use with `onThresholdCross()`.
 */
class LightSensor
{
public:
  /**
   * @brief Constructor to initialize the LightSensor with a specified pin.
   *
   * Configures the LightSensor to use the provided analog pin for reading light intensity and sets the ADC
   * resolution to 12 bits.
   *
   * @param[in] pin The analog pin connected to the light sensor.
   *
   * @attention Ensure the pin is a valid analog input pin on the microcontroller.
   */
  LightSensor(int pin);

  /**
   * @brief Reads the analog value from the light sensor.
   *
   * Uses the `bspGpioAnalogRead` function to retrieve the raw 12-bit analog value (0–4095) from the sensor
   * and computes a percentage value (0–100), storing both internally.
   *
   * @param[in] None
   *
   * @attention Ensure the sensor is properly connected to the specified pin.
   *
   * @return
   *  - `LIGHT_SENSOR_OK`: Success
   *
   *  - `LIGHT_SENSOR_ERR`: Reading error
   */
  light_sensor_error_t read();

  /**
   * @brief Reads and maps the sensor value to a specified range.
   *
   * Reads the raw analog value from the sensor and maps it to a user-defined range (e.g., 0–100) using linear
   * scaling.
   *
   * @param[in] minValue Minimum value of the output range.
   * @param[in] maxValue Maximum value of the output range.
   *
   * @attention Ensure `minValue` is less than `maxValue` and the sensor is connected.
   *
   * @return int The mapped sensor value within the specified range.
   */
  int readAndMap(int minValue, int maxValue);

  /**
   * @brief Checks if the light intensity exceeds a threshold.
   *
   * Compares the current raw sensor value against a specified threshold to determine if the light intensity
   * is above it.
   *
   * @param[in] threshold The threshold value for light intensity (0–4095).
   *
   * @attention Requires a prior call to `read()` to update the sensor value.
   *
   * @return bool `true` if the light intensity is above the threshold, `false` otherwise.
   */
  bool isAboveThreshold(int threshold);

  /**
   * @brief Retrieves the raw analog light sensor value.
   *
   * Returns the most recent raw 12-bit analog value (0–4095) read from the sensor.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `read()`.
   *
   * @return int The raw analog sensor value.
   */
  int getLightValue();

  /**
   * @brief Retrieves the light intensity as a percentage.
   *
   * Returns the internally stored percentage value (0–100) computed from the raw analog sensor value.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `read()`.
   *
   * @return int The light intensity as a percentage.
   */
  int getLightValuePercentage();

  /**
   * @brief Computes the average sensor reading over multiple samples.
   *
   * Takes multiple readings from the sensor with a 10ms delay between each and returns their average to
   * reduce noise.
   *
   * @param[in] samples Number of samples to average (default: 10).
   *
   * @attention Ensure the sensor is connected and `samples` is a positive integer.
   *
   * @return int The averaged raw sensor value.
   */
  int getAverageReading(int samples = 10);

  /**
   * @brief Sets a callback function for threshold-crossing events.
   *
   * Configures a callback function to be triggered when the light intensity crosses the specified threshold,
   * invoking the callback if the threshold is currently exceeded.
   *
   * @param[in] threshold The threshold value for triggering the callback (0–4095).
   * @param[in] callback The function to call when the threshold is crossed.
   *
   * @attention The callback function must be defined, have no parameters, and no return value. Requires a
   * prior call to `read()` for accurate threshold detection.
   *
   * @return
   *  - `LIGHT_SENSOR_OK`: Success
   *
   *  - `LIGHT_SENSOR_ERR`: Invalid callback or configuration error
   */
  light_sensor_error_t onThresholdCross(int threshold, void (*callback)());

private:
  int      _pin;                 /** SIG pin of the Light Sensor */
  uint32_t sensorValue[2];       /** Value from the sensor */
  void (*_callback)() = nullptr; /** Callback function */
};

#endif // LIGHT_SENSOR_H

/* End of file -------------------------------------------------------- */
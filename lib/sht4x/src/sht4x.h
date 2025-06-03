/**
 * @file       sht4x.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-21
 * @author     Tuan Nguyen
 *
 * @brief      Header file for SHT4X library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef SHT4X_H
  #define SHT4X_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define SHT4X_LIB_VERSION              (F("0.1.0"))

  #define SHT40_I2C_ADDR_44              0x44
  #define SHT40_I2C_ADDR_45              0x45
  #define SHT41_I2C_ADDR_44              0x44
  #define SHT41_I2C_ADDR_45              0x45
  #define SHT45_I2C_ADDR_44              0x44
  #define SHT45_I2C_ADDR_45              0x45

  #define SHT4X_HUMIDITY_INDEX           0
  #define SHT4X_TEMPERATURE_INDEX        1

  #define SHT4x_CMD_NOHEAT_HIGHPRECISION 0xFD /**< High precision measurement, no heater */
  #define SHT4x_CMD_NOHEAT_MEDPRECISION  0xF6 /**< Medium precision measurement, no heater */
  #define SHT4x_CMD_NOHEAT_LOWPRECISION  0xE0 /**< Low precision measurement, no heater */

  #define SHT4x_CMD_HIGHHEAT_1S          0x39 /**< High precision measurement, high heat for 1 sec */
  #define SHT4x_CMD_HIGHHEAT_100MS       0x32 /**< High precision measurement, high heat for 0.1 sec */
  #define SHT4x_CMD_MEDHEAT_1S           0x2F /**< High precision measurement, med heat for 1 sec */
  #define SHT4x_CMD_MEDHEAT_100MS        0x24 /**< High precision measurement, med heat for 0.1 sec */
  #define SHT4x_CMD_LOWHEAT_1S           0x1E /**< High precision measurement, low heat for 1 sec */
  #define SHT4x_CMD_LOWHEAT_100MS        0x15 /**< High precision measurement, low heat for 0.1 sec */

  #define SHT4x_READSERIAL               0x89 /**< Read Out of Serial Register */
  #define SHT4x_SOFTRESET                0x94 /**< Soft Reset */

/* Public enumerate/structure ----------------------------------------- */

/**
 * @brief Enum for SHT4X sensor error codes.
 */
typedef enum
{
  SHT4X_OK = 0,      /* No error */
  SHT4X_ERR,         /* Generic error */
  SHT4X_ERR_INIT,    /* Initialization error */
  SHT4X_ERR_RESET,   /* Reset error */
  SHT4X_TIMEOUT,     /* Timeout error*/
  SHT4X_ERR_I2C,     /* I2C error */
  SHT4X_ERR_CHECKSUM /* Checksum error */
} sht4x_error_t;

typedef enum
{
  SHT4X_HIGH_PRECISION,
  SHT4X_MED_PRECISION,
  SHT4X_LOW_PRECISION,
} sht4x_precision_t;

/** Optional pre-heater configuration setting */
typedef enum
{
  SHT4X_NO_HEATER,
  SHT4X_HIGH_HEATER_1S,
  SHT4X_HIGH_HEATER_100MS,
  SHT4X_MED_HEATER_1S,
  SHT4X_MED_HEATER_100MS,
  SHT4X_LOW_HEATER_1S,
  SHT4X_LOW_HEATER_100MS,
} sht4x_heater_t;
/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Manages temperature and humidity measurements using the SHT4X sensor.
 *
 * The `SHT4X` class facilitates interaction with the SHT4X temperature and humidity sensor over an I2C
 * interface. It supports initializing the sensor, configuring measurement precision and heater settings, and
 * retrieving temperature and humidity data. The class stores sensor values internally for easy access.
 *
 * ### Features:
 *
 * - Reads temperature and humidity with configurable precision levels.
 *
 * - Supports built-in heater control for various power and duration settings.
 *
 * - Verifies data integrity with CRC8 checksum validation.
 *
 * - Provides access to the latest temperature and humidity readings.
 *
 * - Allows configuration of measurement precision and heater modes.
 *
 * ### Usage:
 *
 * Instantiate the class and call `begin()` to initialize the sensor. Use `update()` to read temperature and
 * humidity, then retrieve values with `getTemperature()` and `getHumidity()`. Configure precision with
 * `setPrecision()` and heater settings with `setHeater()` as needed.
 *
 * ### Dependencies:
 *
 * - Requires an I2C communication library (`bsp_i2c.h` for `bspI2CExist`, `bspI2CWriteByte`,
 * `bspI2CReadBytes`).
 *
 * - Sensor must be connected to a valid I2C bus at address `SHT40_I2C_ADDR_44` (0x44).
 *
 * - Depends on `utility.h` for `crc8` function and `config.h` for configuration settings.
 */
class SHT4X
{
public:
  /**
   * @brief Initializes the SHT4X sensor.
   *
   * Checks for the presence of the SHT4X sensor on the I2C bus at address 0x44.
   *
   * @param[in] None
   *
   * @attention Ensure the sensor is powered and connected to the I2C bus before calling.
   *
   * @return
   *  - `SHT4X_OK`: Sensor detected and initialized successfully
   *
   *  - `SHT4X_ERR_I2C`: Sensor not found or I2C communication error
   */
  sht4x_error_t begin();

  /**
   * @brief Updates temperature and humidity readings from the SHT4X sensor.
   *
   * Sends a measurement command based on the configured precision and heater settings, waits for the
   * conversion time, reads the data, verifies the checksum, and updates internal temperature and humidity
   * values.
   *
   * @param[in] None
   *
   * @attention Ensure the sensor is initialized with `begin()` and properly connected to the I2C bus.
   *
   * @return
   *  - `SHT4X_OK`: Measurement successful
   *
   *  - `SHT4X_ERR_CHECKSUM`: Checksum mismatch indicating data corruption
   */
  sht4x_error_t update(void);

  /**
   * @brief Sets the measurement precision of the SHT4X sensor.
   *
   * Configures the sensor to use high, medium, or low precision for measurements, affecting accuracy and
   * conversion time.
   *
   * @param[in] prec Desired precision setting (`SHT4X_HIGH_PRECISION`, `SHT4X_MED_PRECISION`, or
   * `SHT4X_LOW_PRECISION`).
   *
   * @attention Changes take effect on the next `update()` call.
   */
  void setPrecision(sht4x_precision_t prec);

  /**
   * @brief Retrieves the current measurement precision setting.
   *
   * Returns the currently configured precision level of the sensor.
   *
   * @param[in] None
   *
   * @return sht4x_precision_t The current precision setting.
   */
  sht4x_precision_t getPrecision(void);

  /**
   * @brief Sets the heater mode of the SHT4X sensor.
   *
   * Configures the built-in heater to one of several power levels and durations, or disables it.
   *
   * @param[in] heat Desired heater mode (e.g., `SHT4X_NO_HEATER`, `SHT4X_HIGH_HEATER_1S`).
   *
   * @attention Heater settings affect power consumption and measurement duration. Changes take effect on the
   * next `update()` call.
   */
  void setHeater(sht4x_heater_t heat);

  /**
   * @brief Retrieves the current heater mode setting.
   *
   * Returns the currently configured heater mode of the sensor.
   *
   * @param[in] None
   *
   * @return sht4x_heater_t The current heater mode setting.
   */
  sht4x_heater_t getHeater(void);

  /**
   * @brief Retrieves the last measured temperature.
   *
   * Returns the most recent temperature reading stored internally, in degrees Celsius.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `update()`.
   *
   * @return float The temperature in degrees Celsius.
   */
  float getTemperature();

  /**
   * @brief Retrieves the last measured humidity.
   *
   * Returns the most recent relative humidity reading stored internally, constrained to 0–100%.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `update()`.
   *
   * @return float The relative humidity percentage (%RH).
   */
  float getHumidity();

private:
  float sensorValue[2] = {0.f}; // Humidity: index 0
                                // Temperature: index 1
  sht4x_precision_t _precision = SHT4X_HIGH_PRECISION;
  sht4x_heater_t    _heater    = SHT4X_NO_HEATER;
};

#endif // SHT40_h

/* End of file -------------------------------------------------------- */
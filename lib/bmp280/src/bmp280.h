/**
 * @file       bmp280.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-03-21
 * @author     Tuan Nguyen
 *
 * @brief      Header file for BMP280 library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BMP280_H
  #define BMP280_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define BMP280_LIB_VERSION (F("0.1.0"))

  #define BMP280_I2C_ADDR    0x76
/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  BMP280_OK = 0,       /* No error */
  BMP280_ERR,          /* Generic error */
  BMP280_ERR_INIT,     /* Initialization error */
  BMP280_ERR_RESET,    /* Reset error */
  BMP280_TIMEOUT,      /* Timeout error*/
  BMP280_ERR_I2C,      /* I2C error */
  BMP280_ERR_DIV_ZERO, /* Divide by 0 error */
  BMP280_ERR_CHECKSUM  /* Checksum error */
} bmp280_error_t;

/** Oversampling rate for the sensor. */
typedef enum
{
  SAMPLING_NONE = 0x00, /** No over-sampling. */
  SAMPLING_X1   = 0x01, /** 1x over-sampling. */
  SAMPLING_X2   = 0x02, /** 2x over-sampling. */
  SAMPLING_X4   = 0x03, /** 4x over-sampling. */
  SAMPLING_X8   = 0x04, /** 8x over-sampling. */
  SAMPLING_X16  = 0x05  /** 16x over-sampling. */
} bmp280_sampling_t;

/** Operating mode for the sensor. */
typedef enum
{
  MODE_SLEEP           = 0x00, /** Sleep mode. */
  MODE_FORCED          = 0x01, /** Forced mode. */
  MODE_NORMAL          = 0x03, /** Normal mode. */
  MODE_SOFT_RESET_CODE = 0xB6  /** Software reset. */
} bmp280_mode_t;

/** Filtering level for sensor data. */
typedef enum
{
  FILTER_OFF = 0x00, /** No filtering. */
  FILTER_X2  = 0x01, /** 2x filtering. */
  FILTER_X4  = 0x02, /** 4x filtering. */
  FILTER_X8  = 0x03, /** 8x filtering. */
  FILTER_X16 = 0x04  /** 16x filtering. */
} bmp280_filter_t;

/** Standby duration in ms */
typedef enum
{
  STANDBY_MS_1    = 0x00, /** 1 ms standby. */
  STANDBY_MS_63   = 0x01, /** 62.5 ms standby. */
  STANDBY_MS_125  = 0x02, /** 125 ms standby. */
  STANDBY_MS_250  = 0x03, /** 250 ms standby. */
  STANDBY_MS_500  = 0x04, /** 500 ms standby. */
  STANDBY_MS_1000 = 0x05, /** 1000 ms standby. */
  STANDBY_MS_2000 = 0x06, /** 2000 ms standby. */
  STANDBY_MS_4000 = 0x07  /** 4000 ms standby. */
} bmp280_standby_duration_t;

enum
{
  BMP280_REGISTER_DIG_T1       = 0x88,
  BMP280_REGISTER_DIG_T2       = 0x8A,
  BMP280_REGISTER_DIG_T3       = 0x8C,
  BMP280_REGISTER_DIG_P1       = 0x8E,
  BMP280_REGISTER_DIG_P2       = 0x90,
  BMP280_REGISTER_DIG_P3       = 0x92,
  BMP280_REGISTER_DIG_P4       = 0x94,
  BMP280_REGISTER_DIG_P5       = 0x96,
  BMP280_REGISTER_DIG_P6       = 0x98,
  BMP280_REGISTER_DIG_P7       = 0x9A,
  BMP280_REGISTER_DIG_P8       = 0x9C,
  BMP280_REGISTER_DIG_P9       = 0x9E,
  BMP280_REGISTER_CHIPID       = 0xD0,
  BMP280_REGISTER_VERSION      = 0xD1,
  BMP280_REGISTER_SOFTRESET    = 0xE0,
  BMP280_REGISTER_CAL26        = 0xE1, /**< R calibration = 0xE1-0xF0 */
  BMP280_REGISTER_STATUS       = 0xF3,
  BMP280_REGISTER_CONTROL      = 0xF4,
  BMP280_REGISTER_CONFIG       = 0xF5,
  BMP280_REGISTER_PRESSUREDATA = 0xF7,
  BMP280_REGISTER_TEMPDATA     = 0xFA,
};

typedef struct
{
  uint16_t dig_T1; /**< dig_T1 cal register. */
  int16_t  dig_T2; /**<  dig_T2 cal register. */
  int16_t  dig_T3; /**< dig_T3 cal register. */
  uint16_t dig_P1; /**< dig_P1 cal register. */
  int16_t  dig_P2; /**< dig_P2 cal register. */
  int16_t  dig_P3; /**< dig_P3 cal register. */
  int16_t  dig_P4; /**< dig_P4 cal register. */
  int16_t  dig_P5; /**< dig_P5 cal register. */
  int16_t  dig_P6; /**< dig_P6 cal register. */
  int16_t  dig_P7; /**< dig_P7 cal register. */
  int16_t  dig_P8; /**< dig_P8 cal register. */
  int16_t  dig_P9; /**< dig_P9 cal register. */
} bmp280_calib_data;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Manages environmental measurements using the BMP280 sensor, providing access to pressure,
 * temperature, and altitude data.
 *
 * The `BMP280` class facilitates interaction with the BMP280 environmental sensor over I2C. It supports
 * reading pressure, temperature, and altitude, with calibration and configuration options for sampling,
 * filtering, and operating modes. The class stores converted sensor values for easy access and provides
 * utility functions for sea level pressure and water boiling point calculations.
 *
 * ### Features:
 *
 * - Reads pressure, temperature, and altitude with calibration adjustments.
 *
 * - Supports configurable oversampling, filtering, and standby durations.
 *
 * - Provides forced measurement mode for precise control.
 *
 * - Calculates sea level pressure and water boiling point based on measurements.
 *
 * - Stores sensor data in an internal array for retrieval via getter methods.
 *
 * ### Usage:
 *
 * Instantiate the class and call `begin()` to initialize the sensor. Use `update()` to read all measurements
 * or individual methods like `readPressure()` for specific data. Getter methods (e.g., `getPressure()`)
 * provide access to the latest values. Configure sampling settings with `setSampling()` for desired
 * performance.
 *
 * ### Dependencies:
 *
 * - Requires I2C communication library (`bspI2C` functions).
 *
 * - Sensor must be connected to a valid I2C bus at address `BMP280_I2C_ADDR` (0x76).
 *
 * - Calibration coefficients must be read successfully during initialization.
 */
class BMP280
{
public:
  /**
   * @brief Initializes the BMP280 sensor.
   *
   * Verifies I2C connectivity, reads calibration coefficients, and sets default sampling parameters.
   *
   * @param[in] None
   *
   * @attention Ensure the sensor is powered and connected to the I2C bus before calling.
   *
   * @return
   *  - `BMP280_OK`: Success
   *
   *  - `BMP280_ERR_I2C`: I2C communication failure
   */
  bmp280_error_t begin();

  /**
   * @brief Updates all sensor measurements.
   *
   * Reads temperature, pressure, and altitude in sequence, storing results in the internal sensor array.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return
   *  - `BMP280_OK`: Success
   */
  bmp280_error_t update();

  /**
   * @brief Resets the BMP280 sensor.
   *
   * Sends a soft reset command to the sensor, restoring it to its power-on state.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`. May disrupt ongoing measurements.
   *
   * @return
   *  - `BMP280_OK`: Success
   */
  bmp280_error_t reset(void);

  /**
   * @brief Reads the current status of the BMP280 sensor.
   *
   * Retrieves the sensor's status register to check measurement and image update status.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint8_t The status register value.
   */
  uint8_t getStatus(void);

  /**
   * @brief Reads the pressure from the BMP280 sensor.
   *
   * Retrieves raw pressure data, applies calibration coefficients, and stores the result in the internal
   * sensor array.
   *
   * @param[in] None
   *
   * @attention Requires temperature to be read first to set the `t_fine` variable. Ensure successful
   * initialization via `begin()`.
   *
   * @return
   *  - `BMP280_OK`: Success
   *
   *  - `BMP280_ERR_DIV_ZERO`: Division by zero error during calculation
   */
  bmp280_error_t readPressure();

  /**
   * @brief Reads the temperature from the BMP280 sensor.
   *
   * Retrieves raw temperature data, applies calibration coefficients, and stores the result in the internal
   * sensor array.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return
   *  - `BMP280_OK`: Success
   */
  bmp280_error_t readTemperature();

  /**
   * @brief Calculates the altitude based on pressure data.
   *
   * Uses the current pressure and sea level pressure to compute altitude, storing the result in the internal
   * sensor array.
   *
   * @param[in] None
   *
   * @attention Requires a prior call to `readPressure()` and valid sea level pressure.
   *
   * @return
   *  - `BMP280_OK`: Success
   */
  bmp280_error_t readAltitude();

  /**
   * @brief Retrieves the pressure value.
   *
   * Returns the previously read pressure value stored in the internal sensor array, in Pascals.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readPressure()` or `update()`.
   *
   * @return float The pressure value in Pascals.
   */
  float getPressure();

  /**
   * @brief Retrieves the temperature value.
   *
   * Returns the previously read temperature value stored in the internal sensor array, in degrees Celsius.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readTemperature()` or `update()`.
   *
   * @return float The temperature value in °C.
   */
  float getTemperature();

  /**
   * @brief Retrieves the altitude value.
   *
   * Returns the previously calculated altitude value stored in the internal sensor array, in meters.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readAltitude()` or `update()`.
   *
   * @return float The altitude value in meters.
   */
  float getAltitude();

  /**
   * @brief Calculates sea level pressure for a given altitude and atmospheric pressure.
   *
   * Computes the equivalent sea level pressure based on the provided altitude and measured atmospheric
   * pressure.
   *
   * @param[in] altitude The altitude in meters.
   * @param[in] atmospheric The measured atmospheric pressure in Pascals.
   *
   * @attention The provided values should be valid and within reasonable ranges.
   *
   * @return float The calculated sea level pressure in hPa.
   */
  float seaLevelForAltitude(float altitude, float atmospheric);

  /**
   * @brief Sets the sea level pressure for altitude calculations.
   *
   * Updates the internal sea level pressure value used for altitude computations.
   *
   * @param[in] pressure The sea level pressure in hPa.
   *
   * @attention The provided pressure should be a valid value (typically around 1013.25 hPa).
   */
  void setSeaLevelPressure(float pressure);

  /**
   * @brief Calculates the water boiling point based on pressure.
   *
   * Computes the boiling point of water at the given atmospheric pressure using a logarithmic formula.
   *
   * @param[in] pressure The atmospheric pressure in hPa.
   *
   * @attention The pressure value should be valid and positive to avoid mathematical errors.
   *
   * @return float The boiling point temperature in °C.
   */
  float waterBoilingPoint(float pressure);

  /**
   * @brief Triggers a forced measurement.
   *
   * Initiates a single measurement in forced mode and waits for completion.
   *
   * @param[in] None
   *
   * @attention Only works if the sensor is in forced mode (`MODE_FORCED`). Requires successful initialization
   * via `begin()`.
   *
   * @return
   *  - `true`: Measurement completed successfully
   *
   *  - `false`: Sensor not in forced mode
   */
  bool takeForcedMeasurement();

  /**
   * @brief Configures the sensor’s sampling and operating parameters.
   *
   * Sets the operating mode, temperature and pressure oversampling, filter settings, and standby duration.
   *
   * @param[in] mode Operating mode (default: `MODE_NORMAL`).
   * @param[in] tempSampling Temperature oversampling (default: `SAMPLING_X16`).
   * @param[in] pressSampling Pressure oversampling (default: `SAMPLING_X16`).
   * @param[in] filter Filter setting (default: `FILTER_OFF`).
   * @param[in] duration Standby duration in normal mode (default: `STANDBY_MS_1`).
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return
   *  - `BMP280_OK`: Success
   */
  bmp280_error_t setSampling(bmp280_mode_t mode = MODE_NORMAL, bmp280_sampling_t tempSampling = SAMPLING_X16,
                             bmp280_sampling_t         pressSampling = SAMPLING_X16,
                             bmp280_filter_t           filter        = FILTER_OFF,
                             bmp280_standby_duration_t duration      = STANDBY_MS_1);

private:
  float sensorValue[3] = {0.f}; // Index 0: Pressure
                                // Index 1: Temperature
                                // Index 2: Altitude

  float _seaLevelhPa = 1013.25f;

  /** Encapsulates the config register */
  struct config
  {
    /** Initialize to power-on-reset state */
    config() : t_sb(STANDBY_MS_1), filter(FILTER_OFF), none(0), spi3w_en(0) {}
    /** Inactive duration (standby time) in normal mode */
    unsigned int t_sb     : 3;
    /** Filter settings */
    unsigned int filter   : 3;
    /** Unused - don't set */
    unsigned int none     : 1;
    /** Enables 3-wire SPI */
    unsigned int spi3w_en : 1;
    /** Used to retrieve the assembled config register's byte value. */
    unsigned int get() { return (t_sb << 5) | (filter << 2) | spi3w_en; }
  };

  /** Encapsulates trhe ctrl_meas register */
  struct ctrl_meas
  {
    /** Initialize to power-on-reset state */
    ctrl_meas() : osrs_t(SAMPLING_NONE), osrs_p(SAMPLING_NONE), mode(MODE_SLEEP) {}
    /** Temperature oversampling. */
    unsigned int osrs_t : 3;
    /** Pressure oversampling. */
    unsigned int osrs_p : 3;
    /** Device mode */
    unsigned int mode   : 2;
    /** Used to retrieve the assembled ctrl_meas register's byte value. */
    unsigned int get() { return (osrs_t << 5) | (osrs_p << 2) | mode; }
  };

  bmp280_error_t readCoefficients(void);
  uint16_t       read16(byte reg);
  uint32_t       read24(byte reg);
  int16_t        readS16(byte reg);
  uint16_t       read16_LE(byte reg);
  int16_t        readS16_LE(byte reg);

  int32_t           t_fine;
  bmp280_calib_data _bmp280_calib;
  config            _configReg;
  ctrl_meas         _measReg;
};

#endif // BMP280_H

/* End of file -------------------------------------------------------- */
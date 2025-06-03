/**
 * @file       sht4x.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for BMP280 library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "bmp280.h"
#include "bsp_i2c.h"
#include "config.h"
#include "utility.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions ------------------------------------------- */
bmp280_error_t BMP280::begin()
{
  if (!bspI2CExist(BMP280_I2C_ADDR))
  {
    return BMP280_ERR_I2C;
  }
  readCoefficients();
  setSampling();

  return BMP280_OK;
}

bmp280_error_t BMP280::update()
{
  readTemperature();
  readPressure();
  readAltitude();
  return BMP280_OK;
}

bmp280_error_t BMP280::reset(void)
{
  bspI2CWriteByte(BMP280_I2C_ADDR, BMP280_REGISTER_SOFTRESET, MODE_SOFT_RESET_CODE);
  return BMP280_OK;
}

uint8_t BMP280::getStatus(void)
{
  uint8_t byte = 0;
  bspI2CReadByte(BMP280_I2C_ADDR, BMP280_REGISTER_STATUS, byte);
  return byte;
}

bmp280_error_t BMP280::readPressure()
{
  int64_t var1, var2, p;

  // Must be done first to get the t_fine variable set up

  int32_t adc_P = read24(BMP280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  var1 = ((int64_t) t_fine) - 128000;
  var2 = var1 * var1 * (int64_t) _bmp280_calib.dig_P6;
  var2 = var2 + ((var1 * (int64_t) _bmp280_calib.dig_P5) << 17);
  var2 = var2 + (((int64_t) _bmp280_calib.dig_P4) << 35);
  var1 =
  ((var1 * var1 * (int64_t) _bmp280_calib.dig_P3) >> 8) + ((var1 * (int64_t) _bmp280_calib.dig_P2) << 12);
  var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) _bmp280_calib.dig_P1) >> 33;

  if (var1 == 0)
  {
    return BMP280_ERR_DIV_ZERO; // avoid exception caused by division by zero
  }
  p    = 1048576 - adc_P;
  p    = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t) _bmp280_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t) _bmp280_calib.dig_P8) * p) >> 19;

  p              = ((p + var1 + var2) >> 8) + (((int64_t) _bmp280_calib.dig_P7) << 4);
  sensorValue[0] = p / 256;

  return BMP280_OK;
}

bmp280_error_t BMP280::readTemperature()
{
  int32_t var1, var2;

  int32_t adc_T = read24(BMP280_REGISTER_TEMPDATA);
  adc_T >>= 4;

  var1 = ((((adc_T >> 3) - ((int32_t) _bmp280_calib.dig_T1 << 1))) * ((int32_t) _bmp280_calib.dig_T2)) >> 11;

  var2 =
  (((((adc_T >> 4) - ((int32_t) _bmp280_calib.dig_T1)) * ((adc_T >> 4) - ((int32_t) _bmp280_calib.dig_T1))) >>
    12) *
   ((int32_t) _bmp280_calib.dig_T3)) >>
  14;

  t_fine = var1 + var2;

  float T        = (t_fine * 5 + 128) >> 8;
  sensorValue[1] = T / 100;
  return BMP280_OK;
}

bmp280_error_t BMP280::readAltitude()
{
  float pressure = getPressure(); // in SI units for Pascal
  pressure /= 100;
  sensorValue[2] = 44330 * (1.0 - pow(pressure / _seaLevelhPa, 0.1903));
  return BMP280_OK;
}

float BMP280::getPressure() { return sensorValue[0]; }

float BMP280::getTemperature() { return sensorValue[1]; }

float BMP280::getAltitude() { return sensorValue[2]; }

float BMP280::seaLevelForAltitude(float altitude, float atmospheric)
{
  return atmospheric / pow(1.0 - (altitude / 44330.0), 5.255);
}

void BMP280::setSeaLevelPressure(float pressure) { _seaLevelhPa = pressure; }

float BMP280::waterBoilingPoint(float pressure)
{
  return (234.175 * log(pressure / 6.1078)) / (17.08085 - log(pressure / 6.1078));
}

bool BMP280::takeForcedMeasurement()
{
  if (_measReg.mode == MODE_FORCED)
  {
    bspI2CWriteByte(BMP280_I2C_ADDR, BMP280_REGISTER_CONTROL, _measReg.get());

    while (getStatus() & 0x08)
    {
      DELAY(1);
    }
    return true;
  }
  return false;
}

bmp280_error_t BMP280::setSampling(bmp280_mode_t mode, bmp280_sampling_t tempSampling,
                                   bmp280_sampling_t pressSampling, bmp280_filter_t filter,
                                   bmp280_standby_duration_t duration)
{
  _measReg.mode   = mode;
  _measReg.osrs_p = pressSampling;
  _measReg.osrs_t = tempSampling;

  _configReg.filter = filter;
  _configReg.t_sb   = duration;

  bspI2CWriteByte(BMP280_I2C_ADDR, BMP280_REGISTER_CONTROL, _measReg.get());
  bspI2CWriteByte(BMP280_I2C_ADDR, BMP280_REGISTER_CONFIG, _configReg.get());

  return BMP280_OK;
}

bmp280_error_t BMP280::readCoefficients()
{
  _bmp280_calib.dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
  _bmp280_calib.dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
  _bmp280_calib.dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

  _bmp280_calib.dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
  _bmp280_calib.dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
  _bmp280_calib.dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
  _bmp280_calib.dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
  _bmp280_calib.dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
  _bmp280_calib.dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
  _bmp280_calib.dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
  _bmp280_calib.dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
  _bmp280_calib.dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);

  return BMP280_OK;
}
/**************************************************************************/
/*!
 *  @brief  Reads a 16 bit value over I2C/SPI
 */
uint16_t BMP280::read16(byte reg)
{
  uint8_t buffer[2];
  bspI2CReadBytes(BMP280_I2C_ADDR, reg, buffer, 2);
  return uint16_t(buffer[0]) << 8 | uint16_t(buffer[1]);
}

uint16_t BMP280::read16_LE(byte reg)
{
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);
}

/*!
 *   @brief  Reads a signed 16 bit value over I2C/SPI
 */
int16_t BMP280::readS16(byte reg) { return (int16_t) read16(reg); }

int16_t BMP280::readS16_LE(byte reg) { return (int16_t) read16_LE(reg); }

/*!
 *  @brief  Reads a 24 bit value over I2C/SPI
 */
uint32_t BMP280::read24(byte reg)
{
  uint8_t buffer[3];
  bspI2CReadBytes(BMP280_I2C_ADDR, reg, buffer, 3);
  return uint32_t(buffer[0]) << 16 | uint32_t(buffer[1]) << 8 | uint32_t(buffer[2]);
}
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */

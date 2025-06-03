/**
 * @file       bsp_i2c.h
 * @license    This project is released under the MIT License.
 * @version    0.1.0
 * @date       2024-12-31
 * @author     Tuan Nguyen
 *
 * @brief      Header file for bsp_i2c
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_I2C_H
  #define BSP_I2C_H

  /* Includes --------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

// Error codes for I2C
typedef enum
{
  BSP_I2C_OK = 0,
  BSP_I2C_ERR,
  BSP_I2C_ERR_READ,
  BSP_I2C_ERR_WRITE,
  BSP_I2C_TIMEOUT
} bsp_i2c_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
bsp_i2c_error_t bspI2CBegin();
bsp_i2c_error_t bspI2CBegin(uint8_t address);

// beginTransmission
bsp_i2c_error_t bspI2CBeginTransmission(uint16_t address);

bsp_i2c_error_t bspI2CBeginTransmission(uint8_t address);

bsp_i2c_error_t bspI2CBeginTransmission(int address);

// endTransmission
uint8_t bspI2CEndTransmission(bool sendStop);

uint8_t bspI2CEndTransmission(void);

// requestFrom
size_t bspI2CRequestFrom(uint16_t address, size_t size, bool sendStop);

uint8_t bspI2CRequestFrom(uint16_t address, uint8_t size, bool sendStop);

uint8_t bspI2CRequestFrom(uint16_t address, uint8_t size, uint8_t sendStop);

size_t bspI2CRequestFrom(uint8_t address, size_t len, bool stopBit);

uint8_t bspI2CRequestFrom(uint16_t address, uint8_t size);

uint8_t bspI2CRequestFrom(uint8_t address, uint8_t size, uint8_t sendStop);

uint8_t bspI2CRequestFrom(uint8_t address, uint8_t size);

uint8_t bspI2CRequestFrom(int address, int size, int sendStop);

uint8_t bspI2CRequestFrom(int address, int size);

// write
size_t bspI2CWrite(uint8_t data);

size_t bspI2CWrite(const uint8_t *data, size_t quantity);

int bspI2CAvailable(void);

int bspI2CRead(void);

int bspI2CPeek(void);

bsp_i2c_error_t bspI2CFlush(void);

/**
 * @brief  Reads a single byte using I2C from a register.
 *
 * This function read a single byte over the I2C interface from a register
 * and stores it in the provided reference variable.
 *
 * @param[in]     address Device's I2C address
 * @param[in]     reg     The register to read from
 * @param[out]    byte    Reference to a variable where the read byte will be stored.
 *
 * @attention  Ensure that the device is correctly connected and that I2C communication is functioning.
 *             The function includes a loop with retries and delays to handle cases where the byte is not
 * immediately available.
 *
 * @return
 *  - `BSP_I2C_OK`      : Success
 *  - `BSP_I2C_ERR_READ`: Error reading from the I2C bus.
 */
bsp_i2c_error_t bspI2CReadByte(int address, uint8_t reg, uint8_t &byte);

/**
 * @brief  Reads a single byte using I2C
 *
 * This function request a single byte over the I2C interface
 * and stores it in the provided reference variable.
 *
 * @param[in]     address Device's I2C address
 * @param[out]    byte    Reference to a variable where the read byte will be stored.
 *
 * @attention  Ensure that the device is correctly connected and that I2C communication is functioning.
 *             The function includes a loop with retries and delays to handle cases where the byte is not
 * immediately available.
 *
 * @return
 *  - `BSP_I2C_OK`      : Success
 *  - `BSP_I2C_ERR_READ`: Error reading from the I2C bus.
 */
bsp_i2c_error_t bspI2CReadByte(int address, uint8_t &byte);

/**
 * @brief  Reads multiple bytes using I2C from a register.
 *
 * This function requests multiple bytes over the I2C interface from a register
 * and stores them in the provided buffer.
 *
 * @param[in]     address Device's I2C address
 * @param[in]     reg     The register to read from
 * @param[out]    bytes   Pointer to a buffer where the read bytes will be stored.
 * @param[in]     len     The number of bytes to read.
 *
 * @attention  Ensure that the sensor is correctly connected and that I2C communication is functioning.
 *             The function includes a loop with retries and delays to handle cases where the bytes are not
 * immediately available.
 *
 * @return
 *  - `BSP_I2C_OK`: Success
 *  - `BSP_I2C_ERR_READ`: Error reading from the I2C bus.
 */
bsp_i2c_error_t bspI2CReadBytes(int address, uint8_t reg, uint8_t *bytes, uint32_t len);

/**
 * @brief  Reads multiple bytes using I2C.
 *
 * This function requests multiple bytes over the I2C interface
 * and stores them in the provided buffer.
 *
 * @param[in]     address Device's I2C address
 * @param[out]    bytes   Pointer to a buffer where the read bytes will be stored.
 * @param[in]     len     The number of bytes to read.
 *
 * @attention  Ensure that the sensor is correctly connected and that I2C communication is functioning.
 *             The function includes a loop with retries and delays to handle cases where the bytes are not
 * immediately available.
 *
 * @return
 *  - `BSP_I2C_OK`: Success
 *  - `BSP_I2C_ERR_READ`: Error reading from the I2C bus.
 */
bsp_i2c_error_t bspI2CReadBytes(int address, uint8_t *bytes, uint32_t len);

/**
 * @brief  Writes a single byte to a register using I2C.
 *
 * This function writes a single byte to a register over the I2C interface.
 *
 * @param[in]     address Device's I2C address
 * @param[in]     reg     The register to read from
 * @param[in]     byte    The byte to write.
 *
 * @attention  Ensure that the sensor is correctly connected and that I2C communication is functioning.
 *
 * @return
 *  - `BSP_I2C_OK`: Success
 *  - `BSP_I2C_ERR_WRITE`: Error writing to the I2C bus.
 */
bsp_i2c_error_t bspI2CWriteByte(int address, uint8_t reg, uint8_t byte);

/**
 * @brief  Writes multiple bytes to a register using I2C.
 *
 * This function writes multiple bytes to a register over the I2C interface.
 *
 * @param[in]     address Device's I2C address
 * @param[in]     reg     The register to read from
 * @param[in]     bytes   Pointer to a buffer containing the bytes to write.
 * @param[in]     len     The number of bytes to write.
 *
 * @attention  Ensure that the sensor is correctly connected and that I2C communication is functioning.
 *
 * @return
 *  - `BSP_I2C_OK`: Success
 *  - `BSP_I2C_ERR_WRITE`: Error writing to the I2C bus.
 */
bsp_i2c_error_t bspI2CWriteBytes(int address, uint8_t reg, uint8_t *bytes, uint32_t len);

/**
 * @brief  Checks if an I2C device is present on the bus.
 *
 * This function verifies the presence of an I2C device by initiating a transmission
 * and checking for an acknowledgment.
 *
 * @param[in]     address Device's I2C address.
 *
 * @attention  Ensure that the I2C bus is initialized before calling this function.
 *
 * @return
 *  - `true`:   Device is present.
 *
 *  - `false`:  Device is not responding.
 */
bool bspI2CExist(uint8_t address);

#endif /* BSP_I2C_H */

/* End of file -------------------------------------------------------- */

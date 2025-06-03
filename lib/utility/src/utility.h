/**
 * @file       utility.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header file for Utility library. This library provides various utility functions for multiple
 * purposes.
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef UTILITY_H
  #define UTILITY_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  #include "Wire.h"

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief Scans the I2C bus for connected devices.
 *
 * The `scanI2CDevices` function probes the I2C bus for devices by attempting communication with addresses
 * from 1 to 126. It reports detected devices and errors via the Serial interface, providing a summary of the
 * scan results. The function is designed for debugging and identifying I2C device addresses on an
 * Arduino-compatible board.
 *
 * ### Features:
 *
 * - Scans I2C addresses 1 through 126 for responsive devices.
 *
 * - Reports detected devices with their hexadecimal addresses.
 *
 * - Identifies unknown errors during communication attempts.
 *
 * - Summarizes the number of devices found or indicates if none are detected.
 *
 * - Includes a delay to allow periodic scanning without overwhelming the Serial output.
 *
 * ### Usage:
 *
 * Call `scanI2CDevices()` to initiate an I2C bus scan. Ensure the Serial interface is initialized (e.g., with
 * `Serial.begin(9600)`) and the I2C bus is configured (e.g., with `Wire.begin()`) before calling. The
 * function outputs results to the Serial monitor and pauses for 2 seconds before completing, suitable for
 * repeated calls in a loop.
 *
 * ### Dependencies:
 *
 * - Requires an Arduino-compatible board with I2C support.
 *
 * - Depends on the `Wire` library for I2C communication (`Wire.beginTransmission`, `Wire.endTransmission`).
 *
 * - Requires the `Serial` interface for output (`Serial.print`, `Serial.println`).
 */
void scanI2CDevices(void);

/**
 * @brief  Calculates CRC-8 checksum for the given data using SHT40's polynomial.
 *
 * This function computes an 8-bit CRC checksum using the polynomial defined in the SHT40
 * datasheet. It is used to verify data integrity when reading from the SHT40 sensor.
 *
 * The CRC-8 parameters are:
 *   - Initialization value: 0xFF
 *   - Polynomial: 0x31 (x⁸ + x⁵ + x⁴ + 1)
 *   - No final XOR (0x00)
 *
 * Example from datasheet: For data bytes {0xBE, 0xEF}, the CRC result is 0x92.
 *
 * @param[in]     data  Pointer to the input data buffer.
 * @param[in]     len   Length of the data buffer in bytes.
 *
 * @attention  Ensure that `data` points to a valid memory location of at least `len` bytes.
 *             This function does not perform bounds checking.
 *
 * @return
 *  - CRC-8 checksum as an 8-bit unsigned integer.
 */
uint8_t crc8(const uint8_t *data, int len);

/**
 * @brief  Compares two firmware version strings.
 *
 * This function compares two version strings (e.g., "1.0", "0.9.3", "2") by parsing up to three levels
 * of semantic versioning: major, minor, and patch. Missing segments are treated as zero, allowing
 * flexible comparison of version formats like "0.1", "0.1.0", or "1".
 *
 * @param[in]     v1  The current firmware version (as a `String`).
 * @param[in]     v2  The incoming/target firmware version (as a `String`).
 *
 * @attention  Version strings should be in numeric dot-separated format. Non-numeric parts are ignored.
 *             For example, "1.0a" would parse as "1.0.0". Only the first three segments are considered.
 *
 * @return
 *  - `-1` if `v1` is lower than `v2`
 *  - ` 0` if both versions are equal
 *  - ` 1` if `v1` is greater than `v2`
 */
int compareVersion(String v1, String v2);

#endif // UTILITY_H

/* End of file -------------------------------------------------------- */
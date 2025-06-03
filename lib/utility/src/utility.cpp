/**
 * @file       utility.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for Utility library. This library provides various utility functions for multiple
 * purposes.
 *
 */

/* Includes ----------------------------------------------------------- */
#include "utility.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void scanI2CDevices()
{
  byte error, address;
  int  nDevices = 0;

  Serial.println("Scanning for I2C devices...");

  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
  {
    Serial.println("No I2C devices found\n");
  }
  else
  {
    Serial.println("Scan complete\n");
  }

  delay(2000); // wait 2 seconds for next scan
}

uint8_t crc8(const uint8_t *data, int len)
{
  /*
   *
   * CRC-8 formula from page 9/17 of SHT40 datasheet
   *
   * Test data 0xBE, 0xEF should yield 0x92
   *
   * Initialization data 0xFF
   * Polynomial 0x31 (x8 + x5 +x4 +1)
   * Final XOR 0x00
   */

  const uint8_t POLYNOMIAL(0x31);
  uint8_t       crc(0xFF);

  for (int j = len; j; --j)
  {
    crc ^= *data++;

    for (int i = 8; i; --i)
    {
      crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
    }
  }
  return crc;
}

int compareVersion(String v1, String v2)
{
  int parts1[3] = {0}, parts2[3] = {0};

  // Helper to parse a version string into 3 int parts
  auto parseVersion = [](const String &ver, int *parts) {
    int partIndex = 0;
    int lastDot   = -1;
    for (int i = 0; i <= ver.length(); ++i)
    {
      if (ver[i] == '.' || i == ver.length())
      {
        if (partIndex < 3)
        {
          parts[partIndex++] = ver.substring(lastDot + 1, i).toInt();
        }
        lastDot = i;
      }
    }
  };

  parseVersion(v1, parts1);
  parseVersion(v2, parts2);

  // Compare part by part
  for (int i = 0; i < 3; i++)
  {
    if (parts1[i] < parts2[i])
      return -1; // v1 < v2
    if (parts1[i] > parts2[i])
      return 1; // v1 > v2
  }

  return 0; // equal
}

/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */

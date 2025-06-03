#include "globals.h"

void setup()
{
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN, 100000UL);

#ifdef DEBUG_I2C
  while (!Serial)
    ; // Wait for Serial to be ready
  scanI2CDevices();
#endif // DEBUG_I2C

// WiFi Setup
#ifdef WIFI_MODULE
  wifiSetup();
#endif // WIFI_MODULE

// Devices Setup
#ifdef UNIT_ENV_IV_MODULE
  unitENVIVSetup();
#endif // UNIT_ENV_IV_MODULE

#ifdef LIGHT_SENSOR_MODULE
  lightSensorSetup();
#endif // LIGHT_SENSOR_MODULE

#ifdef LCD_MODULE
  lcdSetup();
#endif // LCD_MODULE

#ifdef LED_RGB_MODULE
  ledRgbSetup();
#endif // LED_RGB_MODULE

#ifdef SERVO_MODULE
  doorSetup();
#endif // SERVO_MODULE

#ifdef MINI_FAN_MODULE
#endif // MINI_FAN_MODULE

#ifdef BUTTON_MODULE
  buttonSetup();
#endif // BUTTON_MODULE

// IOT Server Setup
#ifdef IOT_SERVER_MODULE
  iotServerSetup();
#endif // IOT_SERVER_MODULE
}

void loop()
{
#ifdef DEBUG_I2C
  scanI2CDevices();
#endif
}

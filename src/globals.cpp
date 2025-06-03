#include "globals.h"

#ifdef SHT4X_MODULE
SHT4X sht40;
#endif

#ifdef BMP280_MODULE
BMP280 bmp280;
#endif

#ifdef LIGHT_SENSOR_MODULE
LightSensor lightSensor(LIGHT_SENSOR_PIN);
#endif

#ifdef LCD_MODULE
LCD_I2C lcd(0x21, 16, 2);
#endif

#ifdef MINI_FAN_MODULE
MiniFan miniFan(MINI_FAN_PIN);
#endif

#ifdef SERVO_MODULE
Servo doorServo;
#endif

#ifdef LED_RGB_MODULE
Adafruit_NeoPixel rgb(4, LED_RGB_PIN, NEO_GRB + NEO_KHZ800);
#endif

#ifdef BUTTON_MODULE
ButtonHandler button(BUTTON_PIN, false, true);
#endif

bool wifiConnected = false;

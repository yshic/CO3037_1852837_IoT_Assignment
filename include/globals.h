/**
 * @file       globals.h
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Header file for global
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef GLOBALS_H
  #define GLOBALS_H

/* Private defines ---------------------------------------------------- */
// DEBUGGING
// #define DEBUG_I2C

  #ifndef DEBUG_PRINT
    #define DEBUG_PRINT
  #endif // DEBUG_PRINT

// Choose Development Boards
  #define YOLO_UNO
//  #define XIAO_ESP32S3

//  #define DEBUG_PRINT_RTOS_TIMING
// Communication
/* Wireless   --------------------------------------------------------- */
  #ifdef ESP32
    #define WIFI_MODULE
    #define IOT_SERVER_MODULE
    #define OTA_UPDATE_MODULE
  #endif

// Peripherals

// Sensors
  #define UNIT_ENV_IV_MODULE
// #define SHT4X_MODULE
// #define BMP280_MODULE
  #define LIGHT_SENSOR_MODULE

// Display
  #define LCD_MODULE
  #define LED_RGB_MODULE

// MOTORS / ACTUATORS
  #define MINI_FAN_MODULE
  #define SERVO_MODULE
  #define BUTTON_MODULE

  #ifdef UNIT_ENV_IV_MODULE
    #ifndef SHT4X_MODULE
      #define SHT4X_MODULE
    #endif

    #ifndef BMP280_MODULE
      #define BMP280_MODULE
    #endif
  #endif // UNIT_ENV_IV_MODULE

  // Define Pin
  #ifdef YOLO_UNO
    #define SDA_PIN          GPIO_NUM_11
    #define SCL_PIN          GPIO_NUM_12

    #define BUTTON_PIN       GPIO_NUM_6
    #define LED_RGB_PIN      GPIO_NUM_4
    #define MINI_FAN_PIN     GPIO_NUM_3
    #define SERVO_PIN        GPIO_NUM_2
    #define LIGHT_SENSOR_PIN GPIO_NUM_1
  #endif

  #ifdef XIAO_ESP32S3
    #define SDA_PIN          5
    #define SCL_PIN          6

    #define BUTTON_PIN       D2
    #define LED_RGB_PIN      D0
    #define MINI_FAN_PIN     A8
    #define SERVO_PIN        A1
    #define LIGHT_SENSOR_PIN A9
  #endif

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  // Display
  #ifdef LCD_MODULE
    #include "lcd_16x2.h"
extern LCD_I2C lcd;
  #endif // LCD_MODULE

  #ifdef LED_RGB_MODULE
    #include <Adafruit_NeoPixel.h>
extern Adafruit_NeoPixel rgb;
  #endif

  #ifdef SHT4X_MODULE
    #include "sht4x.h"
extern SHT4X sht40;
  #endif // SHT4X_MODULE

  #ifdef BMP280_MODULE
    #include "bmp280.h"
extern BMP280 bmp280;
  #endif // BMP280_MODULE

  #ifdef LIGHT_SENSOR_MODULE
    #include "light_sensor.h"
extern LightSensor lightSensor;
  #endif // LIGHT_SENSOR_MODULE

  // Motor
  #ifdef MINI_FAN_MODULE
    #include "mini_fan.h"
extern MiniFan miniFan;
  #endif // MINI_FAN_MODULE

  #ifdef SERVO_MODULE
    #include "ESP32Servo.h"
extern Servo doorServo;
  #endif

  // Misc
  #include "bsp_gpio.h"
  #include "bsp_i2c.h"
  #include "bsp_uart.h"

  #ifdef BUTTON_MODULE
    #include "button.h"
extern ButtonHandler button;
  #endif // BUTTON_MODULE

  #include "secrets.h"
  #include "utility.h"

// RTOS
  #include "../src/tasks/actuators_task.h"
  #include "../src/tasks/button_task.h"
  #include "../src/tasks/iot_server_task.h"
  #include "../src/tasks/lcd_task.h"
  #include "../src/tasks/sensors_task.h"
  #include "../src/tasks/wifi_task.h"

extern bool wifiConnected;

#endif // GLOBALS_H

/* End of file -------------------------------------------------------- */
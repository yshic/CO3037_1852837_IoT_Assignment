/**
 * @file       lcd_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for LCD Task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "lcd_task.h"
#include "globals.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Task definitions ------------------------------------------- */
#ifdef LCD_MODULE
void lcdTask(void *pvParameters)
{
  for (;;)
  {
    if (wifiConnected)
    {
      switch (lcd.getScreenState())
      {
  #ifdef SERVO_MODULE
        case LCD_SCREEN_DOOR:
          lcd.clear();
          lcd.print("Door Status: ");
          lcd.setCursor(0, 1);
          if (doorServo.getDoorStatus())
          {
            lcd.print("Opened");
          }
          else
          {
            lcd.print("Closed");
          }
          break;
  #endif // SERVO_MODULE

  #ifdef SHT4X_MODULE
        case LCD_SCREEN_SHT4X:
          lcd.clear();
          lcd.print("Hum: ");
          lcd.print(sht40.getHumidity());
          lcd.print(" %");
          lcd.setCursor(0, 1);
          lcd.print("Temp: ");
          lcd.print(sht40.getTemperature());
          lcd.print(" *C");
          break;
  #endif

  #ifdef BMP280_MODULE
        case LCD_SCREEN_BMP280:
          lcd.clear();
          lcd.print("Pres.: ");
          lcd.print(bmp280.getPressure());
          lcd.print(" atm");
          lcd.setCursor(0, 1);
          lcd.print("Alt.: ");
          lcd.print(bmp280.getAltitude());
          lcd.print(" m");
          break;
  #endif

  #ifdef LIGHT_SENSOR_MODULE
        case LCD_SCREEN_LIGHT:
          lcd.clear();
          lcd.print("Light level: ");
          lcd.print(lightSensor.getLightValuePercentage());
          lcd.progressBar(1, lightSensor.getLightValuePercentage());
          break;
  #endif

  #ifdef MINI_FAN_MODULE
        case LCD_SCREEN_MINIFAN:
          lcd.clear();
          lcd.print("Fan Speed: ");
          lcd.print(miniFan.getFanSpeedPercentage());
          lcd.print("%");
          lcd.setCursor(0, 1);
          lcd.print(miniFan.getFanSpeed());
          break;
  #endif

        default:
          lcd.clear();
          lcd.print("Blank screen");
          break;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(DELAY_LCD));
  }
}

void lcdSetup()
{
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.clear();
  xTaskCreate(lcdTask, "LCD Task", 8192, NULL, 2, NULL);
}
#endif // LCD_MODULE

#ifdef LED_RGB_MODULE
void ledRgbSetup() { rgb.begin(); }
#endif // LED_RGB_MODULE
       /* End of file -------------------------------------------------------- */
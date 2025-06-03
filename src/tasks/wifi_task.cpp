/**
 * @file       wifi_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for WiFi Task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "wifi_task.h"
#include "globals.h"
#include <WiFi.h>

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Task definitions ------------------------------------------- */
void wifiTask(void *pvParameters)
{
  for (;;)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(DEFAULT_SSID_ACLAB, DEFAULT_PASSWORD_ACLAB);
      // Set a timeout (10 seconds)
      unsigned long startTime = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
      {
        vTaskDelay(pdMS_TO_TICKS(500));
      }

      if (WiFi.status() == WL_CONNECTED)
      {
        wifiConnected = true;
#ifdef DEBUG_PRINT
        Serial.println("Connected to WiFi");
#endif // DEBUG_PRINT

#ifdef LCD_MODULE
        lcd.clear();
        lcd.print("WiFi connected");
        lcd.setCursor(0, 1);
        lcd.print("IP: ");
        lcd.print(WiFi.localIP());
#endif // LCD_MODULE
      }
      else
      {
        if (wifiConnected)
        {
#ifdef DEBUG_PRINT
          Serial.println("WiFi failed. Retrying...");
#endif // DEBUG_PRINT

#ifdef LCD_MODULE
          lcd.clear();
          lcd.print("WiFi Failed");
          lcd.setCursor(0, 1);
          lcd.print("Retrying...");
#endif // LCD_MODULE
        }
      }
    }

    vTaskDelay(pdMS_TO_TICKS(5000)); // Check every 5 seconds
  }
}

void wifiSetup()
{
  xTaskCreatePinnedToCore(wifiTask, "WiFi Task", 4096, NULL, 1, NULL, CONFIG_ARDUINO_RUNNING_CORE);
}

/* End of file -------------------------------------------------------- */
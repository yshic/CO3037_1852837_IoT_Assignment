/**
 * @file       button_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for Button Task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "button_task.h"
#include "globals.h"

#ifdef BUTTON_MODULE
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Task definitions ------------------------------------------- */
void buttonTask(void *pvParameters)
{
  for (;;)
  {
    // Serial.print(bspGpioDigitalRead(BUTTON_PIN));
    button.update();
    vTaskDelay(pdMS_TO_TICKS(DELAY_BUTTON));
  }
}

void buttonSetup()
{
  // Button timing config
  button.setDebounceDuration(80);
  button.setDoubleClickInterval(300);
  button.setHoldDuration(1000);

  // Setup callback
  button.attachSingleClickCallback([]() {
  #ifdef DEBUG_BUTTON_CALLBACK
    Serial.println("Single Click");
  #endif // DEBUG_BUTTON_CALLBACK
    lcd.updateScreenState(true);
  });

  button.attachDoubleClickCallback([]() {
  #ifdef DEBUG_BUTTON_CALLBACK
    Serial.println("Double Click");
  #endif // DEBUG_BUTTON_CALLBACK
    lcd.updateScreenState(false);
  });

  xTaskCreate(buttonTask, "Button Task", 8192, NULL, 1, NULL);
}
#endif
/* End of file -------------------------------------------------------- */
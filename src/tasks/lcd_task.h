/**
 * @file       lcd_task.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Header file for LCD Task
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef LCD_TASK_H
  #define LCD_TASK_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define DELAY_LCD 1000
/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Funtions Declaration -------------------------------------------------- */
void lcdTask(void *pvParameters);
void lcdSetup();

void ledRgbSetup();

#endif // LCD_TASK_H

/* End of file -------------------------------------------------------- */
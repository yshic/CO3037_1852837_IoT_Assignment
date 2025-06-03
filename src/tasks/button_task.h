/**
 * @file       button_task.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-03-05
 * @author     Tuan Nguyen
 *
 * @brief      Header file for Button Task
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BUTTON_TASK_H
  #define BUTTON_TASK_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define DELAY_BUTTON 10
/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Funtions Declaration -------------------------------------------------- */
void buttonTask(void *pvParameters);
void buttonSetup();
#endif // BUTTON_TASK_H

/* End of file -------------------------------------------------------- */
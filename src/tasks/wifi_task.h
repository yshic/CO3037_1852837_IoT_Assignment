/**
 * @file       wifi_task.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header file for wifi_task
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef WIFI_TASK_H
  #define WIFI_TASK_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  #include <Preferences.h>
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Funtions Declaration -------------------------------------------------- */
void handleSave();
void captivePortalTask(void *pvParameters);
void wifiTask(void *pvParameters);
void wifiSetup();

#endif // WIFI_TASK_H

/* End of file -------------------------------------------------------- */
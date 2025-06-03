/**
 * @file       actuators_task.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-28
 * @author     Tuan Nguyen
 *
 * @brief      Header file for ACTUATORS_TASK library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef ACTUATORS_TASK_H
  #define ACTUATORS_TASK_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define ACTUATORS_TASK_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Task Declaration -------------------------------------------------- */
void doorSetup();

#endif // ACTUATORS_TASK_H

/* End of file -------------------------------------------------------- */
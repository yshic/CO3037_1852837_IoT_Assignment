/**
 * @file       bsp_pwm.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-05-05
 * @author     Tuan Nguyen
 *
 * @brief      Header file for BSP_PWM library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_PWM_H
#define BSP_PWM_H

/* Includes ----------------------------------------------------------- */
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "esp32-hal-ledc.h"
/* Public defines ----------------------------------------------------- */
#define BSP_PWM_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
class BspPwm {
  public:
  private:
};

#endif // BSP_PWM_H

/* End of file -------------------------------------------------------- */
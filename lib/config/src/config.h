/**
 * @file       config.h
 * @version    0.1.0
 * @date       2025-03-05
 * @author     Tuan Nguyen
 *
 * @brief      Header file for Global configuration
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef CONFIG_H
  #define CONFIG_H

/* Includes ----------------------------------------------------------- */

/* Private defines ---------------------------------------------------- */
  #define USE_FREERTOS

  #ifndef DELAY
    #ifdef USE_FREERTOS
      #define DELAY(ms) vTaskDelay(ms / portTICK_PERIOD_MS)
    #else
      #define DELAY(ms) delay(ms)
    #endif
  #endif

  #ifndef DEBUG_PRINT
    #define DEBUG_PRINT
  #endif // DEBUG_PRINT

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

#endif // CONFIG_H

/* End of file -------------------------------------------------------- */
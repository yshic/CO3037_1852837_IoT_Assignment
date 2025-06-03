/**
 * @file       sensors_task.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-03-01
 * @author     Tuan Nguyen
 *
 * @brief      Header file for Sensors Task
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef SENSORS_TASK_H
  #define SENSORS_TASK_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define DELAY_DHT20        60000
  #define DELAY_SHT4X        60000
  #define DELAY_BMP280       60000
  #define DELAY_LIGHT_SENSOR 10000
  #define DELAY_ULTRASONIC   1000
  #define DELAY_PIRSENSOR    1000
  #define DELAY_MOISTURE     60000
  #define DELAY_SOIL_RS485   60000

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Funtions Declaration -------------------------------------------------- */
void sht40Task(void *pvParameters);
void bmp280Task(void *pvParameters);
void lightSensorTask(void *pvParameters);

void sht40Setup();
void bmp280Setup();
void unitENVIVSetup();
void lightSensorSetup();
#endif // SENSORS_TASK_H

/* End of file -------------------------------------------------------- */
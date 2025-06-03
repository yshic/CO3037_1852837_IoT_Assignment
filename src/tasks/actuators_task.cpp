/**
 * @file       actuators_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-28
 * @author     Tuan Nguyen
 *
 * @brief      Source file for actuators_task.cpp library
 *
 */

/* Includes ----------------------------------------------------------- */

/* Private defines ---------------------------------------------------- */
#include "actuators_task.h"
#include "globals.h"

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Task definitions-------------------------------------------- */
#ifdef SERVO_MODULE
void doorSetup()
{
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  doorServo.setPeriodHertz(50);           // standard 50 hz servo
  doorServo.attach(SERVO_PIN, 500, 1500); // attaches the servo
}
#endif // SERVO_MODULE
       /* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */
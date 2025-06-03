/**
 * @file       logging.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-05-17
 * @author     Tuan Nguyen
 *
 * @brief      Source file for logging.cpp library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "logging.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions-------------------------------------------- */
TaskTimingLogger::TaskTimingLogger(const char *taskName)
    : _taskName(taskName), _timezoneOffset(7), _lastTimeMs(0), _execStartTime(0)
{
}

task_timing_error_t TaskTimingLogger::setTimezoneOffset(int offsetHours)
{
  _timezoneOffset = offsetHours;
  return TASK_LOGGER_OK;
}

task_timing_error_t TaskTimingLogger::logTaskStart()
{
  unsigned long startTimeMs = millis();
  int           startHour   = (startTimeMs / 3600000 + _timezoneOffset) % 24;
  int           startMin    = (startTimeMs % 3600000) / 60000;
  int           startSec    = (startTimeMs % 60000) / 1000;
  Serial.print(_taskName);
  Serial.print(" Start: ");
  Serial.print(startHour);
  Serial.print(":");
  Serial.print(startMin);
  Serial.print(":");
  Serial.print(startSec);
  Serial.print(" (GMT+");
  Serial.print(_timezoneOffset);
  Serial.print(") - ");
  _execStartTime = micros();
  return Serial ? TASK_LOGGER_OK : TASK_LOGGER_FAIL;
}

task_timing_error_t TaskTimingLogger::logExecutionTime(unsigned long execStartTime)
{
  unsigned long execEndTime   = micros();
  unsigned long executionTime = execEndTime - _execStartTime;
  Serial.print("Execution Time: ");
  Serial.print(executionTime);
  Serial.print(" us - Delay Start: ");
  unsigned long delayStartMs = millis();
  int           delayHour    = (delayStartMs / 3600000 + _timezoneOffset) % 24;
  int           delayMin     = (delayStartMs % 3600000) / 60000;
  int           delaySec     = (delayStartMs % 60000) / 1000;
  Serial.print(delayHour);
  Serial.print(":");
  Serial.print(delayMin);
  Serial.print(":");
  Serial.print(delaySec);
  Serial.print(" (GMT+");
  Serial.print(_timezoneOffset);
  Serial.print(") - ");
  return Serial ? TASK_LOGGER_OK : TASK_LOGGER_FAIL;
}

task_timing_error_t TaskTimingLogger::logDelayStart()
{
  return TASK_LOGGER_OK; // Placeholder, no additional logging needed
}

task_timing_error_t TaskTimingLogger::logNextStart()
{
  unsigned long nextStartMs = millis();
  int           nextHour    = (nextStartMs / 3600000 + _timezoneOffset) % 24;
  int           nextMin     = (nextStartMs % 3600000) / 60000;
  int           nextSec     = (nextStartMs % 60000) / 1000;
  Serial.print("Next Start: ");
  Serial.print(nextHour);
  Serial.print(":");
  Serial.print(nextMin);
  Serial.print(":");
  Serial.println(nextSec);
  Serial.print(" (GMT+");
  Serial.println(_timezoneOffset);
  Serial.println(")");
  return Serial ? TASK_LOGGER_OK : TASK_LOGGER_FAIL;
}
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */
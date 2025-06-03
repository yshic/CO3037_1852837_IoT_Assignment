/**
 * @file       logging.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-05-17
 * @author     Tuan Nguyen
 *
 * @brief      Header file for LOGGING library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef LOGGING_H
  #define LOGGING_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define LOGGING_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Enum for TaskTimingLogger error codes.
 */
typedef enum
{
  TASK_LOGGER_OK   = 0, /**< Success */
  TASK_LOGGER_FAIL = -1 /**< General failure, e.g., serial output error */
} task_timing_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
/**
 * @brief A utility class for logging execution times and timestamps of RTOS tasks.
 *
 * This class provides methods to log the start time, execution duration, delay start time,
 * and next execution start time of an RTOS task. Timestamps are adjusted for a configurable
 * timezone offset, and all logging is conditional on the DEBUG_PRINT_RTOS_TIMING macro.
 * The logs are output to the serial console for debugging and evaluation purposes.
 */
class TaskTimingLogger
{
public:
  /**
   * @brief Constructor for TaskTimingLogger.
   *
   * Initializes the logger with the specified task name and sets the default timezone offset to 0.
   *
   * @param[in] taskName The name of the task to be logged (e.g., "DHT20").
   */
  TaskTimingLogger(const char *taskName);

  /**
   * @brief Sets the timezone offset for timestamp logging.
   *
   * Configures the timezone offset in hours relative to GMT (e.g., 7 for GMT+7).
   * This offset is applied to all timestamp calculations.
   *
   * @param[in] offsetHours The timezone offset in hours (e.g., 7 for GMT+7).
   *
   * @return TaskTimingLoggerError_t
   *  - `TASK_LOGGER_OK`: Success
   */
  task_timing_error_t setTimezoneOffset(int offsetHours);

  /**
   * @brief Logs the start time of the task.
   *
   * Records the current time (adjusted for the timezone offset) when the task begins its execution cycle.
   * The timestamp is printed to the serial console in HH:MM:SS format if DEBUG_PRINT_RTOS_TIMING is defined.
   *
   * @attention Ensure Serial.begin() is called before using this method, as it writes to the serial console.
   *
   * @return TaskTimingLoggerError_t
   *  - `TASK_LOGGER_OK`: Success
   *
   *  - `TASK_LOGGER_FAIL`: Serial output failed
   */
  task_timing_error_t logTaskStart();

  /**
   * @brief Logs the execution time of the task and the delay start time.
   *
   * Measures the time taken for the task's main operation (since logTaskStart was called) in microseconds
   * and logs it to the serial console. Also logs the time when the task enters its delay period,
   * adjusted for the timezone offset.
   *
   * @param[in] execStartTime The start time of the task execution in microseconds (from micros()).
   *
   * @attention Ensure Serial.begin() is called before using this method, as it writes to the serial console.
   *            The execStartTime should match the time recorded by logTaskStart.
   *
   * @return TaskTimingLoggerError_t
   *  - `TASK_LOGGER_OK`: Success
   *
   *  - `TASK_LOGGER_FAIL`: Serial output failed
   */
  task_timing_error_t logExecutionTime(unsigned long execStartTime);

  /**
   * @brief Placeholder for logging the delay start time (already handled in logExecutionTime).
   *
   * This method is a placeholder for future expansion. Currently, the delay start time is logged
   * within logExecutionTime.
   *
   * @return TaskTimingLoggerError_t
   *  - `TASK_LOGGER_OK`: Success
   */
  task_timing_error_t logDelayStart();

  /**
   * @brief Logs the time of the next task execution cycle.
   *
   * Records the time when the task resumes after its delay period, adjusted for the timezone offset.
   * The timestamp is printed to the serial console in HH:MM:SS format if DEBUG_PRINT_RTOS_TIMING is defined.
   *
   * @attention Ensure Serial.begin() is called before using this method, as it writes to the serial console.
   *
   * @return TaskTimingLoggerError_t
   *  - `TASK_LOGGER_OK`: Success
   *
   *  - `TASK_LOGGER_FAIL`: Serial output failed
   */
  task_timing_error_t logNextStart();

private:
  const char   *_taskName;       /**< Name of the task being logged */
  int           _timezoneOffset; /**< Timezone offset in hours (e.g., 7 for GMT+7) */
  unsigned long _lastTimeMs;     /**< Last recorded time in milliseconds (for internal use) */
  unsigned long _execStartTime;  /**< Start time of the task execution in microseconds */
};

#endif // LOGGING_H

/* End of file -------------------------------------------------------- */
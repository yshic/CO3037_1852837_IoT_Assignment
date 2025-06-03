/**
 * @file       bsp_rs485.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-29
 * @author     Tuan Nguyen
 *
 * @brief      Header file for BSP_RS485 library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_RS485_H
  #define BSP_RS485_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  #include "bsp_uart.h"
  /* Public defines ----------------------------------------------------- */
  #define BSP_RS485_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */

// Error codes for RS485
typedef enum
{
  BSP_RS485_OK = 0,
  BSP_RS485_ERR,
  BSP_RS485_ERR_READ,
  BSP_RS485_ERR_WRITE,
  BSP_RS485_TIMEOUT
} bsp_rs485_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
class BspRs485 : public Stream
{
public:
  BspRs485(uint8_t uart_nr);

public:
  bsp_rs485_error_t begin(unsigned long baud, uint32_t config = SERIAL_8N1, int8_t rxPin = -1,
                          int8_t txPin = -1, bool invert = false, unsigned long timeout_ms = 20000UL,
                          uint8_t rxfifo_full_thrhd = 112);
  bsp_rs485_error_t end();
  int               available(void) override;
  int               peek(void) override;
  int               read(void) override;
  void              flush() override;
  size_t            write(uint8_t);
  void              sendModbusCommand(byte *command, uint32_t cmdLen, byte *response, uint32_t resLen);

private:
  BspUart *_uart;
};

extern BspRs485 rs485Serial0;

extern BspRs485 rs485Serial1;

extern BspRs485 rs485Serial2;
#endif // BSP_RS485_H

/* End of file -------------------------------------------------------- */
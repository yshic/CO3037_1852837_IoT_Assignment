/**
 * @file       bsp_rs485.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-29
 * @author     Tuan Nguyen
 *
 * @brief      Source file for bsp_rs485.cpp library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_rs485.h"
#include "config.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
BspRs485 rs485Serial0(0);

BspRs485 rs485Serial1(1);

BspRs485 rs485Serial2(2);
/* Private variables -------------------------------------------------- */

/* Class method definitions-------------------------------------------- */
BspRs485::BspRs485(uint8_t uart_nr)
{
  switch (uart_nr)
  {
    case 0:
      _uart = &uart0;
      break;
    case 1:
      _uart = &uart1;
      break;
    case 2:
      _uart = &uart2;
      break;
    default:
      _uart = &uart1;
      break;
  }
}

bsp_rs485_error_t BspRs485::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin,
                                  bool invert, unsigned long timeout_ms, uint8_t rxfifo_full_thrhd)
{
  _uart->begin(baud, config, rxPin, txPin, invert, timeout_ms, rxfifo_full_thrhd);
  return BSP_RS485_OK;
}

bsp_rs485_error_t BspRs485::end()
{
  _uart->end();
  return BSP_RS485_OK;
}

int BspRs485::available(void) { return _uart->available(); }

int BspRs485::peek(void) { return _uart->peek(); }

int BspRs485::read(void) { return _uart->read(); }

void BspRs485::flush() { _uart->flush(); }

size_t BspRs485::write(uint8_t c) { return _uart->write(c); }

void BspRs485::sendModbusCommand(byte *command, uint32_t cmdLen, byte *response, uint32_t resLen)
{
  _uart->write(command, cmdLen);
  _uart->flush();
  DELAY(100);
  if (_uart->available() >= resLen)
  {
    _uart->readBytes(response, resLen);
  }
}
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */
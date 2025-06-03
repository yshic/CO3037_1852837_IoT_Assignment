/**
 * @file       bsp_uart.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-29
 * @author     Tuan Nguyen
 *
 * @brief      Source file for bsp_uart.cpp library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_uart.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
BspUart uart0(0);

BspUart uart1(1);

BspUart uart2(2);

/* Private variables -------------------------------------------------- */

/* Functions definitions-------------------------------------------- */
BspUart::BspUart(uint8_t uart_nr)
{
  switch (uart_nr)
  {
    case 0:
      _uart = &Serial0;
      break;
    case 1:
      _uart = &Serial1;
      break;
    case 2:
      _uart = &Serial2;
      break;
    default:
      _uart = &Serial1;
      break;
  }
}

bool BspUart::setRxTimeout(uint8_t symbols_timeout) { return _uart->setRxTimeout(symbols_timeout); }

bool BspUart::setRxFIFOFull(uint8_t fifoBytes) { return _uart->setRxFIFOFull(fifoBytes); }

bsp_uart_error_t BspUart::onReceiveError(OnReceiveErrorCb function)
{
  _uart->onReceiveError(function);
  return BSP_UART_OK;
}

bsp_uart_error_t BspUart::onReceive(OnReceiveCb function, bool onlyOnTimeout)
{
  _uart->onReceive(function, onlyOnTimeout);
  return BSP_UART_OK;
}

bsp_uart_error_t BspUart::eventQueueReset()
{
  _uart->eventQueueReset();
  return BSP_UART_OK;
}

bsp_uart_error_t BspUart::updateBaudRate(unsigned long baud)
{
  _uart->updateBaudRate(baud);
  return BSP_UART_OK;
}

bsp_uart_error_t BspUart::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert,
                                unsigned long timeout_ms, uint8_t rxfifo_full_thrhd)
{
  _uart->begin(baud, config, rxPin, txPin, invert, timeout_ms, rxfifo_full_thrhd);
  return BSP_UART_OK;
}

bsp_uart_error_t BspUart::end(void)
{
  _uart->end();
  return BSP_UART_OK;
}

int BspUart::available(void) { return _uart->available(); }

int BspUart::availableForWrite(void) { return _uart->availableForWrite(); }

int BspUart::peek(void) { return _uart->peek(); }

int BspUart::read(void) { return _uart->read(); }

size_t BspUart::read(char *buffer, size_t size) { return read((uint8_t *) buffer, size); }

size_t BspUart::read(uint8_t *buffer, size_t size) { return _uart->read(buffer, size); }

size_t BspUart::readBytes(uint8_t *buffer, size_t length) { return _uart->readBytes(buffer, length); }

size_t BspUart::readBytes(char *buffer, size_t length) { return readBytes((uint8_t *) buffer, length); }

void BspUart::flush(void) { _uart->flush(); }

bsp_uart_error_t BspUart::flush(bool txOnly)
{
  _uart->flush(txOnly);
  return BSP_UART_OK;
}

size_t BspUart::write(uint8_t c) { return _uart->write(c); }

size_t BspUart::write(const uint8_t *buffer, size_t size) { return _uart->write(buffer, size); }

uint32_t BspUart::baudRate() { return _uart->baudRate(); }

bsp_uart_error_t BspUart::setDebugOutput(bool en)
{
  _uart->setDebugOutput(en);
  return BSP_UART_OK;
}

bsp_uart_error_t BspUart::setRxInvert(bool invert)
{
  _uart->setRxInvert(invert);
  return BSP_UART_OK;
}

bool BspUart::setPins(int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin)
{
  return _uart->setPins(rxPin, txPin, ctsPin, rtsPin);
}

bool BspUart::setHwFlowCtrlMode(SerialHwFlowCtrl mode, uint8_t threshold)
{
  return _uart->setHwFlowCtrlMode(mode, threshold);
}

bool BspUart::setMode(SerialMode mode) { return _uart->setMode(mode); }

size_t BspUart::setRxBufferSize(size_t new_size) { return _uart->setRxBufferSize(new_size); }

size_t BspUart::setTxBufferSize(size_t new_size) { return _uart->setTxBufferSize(new_size); }
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */
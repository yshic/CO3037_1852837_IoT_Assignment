/**
 * @file       bsp_uart.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-29
 * @author     Tuan Nguyen
 *
 * @brief      Header file for BSP_UART library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_UART_H
  #define BSP_UART_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define BSP_UART_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */

// Error codes for UART
typedef enum
{
  BSP_UART_OK = 0,
  BSP_UART_ERR,
  BSP_UART_ERR_READ,
  BSP_UART_ERR_WRITE,
  BSP_UART_TIMEOUT
} bsp_uart_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Functions Declaration -------------------------------------------------- */
class BspUart : public Stream
{
public:
  BspUart(uint8_t uart_nr);

public:
  bool setRxTimeout(uint8_t symbols_timeout);

  bool setRxFIFOFull(uint8_t fifoBytes);

  bsp_uart_error_t onReceive(OnReceiveCb function, bool onlyOnTimeout = false);

  bsp_uart_error_t onReceiveError(OnReceiveErrorCb function);

  bsp_uart_error_t eventQueueReset();

  bsp_uart_error_t updateBaudRate(unsigned long baud);

  bsp_uart_error_t begin(unsigned long baud, uint32_t config = SERIAL_8N1, int8_t rxPin = -1,
                         int8_t txPin = -1, bool invert = false, unsigned long timeout_ms = 20000UL,
                         uint8_t rxfifo_full_thrhd = 112);

  bsp_uart_error_t end(void);

  int available(void) override;

  int availableForWrite(void);

  int peek(void) override;

  int read(void) override;

  size_t read(uint8_t *buffer, size_t size);

  size_t read(char *buffer, size_t size);

  size_t readBytes(uint8_t *buffer, size_t length);

  size_t readBytes(char *buffer, size_t length);

  void flush(void) override;

  bsp_uart_error_t flush(bool txOnly);

  size_t write(uint8_t);

  size_t write(const uint8_t *buffer, size_t size);

  inline size_t write(const char *buffer, size_t size) { return write((uint8_t *) buffer, size); }

  inline size_t write(const char *s) { return write((uint8_t *) s, strlen(s)); }

  inline size_t write(unsigned long n) { return write((uint8_t) n); }

  inline size_t write(long n) { return write((uint8_t) n); }

  inline size_t write(unsigned int n) { return write((uint8_t) n); }

  inline size_t write(int n) { return write((uint8_t) n); }

  uint32_t baudRate();

  bsp_uart_error_t setDebugOutput(bool);

  bsp_uart_error_t setRxInvert(bool);

  bool setPins(int8_t rxPin, int8_t txPin, int8_t ctsPin = -1, int8_t rtsPin = -1);

  bool setHwFlowCtrlMode(SerialHwFlowCtrl mode      = UART_HW_FLOWCTRL_CTS_RTS,
                         uint8_t          threshold = 64); // 64 is half FIFO Length

  bool setMode(SerialMode mode);

  size_t setRxBufferSize(size_t new_size);

  size_t setTxBufferSize(size_t new_size);

private:
  HardwareSerial *_uart;
};

extern BspUart uart0;

extern BspUart uart1;

extern BspUart uart2;
#endif // BSP_UART_H

/* End of file -------------------------------------------------------- */
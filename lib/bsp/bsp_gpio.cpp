/**
 * @file       bsp_gpio.cpp
 * @license    This project is released under the MIT License.
 * @version    1.0.0
 * @date       2024-12-31
 * @author     Tuan Nguyen
 *
 * @brief      Source file for bsp_gpio
 *
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_gpio.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */

/* Digital Pin -------------------------------------------------------- */

void bspGpioPinMode(uint8_t pin, uint8_t mode)
{
  switch (mode)
  {
    case INPUT:
      pinMode(pin, INPUT);
      break;
    case INPUT_PULLUP:
      pinMode(pin, INPUT_PULLUP);
      break;
    case INPUT_PULLDOWN:
      pinMode(pin, INPUT_PULLDOWN);
      break;
    case OUTPUT:
      pinMode(pin, OUTPUT);
      break;
    case OPEN_DRAIN:
      pinMode(pin, OPEN_DRAIN);
      break;
    default:
      break;
  }
}

void bspGpioDigitalWrite(uint8_t pin, uint8_t state)
{
  switch (state)
  {
    case 0:
      digitalWrite(pin, state);
      break;
    case 1:
      digitalWrite(pin, state);
      break;
    default:
      break;
  }
}

bool bspGpioDigitalRead(uint8_t pin) { return digitalRead(pin) == 1 ? true : false; }

/* Analog Pin --------------------------------------------------------- */

int  bspGpioAnalogRead(uint8_t pin) { return analogRead(pin); }
void bspGpioAnalogReadResolution(uint8_t bits) { analogReadResolution(bits); }
void bspGpioAnalogWrite(uint8_t pin, int value) { analogWrite(pin, value); }
void bspGpioAnalogWriteResolution(uint8_t bits) { analogWriteResolution(bits); }

/* Advanced I/O ------------------------------------------------------- */

unsigned long bspGpioPulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
  return pulseIn(pin, state, timeout);
}
unsigned long bspGpioPulseInLong(uint8_t pin, uint8_t state, unsigned long timeout)
{
  return pulseInLong(pin, state, timeout);
}
uint8_t bspGpioShiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
  return shiftIn(dataPin, clockPin, bitOrder);
}
void bspGpioShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
  shiftOut(dataPin, clockPin, bitOrder, val);
}
void bspGpioTone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
  tone(pin, frequency, duration);
}
void bspGpioNoTone(uint8_t pin) { noTone(pin); }

/* Private definitions ------------------------------------------------ */
/* End of file -------------------------------------------------------- */

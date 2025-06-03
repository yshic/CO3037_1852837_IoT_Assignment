/**
 * @file       bsp_gpio.h
 * @license    This project is released under the MIT License.
 * @version    1.0.0
 * @date       2024-12-31
 * @author     Tuan Nguyen
 *
 * @brief      Header file for bsp_gpio
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_GPIO_H
  #define BSP_GPIO_H

  /* Includes ----------------------------------------------------------- */
  #include <Arduino.h>

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief This function sets the mode of a GPIO pin
 *
 * @param[in] pin      GPIO Pin
 * @param[in] mode     GPIO Mode
 *
 * @attention  None
 *
 * @return
 *
 * None
 */
void bspGpioPinMode(uint8_t pin, uint8_t mode);

/* Digital Pin -------------------------------------------------------- */

/**
 * @brief This function writes the state of a digital pin
 *
 * @param[in] pin           GPIO Pin
 * @param[in] state         State to write to the GPIO Pin
 *
 * @attention None
 *
 * @return
 *
 * None
 */
void bspGpioDigitalWrite(uint8_t pin, uint8_t state);

/**
 * @brief This function reads the state of a digital pin
 *
 * @param[in] pin      GPIO Pin
 *
 * @attention  If the pin isn’t connected to anything, bspGpioDigitalRead() can return either HIGH or LOW (and
 * this can change randomly).
 *
 * @return
 * - true - HIGH
 *
 * - false - LOW
 *
 * - Data type: bool
 */
bool bspGpioDigitalRead(uint8_t pin);

/* Analog Pin -------------------------------------------------------- */

/**
 * @brief This function reads the value from the specified analog pin
 *
 * @param[in] pin      Analog input pin
 *
 * @attention  If the analog input pin is not connected to anything, the value returned by bspGpioAnalogRead
 * will
 * fluctuate based on a number of factors (e.g. the values of the other analog inputs, how close your hand is
 * to the board, etc.).
 *
 * @return
 * - The analog reading on the pin. Although it is limited to the resolution of the analog to digital
 * converter * (0-1023 for 10 bits or 0-4095 for 12 bits).
 *
 * - Data type: int
 */
int bspGpioAnalogRead(uint8_t pin);

/**
 * @brief This function sets the size (in bits) of the value returned by bspGpioAnalogRead().
 *
 * @param[in] bits      The resolution (in bits). You can set this between 1 and 32.
 *
 * @attention
 * - If you set the bspGpioAnalogReadResolution() value to a value higher than your board’s
 * capabilities, the board will only report back at its highest resolution, padding the extra bits
 * with zeros.
 *
 * - If you set the bspGpioAnalogReadResolution() value to a value lower than your board’s capabilities, the
 * extra least significant bits read from the ADC will be discarded.
 *
 * @return
 * None
 */
void bspGpioAnalogReadResolution(uint8_t bits);

/**
 * @brief This function writes an analog value (PWM wave) to a pin.
 *
 * @param[in] pin       The pin to write to.
 * @param[in] value     The duty cycle: between 0 (always off) and 255 (always on). Allowed data types: int
 *
 * @return
 * None
 */
void bspGpioAnalogWrite(uint8_t pin, int value);

/**
 * @brief This function sets the resolution of the bspGpioAnalogWrite() function. It defaults to 8 bits
 * (values between 0-255).
 *
 * @param[in] bits      The resolution (in bits). The value can range from 1 to 32.
 *
 * @attention
 * - If you set the bspGpioAnalogWriteResolution() value to a value higher than your board’s capabilities, the
 * board will discard the extra bits.
 *
 * - If you set the bspGpioAnalogWriteResolution() value to a value lower than your board’s capabilities, the
 * missing bits will be padded with zeros to fill the hardware required size.
 *
 * @return
 * None
 */
void bspGpioAnalogWriteResolution(uint8_t bits);

/* Advanced I/O ------------------------------------------------------- */

/**
 * @brief This function reads a pulse (either HIGH or LOW) on a pin.
 *
 * @param[in] pin       The pin on which you want to read the pulse. Allowed data types: int.
 * @param[in] value     Type of pulse to read: either HIGH or LOW. Allowed data types: int.
 * @param[in] timeout   (optional): The number of microseconds to wait for the pulse to start; default is one
 * second. Allowed data types: unsigned long.
 *
 * @return
 * - The length of the pulse (in microseconds) or 0 if no pulse started before the timeout.
 *
 * - Data type: unsigned long
 */
unsigned long bspGpioPulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000UL);

/**
 * @brief This function is an alternative to bspGpioPulseIn() which is better at handling long pulse and
 * interrupt affected scenarios.
 *
 * @param[in] pin       The pin on which you want to read the pulse. Allowed data types: int.
 * @param[in] value     Type of pulse to read: either HIGH or LOW. Allowed data types: int.
 * @param[in] timeout   (optional): The number of microseconds to wait for the pulse to start; default is one
 * second. Allowed data types: unsigned long.
 *
 * @return
 * - The length of the pulse (in microseconds) or 0 if no pulse started before the timeout.
 *
 * - Data type: unsigned long
 */
unsigned long bspGpioPulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000UL);

/**
 * @brief This function shifts in a byte of data one bit at a time. Starts from either the most (i.e. the
 * leftmost) or least (rightmost) significant bit. For each bit, the clock pin is pulled high, the next bit
 * is read from the data line, and then the clock pin is taken low.
 *
 * @param[in] dataPin       The pin on which to input each bit. Allowed data types: int.
 * @param[in] clockPin      The pin to toggle to signal a read from dataPin.
 * @param[in] bitOrder      Order to shift in the bits; either MSBFIRST or LSBFIRST. (Most Significant Bit
 * First, or, Least Significant Bit First).
 *
 * @return
 * - The value read.
 *
 * - Data type: uint8_t
 */
uint8_t bspGpioShiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

/**
 * @brief This function shifts out a byte of data one bit at a time. Starts from either the most (i.e. the
 * leftmost) or least (rightmost) significant bit. Each bit is written in turn to a data pin, after which a
 * clock pin is pulsed (taken high, then low) to indicate that the bit is available.
 *
 * @param[in] dataPin       The pin on which to output each bit. Allowed data types: int.
 * @param[in] clockPin      The pin to toggle once the dataPin has been set to the correct value. Allowed
 * data types: int.
 * @param[in] bitOrder      Order to shift out the bits; either MSBFIRST or LSBFIRST. (Most Significant Bit
 * First, or, Least Significant Bit First).
 * @param[in] val           The data to shift out. Allowed data types: uint8_t.
 *
 * @return
 * None
 */
void bspGpioShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

/**
 * @brief This function generates a square wave of the specified frequency (and 50% duty cycle) on a pin. A
 * duration can be specified, otherwise the wave continues until a call to bspGpioNoTone().
 *
 * @param[in] pin           The pin on which to generate the tone.
 * @param[in] frequency     The frequency of the tone in hertz. Allowed data types: unsigned int.
 * @param[in] duration      (optional): The duration of the tone in milliseconds . Allowed data types:
 * unsigned long.
 *
 * @return
 * None
 */
void bspGpioTone(uint8_t pin, unsigned int frequency, unsigned long duration);

/**
 * @brief This function stops the generation of a square wave triggered by bspGpioTone(). Has no effect if no
 * tone is being generated.
 *
 * @param[in] pin           The pin on which to stop generating the tone
 *
 * @attention
 * If you want to play different pitches on multiple pins, you need to call bspGpioNoTone() on one pin before
 * calling bspGpioTone() on the next pin.
 *
 * @return
 * None
 */
void bspGpioNoTone(uint8_t pin);




#endif /* BSP_GPIO_H */

/* End of file -------------------------------------------------------- */

#include "lcd_16x2.h"

#include <lcd_16x2_constants.h>

/**
 * @brief function begin
 *
 * @param beginWire if true start I²C wire
 */
void LCD_I2C::begin(TwoWire *wire)
{
  _wire = wire;

  // Clear i2c adapter
  I2C_Write(0b00000000);
  // Wait more than 40 ms after powerOn
  delay(50);
  InitializeLCD();
}

/**
 * @brief switch backlight on
 *
 */
void LCD_I2C::backlight()
{
  _output.led = 1;
  // Led pin is independent from LCD data and control lines.
  I2C_Write(0b00000000 | _output.led << 3);
}

/**
 * @brief switch backlight off
 *
 */
void LCD_I2C::backlightOff()
{
  _output.led = 0;
  // Led pin is independent from LCD data and control lines.
  I2C_Write(0b00000000 | _output.led << 3);
}

/**
 * @brief Clears the LCD screen and positions the cursor in the upper-left corner.
 *
 */
void LCD_I2C::clear()
{
  _output.rs = 0;
  _output.rw = 0;

  LCD_Write(HD44780_CLEAR_DISPLAY);
  delayMicroseconds(1550);
}

/**
 * @brief Positions the cursor in the upper-left of the LCD.
 *  That is, use that location in outputting subsequent text to the display.
 *  To also clear the display, use the clear() function instead.
 *
 */
void LCD_I2C::home()
{
  _output.rs = 0;
  _output.rw = 0;

  LCD_Write(HD44780_CURSOR_HOME);
  delayMicroseconds(1550);
}

/**
 * @brief Set the direction for text written to the LCD to left-to-right, the default.
 *  This means that subsequent characters written to the display will go from left to right,
 *  but does not affect previously-output text.
 *
 */
void LCD_I2C::leftToRight()
{
  _output.rs = 0;
  _output.rw = 0;

  _entryState |= HD44780_ENTRY_SHIFTINCREMENT;

  LCD_Write(HD44780_ENTRY_MODE_SET | _entryState);
  delayMicroseconds(37);
}

/**
 * @brief Set the direction for text written to the LCD to right-to-left (the default is left-to-right).
 *  This means that subsequent characters written to the display will go from right to left,
 *  but does not affect previously-output text.
 *
 */
void LCD_I2C::rightToLeft()
{
  _output.rs = 0;
  _output.rw = 0;

  _entryState &= ~HD44780_ENTRY_SHIFTINCREMENT;

  LCD_Write(HD44780_ENTRY_MODE_SET | _entryState);
  delayMicroseconds(37);
}

/**
 * @brief set autoscroll on.
 *  Each character is printed on the same location on the LCD.
 *  Depending on the current text direction, autoscroll moves each character
 *  to the left when left => right is set.
 *  Or moves each character to the right when right => left is set.
 *
 */
void LCD_I2C::autoscroll()
{
  _output.rs = 0;
  _output.rw = 0;

  _entryState |= HD44780_ACCOMPANIES_DISPLAY_SHIFT;

  LCD_Write(HD44780_ENTRY_MODE_SET | _entryState);
  delayMicroseconds(37);
}

/**
 * @brief Set autoscroll off.
 * Moves the cursor one step when adding a character.
 *
 */
void LCD_I2C::autoscrollOff()
{
  _output.rs = 0;
  _output.rw = 0;

  _entryState &= ~HD44780_ACCOMPANIES_DISPLAY_SHIFT;

  LCD_Write(HD44780_ENTRY_MODE_SET | _entryState);
  delayMicroseconds(37);
}

/**
 * @brief Turns on the LCD display, after it’s been turned off with noDisplay().
 * This will restore the text (and cursor) that was on the display.
 *
 */
void LCD_I2C::display()
{
  _output.rs = 0;
  _output.rw = 0;

  _displayState |= HD44780_DISPLAY_ON;

  LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
  delayMicroseconds(37);
}

/**
 * @brief Turns off the LCD display, without losing the text currently shown on it.
 *
 */
void LCD_I2C::displayOff()
{
  _output.rs = 0;
  _output.rw = 0;

  _displayState &= ~HD44780_DISPLAY_ON;

  LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
  delayMicroseconds(37);
}

/**
 * @brief Shows the cursor.
 *
 */
void LCD_I2C::cursor()
{
  _output.rs = 0;
  _output.rw = 0;

  _displayState |= HD44780_CURSOR_ON;

  LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
  delayMicroseconds(37);
}

/**
 * @brief Hides the cursor.
 *
 */
void LCD_I2C::cursorOff()
{
  _output.rs = 0;
  _output.rw = 0;

  _displayState &= ~HD44780_CURSOR_ON;

  LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
  delayMicroseconds(37);
}

/**
 * @brief Display the blinking LCD cursor.
 *  If used in combination with the cursor() function, the result will depend on the particular display.
 *
 */
void LCD_I2C::blink()
{
  _output.rs = 0;
  _output.rw = 0;

  _displayState |= HD44780_BLINK_ON;

  LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
  delayMicroseconds(37);
}

/**
 * @brief Turns off the blinking LCD cursor.
 *
 */
void LCD_I2C::blinkOff()
{
  _output.rs = 0;
  _output.rw = 0;

  _displayState &= ~HD44780_BLINK_ON;

  LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
  delayMicroseconds(37);
}

/**
 * @brief Scrolls the contents of the display (text and cursor) one space to the left.
 *
 */
void LCD_I2C::scrollDisplayLeft()
{
  _output.rs = 0;
  _output.rw = 0;

  LCD_Write(0b00011000);
  delayMicroseconds(37);
}

/**
 * @brief Scrolls the contents of the display (text and cursor) one space to the right.
 *
 */
void LCD_I2C::scrollDisplayRight()
{
  _output.rs = 0;
  _output.rw = 0;

  LCD_Write(0b00011100);
  delayMicroseconds(37);
}

/**
 * @brief Create a custom character (glyph) for use on the LCD.
 *  Up to eight characters of 5x8 pixels are supported (numbered 0 to 7).
 *  The appearance of each custom character is specified by an array of eight bytes,
 *  one for each row. The five least significant bits of each byte determine the pixels in that row.
 *  To display a custom character on the screen, write() its number.
 *
 * @param memory_location memory location where char is saved
 * @param charmap defined character map
 */
void LCD_I2C::createChar(uint8_t memory_location, uint8_t charmap[])
{
  _output.rs = 0;
  _output.rw = 0;

  memory_location %= 8;

  LCD_Write(HD44780_SET_CGRAM_ADDR | (memory_location << 3));
  delayMicroseconds(37);

  for (int i = 0; i < 8; i++)
    write(charmap[i]);

  setCursor(0, 0); // Set the address pointer back to the DDRAM
}

/**
 * @brief Set Cursor to position defined by column and row
 *
 * @param col
 * @param row
 */
void LCD_I2C::setCursor(uint8_t column, uint8_t row)
{
  static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  _output.rs                         = 0;
  _output.rw                         = 0;
  // sanity limits
  if (column > _columnMax)
  {
    column = _columnMax;
  }
  // sanity limits
  if (row > _rowMax)
  {
    row = _rowMax;
  }

  uint8_t newAddress = row_offsets[row] + column;

  LCD_Write(HD44780_SET_DDRRAM_ADDR | newAddress);
  delayMicroseconds(37);
}

/**
 * @brief Write a character to the LCD.
 *
 * @param character to write
 * @return size_t written bytes
 */
size_t LCD_I2C::write(uint8_t character)
{
  _output.rs = 1;
  _output.rw = 0;

  LCD_Write(character);
  delayMicroseconds(41);

  return 1;
}

/**
 * @brief Function to initialize LCD
 *
 */
void LCD_I2C::InitializeLCD()
{
  // See HD44780U datasheet "Initializing by Instruction" Figure 24 (4-Bit Interface)
  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40 ms after power rises above 2.7 V
  // before sending commands. Arduino can turn on way before 4.5 V so we'll wait 50
  _output.rs = 0;
  _output.rw = 0;

  // wait more than 40 ms after Vcc = 2.7 V
  delay(50);

  // first - 0x30
  LCD_Write(0b00110000, true);
  delayMicroseconds(4200);
  // second - 0x30
  LCD_Write(0b00110000, true);
  delayMicroseconds(150);
  // third - 0x30
  LCD_Write(0b00110000, true);
  delayMicroseconds(37);

  // Function Set - 4 bits mode write 0x20
  LCD_Write(0b00100000, true);
  delayMicroseconds(37);

  // Setup Display Function Set  - 4 bit Interface, 1 = 2 lines, 0 = 5x8 font
  LCD_Write(HD44780_FUNCTION_SET | HD44780_4_BIT_MODE | HD44780_2_LINE | HD44780_5x8_DOTS);
  delayMicroseconds(37);

  displayOff();
  clear();
  leftToRight();
}

/**
 * @brief write character code from corresponding code table
 *
 * see HD44780.pdf => page 17
 *
 * @param output
 */
void LCD_I2C::writeCharCode(uint8_t code)
{
  _output.rs = 1;
  LCD_Write(code);
  delayMicroseconds(37);
}

/**
 * @brief I²C write function
 *
 * @param output data to write
 */
void LCD_I2C::I2C_Write(uint8_t output)
{
  _wire->beginTransmission(_address);
  _wire->write(output);
  _wire->endTransmission();
}

/**
 * @brief LCD_Write function
 *
 * @param output data to write
 * @param initialization initialization
 */
void LCD_I2C::LCD_Write(uint8_t output, bool initialization)
{
  _output.data = output;

  _output.en = true;
  I2C_Write(_output.GetHighData());
  // High part of enable should be > 450 ns
  delayMicroseconds(1);

  _output.en = false;
  I2C_Write(_output.GetHighData());

  // During initialization we only send half a byte
  if (!initialization)
  {
    // I think we need a delay between half byte writes, but no sure how long it needs to be.
    delayMicroseconds(37);

    _output.en = true;
    I2C_Write(_output.GetLowData());
    // High part of enable should be > 450 ns
    delayMicroseconds(1);

    _output.en = false;
    I2C_Write(_output.GetLowData());
  }
}

void LCD_I2C::progressBar(uint8_t row, uint8_t progress)
{
  // Define the width of the progress bar (in characters)
  const uint8_t progressBarWidth = 16;
  // Define the number of custom characters needed
  const uint8_t customChars = 5;

  // Define the custom characters for the progress bar
  uint8_t progressChars[customChars][8] = {
  {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000}, // 1/5
  {0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000}, // 2/5
  {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100}, // 3/5
  {0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110}, // 4/5
  {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111}  // 5/5 (full block)
  };

  // Create the custom characters
  for (uint8_t i = 0; i < customChars; i++)
  {
    createChar(i, progressChars[i]);
  }

  // Calculate the number of full and partial blocks
  uint8_t fullBlocks   = (progress * progressBarWidth) / 100;
  uint8_t partialBlock = ((progress * progressBarWidth) % 100) / (100 / customChars);

  // Set the cursor to the specified row
  setCursor(0, row);

  // Display the progress bar
  for (uint8_t i = 0; i < progressBarWidth; i++)
  {
    if (i < fullBlocks)
    {
      write(0xFF); // Full block character (use 0xFF for a full block)
    }
    else if (i == fullBlocks)
    {
      write(partialBlock); // Partial block character
    }
    else
    {
      write(' '); // Empty space
    }
  }
}

lcd_screen_state_t LCD_I2C::getScreenState() { return lcdScreenState; }

void LCD_I2C::setScreenState(lcd_screen_state_t screenState) { lcdScreenState = screenState; }

void LCD_I2C::updateScreenState(bool increment)
{
  if (LCD_SCREEN_COUNT == 0)
  {
    return;
  }
  if (increment)
  {
    lcdScreenState = (lcd_screen_state_t) ((lcdScreenState + 1) % LCD_SCREEN_COUNT);
#ifdef DEBUG_PRINT_SCREEN_STATE
    Serial.println(lcdScreenState);
#endif // DEBUG_PRINT_SCREEN_STATE
  }
  else
  {
    lcdScreenState = (lcd_screen_state_t) ((lcdScreenState - 1 + LCD_SCREEN_COUNT) % LCD_SCREEN_COUNT);
#ifdef DEBUG_PRINT_SCREEN_STATE
    Serial.println(lcdScreenState);
#endif // DEBUG_PRINT_SCREEN_STATE
  }
}
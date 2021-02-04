# ST7920Emulator
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

## About
The ST7920Emulator is a platform independent emulator library that parses ST7920 serial data and draws the calculated display contents using user a supplied function. This library has no external dependencies as both data I/O of the serial data and the controlling of an external display is done by external code.

Features:
* Platform independent implementation
* Full 16x8 half size font included
* Character generation
* Graphics display
* Seperate RAM sections for DDRAM and GDRAM which enables realistic representation of the interaction of both display data sources

Limitations:
* No IRAM support
* No scroll or cursor control support

## Usage
The constructor of the emulator takes two functions as a parameter.

The first function has no argument and is called when the display area of the ST7920 shall be cleared, e.g.: ```void clearDisplay(void)```

The second function takes three uint8_t values as a parameter and is called when one byte of data shall be drawn on the display, e.g.: ```void drawByte(uint8_t x, uint8_t y, uint8_t d)```

Each bit in the data byte to draw represents one pixel of the ST7920. If the bit is set, the corresponding pixel shall be drawn. The lowest bit of the data byte is the first pixel to draw. This allows for easier draw loops by either shifting the data byte to the right and checking the lowest bit in a loop or by using the for loop variable as a bit index as shown below.

Example code:
```
#include "St7920Emulator.hpp"

/* Example interface to function that fills a given rect. using the supplied color.
void fill_rect(start_rect_x, start_rect_y, width_x, width_y, color) */
extern void fill_rect(...);

/* Clear display area used by ST7920 content */
void clearDisplay() {
    // Clear ST7920 display area
    fill_rect(ST7920_OFFSET_LEFT,
              ST7920_OFFSET_TOP,
              ST7920_PIXEL_SIZE * 128,
              ST7920_PIXEL_SIZE * 64,
              COLOR_WHITE);
}

/* Draw one byte of display data */
/* Pixel values are in ascending direction in the data byte, see below */
void drawByte(uint8_t x, uint8_t y, uint8_t d) {
    // Loop over all bits
    for (uint8_t i = 0; i < 8; ++i, ++x) {
        // Check if bit is set
        if ((d & (1 << i)) > 0) {
            // Pixel is set
            fill_rect(ST7920_OFFSET_LEFT + ST7920_PIXEL_SIZE * x,
                      ST7920_OFFSET_TOP + ST7920_PIXEL_SIZE * y,
                      ST7920_PIXEL_SIZE,
                      ST7920_PIXEL_SIZE,
                      COLOR_BLACK);
        } else {
            // Pixel is not set
            fill_rect(ST7920_OFFSET_LEFT + ST7920_PIXEL_SIZE * x,
                      ST7920_OFFSET_TOP + ST7920_PIXEL_SIZE * y,
                      ST7920_PIXEL_SIZE,
                      ST7920_PIXEL_SIZE,
                      COLOR_WHITE);
        }
    }
}

/* St7920Emulator handle */
St7920Emulator st7920Emulator(clearDisplay, drawByte);

/* Example interface function to get one byte from the serial input buffer:
uint8_t get_serial_data(uint8_t *);
=> Return 0 if no data available and >0 if data has been written to the supplied pointer.
*/
extern uint8_t get_serial_data(uint8_t *);

/* main function */
void main() {
    /* init all external peripherals */

    // Endless loop
    uint8_t ui8Data;
    while (true) {
        // Check if new serial data is available
        if (get_serial_data(&ui8Data) > 0) {
            // Processed received byte
            st7920Emulator.parseSerialData(ui8Data);
        }
    }
}
```

## Memory usage
The ST7920Emulator uses around 1300 bytes of RAM and 5500 bytes of ROM.

## License
ST7920Emulator is licensed under [GPL v3](https://www.gnu.org/licenses/gpl-3.0)

## Support
If you like this project (for what so ever reason) and want to show your apprecication you might want to [![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/L3L03JDC5) or <a href="https://www.buymeacoffee.com/teeminus" target="_blank"><img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="Buy Me A Coffee" style="height: 41px !important;width: 174px !important;box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;-webkit-box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;" ></a>
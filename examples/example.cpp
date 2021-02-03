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
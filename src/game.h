#ifndef GBA_H
#define GBA_H
#include <stdint.h>
#include "hardware.h"

// ---------------------------------------------------------------------------
//                       MODE3 MACROS
// ---------------------------------------------------------------------------
#define OFFSET(r, c, rowlen) (((c)*(rowlen))+(r))

#define COLOR(r, g, b)  ((r) | (g)<<5 | (b)<<10)

#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0
#define GRAY COLOR(5, 5, 5)

// The size of the GBA Screen
#define WIDTH 240
#define HEIGHT 160

// ---------------------------------------------------------------------------
//                       BUTTON INPUT
// ---------------------------------------------------------------------------
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN(key, buttons) (~(buttons) & (key))

// TA-TODO: COMPLETE THIS MACRO.
// Remember that a button is recently pressed if it wasn't pressed in the last
// input (oldButtons) but is pressed in the current input. Use the KEY_DOWN
// macro to check if the button was pressed in the inputs.
#define KEY_JUST_PRESSED(key, buttons, oldbuttons) (KEY_DOWN(key, buttons) && !(KEY_DOWN(key, oldbuttons)))

// ---------------------------------------------------------------------------
//                       MISC
// ---------------------------------------------------------------------------
#define UNUSED(param) ((void)((param)))

/**
 * Generates a pseudo-random number between min and max, inclusive.
 *
 * @param  min bottom end of range.
 * @param  max top end of range.
 * @return random number in the given range.
 */
int randint(int min, int max);

// ---------------------------------------------------------------------------
//                       DRAWING
// ---------------------------------------------------------------------------
void setPixel(int x, int y, uint16_t color);
void drawRectDMA(int x, int y, int width, int height, volatile uint16_t color);
void drawFullScreenImageDMA(const uint16_t *image);
void drawImageDMA(int x, int y, int width, int height, const uint16_t *image);
void drawPortionImage(int x, int y, int width, int height, const uint16_t *image);
void fillScreenDMA(volatile uint16_t color);
void drawChar(int col, int row, char ch, uint16_t color);
void drawString(int col, int row, char *str, uint16_t color);
void drawCenteredString(int col, int row, int width, int height, char *str, uint16_t color);

/** Contains the pixels of each character from a 6x8 font */
// This is in the font.c file. You can replace the font if you want.
extern const unsigned char fontdata_6x8[12288];

#endif

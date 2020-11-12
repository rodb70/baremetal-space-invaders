#include <stdint.h>
#include "hardware.h"
#include "game.h"

void drawChar(int col, int row, char ch, uint16_t color)
{
    for( int c = 0; c < 8; c++ )
    {
        for( int r = 0; r < 6; r++ )
        {
            if( fontdata_6x8[OFFSET(r, c, 6) + ch * 48] )
            {
                setPixel( row + r, col + c, color );
            }
        }
    }
}

void drawString(int col, int row, char *str, uint16_t color)
{
    while( *str )
    {
        drawChar( col, row, *str++, color );
        row += 6;
    }
}

void drawCenteredString(int x, int y, int width, int height, char *str, uint16_t color)
{
    uint32_t len = 0;
    char *strCpy = str;
    while( *strCpy )
    {
        len++;
        strCpy++;
    }

    uint32_t strWidth = 6 * len;
    uint32_t strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString( col, row, str, color );
}

#include "game.h"

volatile unsigned short *videoBuffer = (volatile unsigned short*) 0x6000000;
uint32_t vBlankCounter = 0;

void waitForVBlank(void)
{
    // TA-TODO: IMPLEMENT
    // Write a while loop that loops until we're NOT in vBlank anymore:
    // (This prevents counting one VBlank more than once if your app is too fast)
    while( *SCANLINECOUNTER > 160 )
        ;

    // Write a while loop that keeps going until we're in vBlank:
    while( *SCANLINECOUNTER < 160 )
        ;

    // Finally, increment the vBlank counter:
    vBlankCounter++;
}

static int __qran_seed = 42;
static int qran(void)
{
    __qran_seed = 1664525 * __qran_seed + 1013904223;
    return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max)
{
    return (qran() * (max - min) >> 15) + min;
}

void setPixel(int x, int y, uint16_t color)
{
    videoBuffer[OFFSET( y, x, WIDTH )] = color;
}

void drawRectDMA(int x, int y, int width, int height, volatile uint16_t color)
{
    for( int i = 0; i < height; i++ )
    {
        DMA[3].src = &color;
        DMA[3].dst = videoBuffer + OFFSET( i + y, x, WIDTH );
        DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | width;
    }
}

void drawFullScreenImageDMA(const uint16_t *image)
{
    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | (WIDTH * HEIGHT);
}

void drawImageDMA(int x, int y, int width, int height, const uint16_t *image)
{
    for( int i = 0; i < height; i++ )
    {
        if( y + i >= 0 && y + i < HEIGHT )
        {
            DMA[3].src = image + OFFSET( i, 0, width );
            DMA[3].dst = videoBuffer + OFFSET( i + y, x, WIDTH );
            DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | width;
        }
    }
}

void drawPortionImage(int x, int y, int width, int height, const uint16_t *image)
{
    for( int i = 0; i < height; i++ )
    {
        DMA[3].src = image + OFFSET( i + y, x, WIDTH );
        DMA[3].dst = videoBuffer + OFFSET( i + y, x, WIDTH );
        DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | width;
    }
}

void fillScreenDMA(volatile uint16_t color)
{
    DMA[3].src = &color;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | (WIDTH * HEIGHT);
}


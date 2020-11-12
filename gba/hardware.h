/*
 * hardware.h
 *
 *  Created on: 12 Nov 2020
 *      Author: rod
 */

#ifndef GBA_HARDWARE_H_
#define GBA_HARDWARE_H_
#include <stdint.h>

#define BUTTONS *(volatile uint32_t *) 0x4000130

#define REG_DISPCNT  *(volatile unsigned short *) 0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

// This is initialized in gba.c
extern volatile uint16_t *videoBuffer;

// -------------------------------------------------------------------------
//                       DMA
// ---------------------------------------------------------------------------
typedef struct
{
    const volatile void *src;
    const volatile void *dst;
    uint32_t             cnt;

} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

// ---------------------------------------------------------------------------
//                       VBLANK
// ---------------------------------------------------------------------------
#define SCANLINECOUNTER (volatile unsigned short *)0x4000006

// Use this variable to count vBlanks. Initialized in gba.c and to be
// manipulated by waitForVBlank()
extern uint32_t vBlankCounter;

/**
 * Runs a blocking loop until the start of next VBlank.
 */
void waitForVBlank(void);



static inline void hardware_video_init( void )
{
    // (Complete) TA-TODO: Manipulate REG_DISPCNT here to set Mode 3.
    REG_DISPCNT = MODE3 | BG2_ENABLE;
}

#endif /* GBA_HARDWARE_H_ */

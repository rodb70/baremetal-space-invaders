#ifndef HOST_HARDWARE_H_
#define HOST_HARDWARE_H_
#include <stdbool.h>
#include <stdint.h>

// Use this variable to count vBlanks. Initialized in gba.c and to be
// manipulated by waitForVBlank()
extern uint32_t vBlankCounter;

extern uint16_t *videoBuffer;
uint32_t button_get( void );

#define BUTTONS button_get()

static inline void hardware_video_init( void )
{
    static bool inited = false;
    extern volatile uint32_t* frame_buffer_init(void);

    if( false == inited )
    {
        (void)frame_buffer_init();
        inited = true;
    }
}

void waitForVBlank(void);

#endif /* HOST_HARDWARE_H_ */

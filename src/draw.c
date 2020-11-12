#include "draw.h"
#include "player.h"
#include "enemy.h"
#include "screens.h"
#include <string.h>
#include <stdio.h>
#include "game.h"

// TA-TODO: Include any header files for images generated by nin10kit.
// Example for the provided garbage image:
// #include "images/garbage.h"

// TA-TODO: Add any draw/undraw functions for sub-elements of your app here.
// For example, for a snake game, you could have a drawSnake function
// or a drawFood function
//
// e.g.:
// static void drawSnake(Snake* snake);
// static void drawFood(Food* food);

// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state)
{
    // TA-TODO: IMPLEMENT.
    drawFullScreenImageDMA( space );
    drawAppState( state );
}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state)
{
    // TA-TODO: IMPLEMENT.
    // UNUSED(state);
    Player player = state->p;
    drawPortionImage( 0, 0, strlen( player.life_representation ) * 6, 8, space );
    drawPortionImage( WIDTH - 6 * 7, 0, 6 * 7, 8, space );
    for( int i = 0; i < player.num_bullets; i++ )
    {
        Projectile b = player.bullets[i];
        drawPortionImage( b.x, b.y, b.width, b.height, space );
    }
    // Enemy enemies[15];
    // memcpy(enemies, state->enemies, sizeof(state->enemies));
    for( int i = 0; i < state->num_enemies; i++ )
    {
        Enemy e = state->enemies[i];
        if( !e.defeated )
        {
            drawPortionImage( e.x, e.y, e.width, e.height, space );
        }

    }
    for( int j = 0; j < state->num_bombs; j++ )
    {
        Projectile b = state->bombs[j];
        drawPortionImage( b.x, b.y, b.width, b.height, space );
    }
    drawPortionImage( player.x, player.y, player.width, player.height, space );
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state)
{
    // TA-TODO: IMPLEMENT.
    Player player = state->p;
    drawString( 0, 0, player.life_representation, BLUE );
    drawString( WIDTH - 7 * 6, 0, state->level_representation, BLUE );
    for( int i = 0; i < player.num_bullets; i++ )
    {
        Projectile b = player.bullets[i];
        drawImageDMA( b.x, b.y, b.width, b.height, bullet );
    }
    for( int i = 0; i < state->num_enemies; i++ )
    {
        Enemy e = state->enemies[i];
        if( !e.defeated )
        {
            // const uint16_t *image = stage1;
            const uint16_t *image;
            if( e.y >= 90 )
            {
                image = stage3;
            }
            else if( e.y >= 50 )
            {
                image = stage2;
            }
            else
            {
                image = stage1;
            }
            drawImageDMA( e.x, e.y, e.width, e.height, image );
        }
    }
    for( int j = 0; j < state->num_bombs; j++ )
    {
        Projectile b = state->bombs[j];
        drawImageDMA( b.x, b.y, b.width, b.height, bomb );
    }
    drawImageDMA( player.x, player.y, player.width, player.height, ship );
}

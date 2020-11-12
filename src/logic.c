#include "logic.h"
#include "player.h"
#include "enemy.h"
#include <string.h>
#include <stdio.h>
#include "game.h"

void initializeAppState(AppState *appState)
{
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    appState->gameOver = 0;

    appState->p.x = WIDTH / 2 - SHIP_WIDTH / 2;
    appState->p.y = (HEIGHT - SHIP_HEIGHT - 10);
    appState->p.vx = 0;
    appState->p.width = SHIP_WIDTH;
    appState->p.height = SHIP_HEIGHT;
    appState->p.lives = 3;
    strcpy( appState->p.life_representation, "Lives: 3" );
    appState->p.num_bullets = 0/*1*/;
    appState->p.bulletTimer = 0;
    // appState->p.bullets[0].x = 50;
    // appState->p.bullets[0].y = 100;
    // appState->p.bullets[0].vy = -1;
    // appState->p.bullets[0].width = BULLET_WIDTH;
    // appState->p.bullets[0].height = BULLET_HEIGHT;
    addEnemies( appState );
    appState->evx = 1;

    appState->num_bombs = 0;

    appState->level = 1;
    strcpy( appState->level_representation, "Level 1" );

}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, uint32_t keysPressedBefore, uint32_t keysPressedNow)
{
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    AppState nextAppState = *currentAppState;

    Player player = nextAppState.p;
    Enemy enemies[10];
    memcpy( enemies, nextAppState.enemies, sizeof(nextAppState.enemies) );
    int num_enemies = nextAppState.num_enemies;
    Projectile bombs[10];
    memcpy( bombs, nextAppState.bombs, sizeof(nextAppState.bombs) );
    int num_bombs = nextAppState.num_bombs;
    //Player movement and bullet addition
    if( KEY_DOWN( BUTTON_LEFT, keysPressedNow ) )
    {
        player.vx = -1;
    }
    else if( KEY_DOWN( BUTTON_RIGHT, keysPressedNow ) )
    {
        player.vx = 1;
    }
    else
    {
        player.vx = 0;
    }
    if( KEY_DOWN(BUTTON_UP, keysPressedNow) && player.bulletTimer == 0 )
    {
        addBullet( &player );
        player.bulletTimer = 45;
    }
    if( player.bulletTimer > 0 )
    {
        player.bulletTimer--;
    }
    player.x += player.vx;
    if( player.x <= 0 )
    {
        player.x = 0;
    }
    if(( player.x + player.width ) >= WIDTH )
    {
        player.x = WIDTH - player.width;
    }

    //Bullet movement and removal
    for( int j = 0; j < player.num_bullets; j++ )
    {
        Projectile b = player.bullets[j];
        player.bullets[j].y += b.vy;
        //Bullet-Top of Screen Collision
        if( player.bullets[j].y < 0 )
        {
            removeBullet( &player, j );
        }
        //Enemy-Bullet Collision
        for( int m = 0; m < num_enemies; m++ )
        {
            Enemy e = enemies[m];
            if( !e.defeated && !b.removed && enemyBulletCollision( e, b ) )
            {
                removeBullet( &player, j );
                b.removed = 1;
                removeEnemy( &nextAppState, m );
                num_enemies--;
                // nextAppState.enemies[m].defeated = 1;
            }
        }
    }
    memcpy( enemies, nextAppState.enemies, sizeof(nextAppState.enemies) );
    //Adds a bomb from a random enemy
    if( vBlankCounter % 120 == 0 )
    {
        int enemyI = randint( 0, num_enemies );
        addBomb( &nextAppState, enemies[enemyI] );
    }

    if( vBlankCounter % 2 == 0 )
    { //slows down enemy movement
        //Enemy loop
        int movedown = 0;
        for( int n = 0; n < num_enemies; n++ )
        {
            Enemy e = enemies[n];
            //Check to see if any enemeies hit a wall before movement
            if( !e.defeated  &&
                ((e.x < 0 && nextAppState.evx < 0) || (e.x + e.width > WIDTH && nextAppState.evx > 0)) )
            {
                nextAppState.evx = -nextAppState.evx;
                movedown = 1;
            }
            if( e.y + e.height >= HEIGHT - SHIP_HEIGHT - 10 )
            {
                nextAppState.gameOver = 1;
            }
        }
        for( int p = 0; p < num_enemies; p++ )
        {
            nextAppState.enemies[p].x += nextAppState.evx;
            if( movedown )
            {
                nextAppState.enemies[p].y += 20;
            }
        }
    }
    for( int o = 0; o < num_bombs; o++ )
    {
        Projectile b = bombs[o];
        if( playerBombCollision( player, b ) )
        {
            player.lives--;
            sprintf( player.life_representation, "Lives: %d", player.lives );
            removeBomb( &nextAppState, o );
        }
        if( b.y > ( HEIGHT - b.height ))
        {
            removeBomb( &nextAppState, o );
        }
        nextAppState.bombs[o].y += b.vy;
    }
    //transitions
    if( num_enemies == 0 )
    {
        nextAppState.evx = ++nextAppState.level;
        sprintf( nextAppState.level_representation, "Level %d", nextAppState.level );
        addEnemies( &nextAppState );
        nextAppState.num_enemies = 10;
        nextAppState.num_bombs = 0;
        player.x = WIDTH / 2 - SHIP_WIDTH / 2;
    }
    if( player.lives < 1 )
    {
        nextAppState.gameOver = 1; //lose
    }
    if( nextAppState.level == 4 )
    {
        nextAppState.gameOver = 2; //win
    }
    nextAppState.p = player;
    UNUSED( keysPressedBefore );
    return nextAppState;
}

//Utility Functions
void addEnemies(AppState *appState)
{
    int enemySpacing = 5;
    int i = 0;
    for( int x = 0; x < 5; x++ )
    {
        int actualx = (x * (STAGE1_WIDTH + enemySpacing)) + 20;
        for( int y = 0; y < 2; y++ )
        {
            int actualy = (y * (STAGE1_HEIGHT + enemySpacing)) + 10;
            appState->enemies[i].x = actualx;
            appState->enemies[i].y = actualy;
            appState->enemies[i].vx = 0;
            appState->enemies[i].width = STAGE1_WIDTH;
            appState->enemies[i].height = STAGE1_HEIGHT;
            appState->enemies[i].defeated = 0;
            i++;
        }
    }
    appState->num_enemies = 10;
}
void removeEnemy(AppState *appState, int i)
{
    for( int j = i; j < appState->num_enemies; j++ )
    {
        appState->enemies[j] = appState->enemies[j + 1];
    }
    appState->num_enemies--;
}

void addBullet(Player *player)
{
    player->bullets[player->num_bullets].width = BULLET_WIDTH;
    player->bullets[player->num_bullets].height = BULLET_HEIGHT;
    player->bullets[player->num_bullets].x = player->x + player->width / 2 - BULLET_WIDTH / 2;
    player->bullets[player->num_bullets].y = player->y - BULLET_HEIGHT;
    player->bullets[player->num_bullets].vy = -1;
    player->bullets[player->num_bullets].removed = 0;
    player->num_bullets++;
}
void removeBullet(Player *player, int i)
{
    for( int k = i; k < player->num_bullets; k++ )
    {
        player->bullets[k] = player->bullets[k + 1];
    }
    player->num_bullets--;
}

void addBomb(AppState *appState, Enemy e)
{
    appState->bombs[appState->num_bombs].width = BOMB_WIDTH;
    appState->bombs[appState->num_bombs].height = BOMB_HEIGHT;
    appState->bombs[appState->num_bombs].x = e.x + e.width / 2 - BOMB_WIDTH / 2;
    appState->bombs[appState->num_bombs].y = e.y + e.height;
    appState->bombs[appState->num_bombs].vy = 1;
    appState->bombs[appState->num_bombs].removed = 0;
    appState->num_bombs++;
}
void removeBomb(AppState *appState, int i)
{
    for( int k = i; k < appState->num_bombs; k++ )
    {
        appState->bombs[k] = appState->bombs[k + 1];
    }
    appState->num_bombs--;
}

int rectangleCollision(int ax, int ay, int awidth, int aheight, int bx, int by, int bwidth, int bheight)
{
    if( (ax + awidth > bx) && (ax < bx + bwidth) && (ay + aheight > by) && (ay < by + bheight) )
    {
        return 1;
    }
    return 0;
}
int playerBombCollision(Player p, Projectile bomb)
{
    return rectangleCollision( p.x, p.y, p.width, p.height, bomb.x, bomb.y, bomb.width, bomb.height );
}
int enemyBulletCollision(Enemy e, Projectile bullet)
{
    return rectangleCollision( e.x, e.y, e.width, e.height, bullet.x, bullet.y, bullet.width, bullet.height );
}

// void updateCheat(AppState *appState, char c) {
//     if (strlen(appState->cheatCode) >= 5) {
//         char updated[6];
//         memcpy(updated, &(appState->cheatCode + 1), 4);
//         updated[4] = c;
//         updated[5] = '\0';
//     } else {
//         appState -> cheatCode += c;
//     }

// }

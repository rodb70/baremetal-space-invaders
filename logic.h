#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int vy;
    int removed;
} Projectile;
typedef struct {
    int x;
    int y;
    int vx;
    int width;
    int height;
    int lives;
    char life_representation[15];
    Projectile bullets[20];
    int num_bullets;
    int bulletTimer;
} Player;
typedef struct {
    int x;
    int y;
    int vx;
    int width;
    int height;
    int defeated;
} Enemy;
typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    Player p;
    Enemy enemies[10];
    int num_enemies;
    //Enemy velocity
    int evx;
    Projectile bombs[10];
    int num_bombs;
    int level;
    char level_representation[8];
    char *cheatCode;
    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */

} AppState;


/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

void addEnemies(AppState *appState);
void removeEnemy(AppState *appState, int i);

void addBullet(Player *player);
void removeBullet(Player *player, int i);

void addBomb(AppState *appState, Enemy e);
void removeBomb(AppState *appState, int i);

int rectangleCollision(int ax, int ay, int awidth, int aheight, int bx, int by, int bwidth, int bheight);
int playerBombCollision(Player p, Projectile bomb);
int enemyBulletCollision(Enemy e, Projectile bullet);

void updateCheat(AppState *appState, char c);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif

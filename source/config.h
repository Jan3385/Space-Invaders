#ifndef CONFIG
#define CONFIG

#include <assert.h>
#include <stdint.h>

#define PROJECT_NAME "Space Invaders!"

// Game Parameters
#define GAME_LIVES    3 // MAX 9
#define PLAYER_SPEED  2
#define PLAYER_SHOOT_COOLDOWN 55  // MAX <uint8
#define PLAYER_PROJECTILE_SPEED 4 // MAX <uint8
#define MOTHERSHIP_SPAWN_ROLL 2000
#define MOTHERSHIP_SPEED 2
#define ENEMY_PROJECTILE_SPEED  3 // MAX <uint8
#define ENEMY_WAIT_MAX 25
#define ENEMY_WAIT_MIN 8
#define SCORE_ALIEN_A 40
#define SCORE_ALIEN_B 20
#define SCORE_ALIEN_C 10
#define SCORE_ALIEN_MOTHERSHIP 150
#define ALIENS_WIDTH  11 // MAX uint8
#define ALIENS_HEIGHT 5  // MAX uint8
#define ALIENS_GAP_X  14
#define ALIENS_GAP_Y  10

// Colors (uint8 r, g, b, a)
#define BACKGROUND_COLOR   0,   0,   0, 255
#define LINE_COLOR        32, 255,  32, 255

// Colors (uint32_t packed argb)
#define BUNKER_PIXEL_COLOR 0xFF20FF20
#define BUNKER_EMPTY_COLOR 0x00000000

// Original Space Invaders resolution
#define WINDOW_WIDTH  224
#define WINDOW_HEIGHT 256
#define WINDOW_SCALE  3

// Misc
#define USED_FONT "assets/PixelFont2.ttf"
#define SAVE_FILE "SaveScore.txt"

// Bunker
#define BUNKER_ARR "\
0000111111111111110000\
0001111111111111111000\
0011111111111111111100\
0111111111111111111110\
1111111111111111111111\
1111111111111111111111\
1111111111111111111111\
1111111111111111111111\
1111111111111111111111\
1111111111111111111111\
1111111111111111111111\
1111111111111111111111\
1111111100000011111111\
1111111000000001111111\
1111110000000000111111\
1111110000000000111111"

#define BUNKER_WIDTH  22
#define BUNKER_HEIGHT (sizeof(BUNKER_ARR)-1) / BUNKER_WIDTH
#define BUNKER_COUNT  4   // MAX < uint8
#define BUNKER_EXPLOSION_SIZE 5 // MAX < int8, MIN >= 0

// Textures
#define SPACEBAR_TEXTURE "assets/Spacebar.png"
#define PLAYER_TEXTURE "assets/Player.png"
#define PLAYER_PROJECTILE_TEXTURE "assets/PlayerProjectile.png"
#define ENEMY_PROJECTILE_TEXTURE  "assets/EnemyProjectile.png"
#define ALIENS_TEXTURE_SHEET "assets/Aliens.png"
#define SINGLE_ALIEN_TEXTURE "assets/Alien.png"
#define MOTHERSHIP_TEXTURE "assets/Mothership.png"
#define ALIEN_TEXTURE_A1 { 0,  0,  8,  8}
#define ALIEN_TEXTURE_A2 { 9,  0,  9,  9}
#define ALIEN_TEXTURE_B1 { 0,  9, 11,  8}
#define ALIEN_TEXTURE_B2 {12,  9, 11,  8}
#define ALIEN_TEXTURE_C1 { 0, 18, 12,  8}
#define ALIEN_TEXTURE_C2 {13, 18, 12,  8}

// Asserts for invalid configs
static_assert(GAME_LIVES <= 9, "Game can have maximum of 9 lives!");
static_assert(PLAYER_SHOOT_COOLDOWN < UINT8_MAX, "Shooting cooldown has to be lower than 8b unsigned int max!");
static_assert(PLAYER_PROJECTILE_SPEED < UINT8_MAX && ENEMY_PROJECTILE_SPEED < UINT8_MAX, "Projectile speed is limited to below 8b unsigned int max!");
static_assert(ALIENS_WIDTH <= UINT8_MAX && ALIENS_HEIGHT <= UINT8_MAX, "Alien size has to be below or equal 8b unsigned int max!");
static_assert(sizeof(BUNKER_ARR) - 1 == BUNKER_WIDTH * BUNKER_HEIGHT, "Bunker sizes have to match the bunker array!");
static_assert(BUNKER_COUNT < UINT8_MAX, "Bunker count has to be below 8b unsigned int max!");
static_assert(BUNKER_EXPLOSION_SIZE < INT8_MAX && BUNKER_EXPLOSION_SIZE >= 0, "Bunker explosion has to be below 8b signed int max! and above 0");

#endif
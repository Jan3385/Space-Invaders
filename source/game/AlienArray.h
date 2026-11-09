#ifndef ALIEN_ARRAY
#define ALIEN_ARRAY

#include <stdbool.h>
#include <stdint.h>

#ifdef WIN32
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#include "config.h"
#include "math/Vector.h"
#include "game/Projectile.h"

typedef struct Game Game;
typedef struct Renderer Renderer;
typedef struct Texture Texture;

typedef struct AlienArray{
  bool isAlive[ALIENS_WIDTH][ALIENS_HEIGHT];
  Vec2i position;
  Texture *spritesheet;
  Texture *projectileTexture;
  Projectile *eProjectile;
  uint16_t livingAliens;
  int8_t movement;
  uint8_t deadRowStart;
  uint8_t deadRowEnd;
  bool secondaryTexture;
} AlienArray;

AlienArray *CreateAlienArray(Renderer *renderer);
void DestroyAlienArray(AlienArray *array);

Vec2i AlienArray_RandomAlienPosBottom(AlienArray *array);

void AlienArray_Move(AlienArray *array);
void AlienArray_CheckStartEnd(AlienArray *array);
bool AlienArray_ProjectileCollision(AlienArray *array, Projectile *proj, Game *game);

/// @brief Returns a source rect for the correct alien on the aliens spritesheet 
/// @param y Height of the alien in array (0 is on the top)
/// @param secondaryTexture Decides if texture 1 or 2 should be used
/// @return SDL_Rect to use as source for alien texture spritesheet
SDL_Rect AlienArray_GetSpritesheetRect(int y, bool secondaryTexture);

#endif
#ifndef BUNKER_ARRAY
#define BUNKER_ARRAY

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

typedef struct Bunker{
  bool blocks[BUNKER_WIDTH][BUNKER_HEIGHT];
  Vec2i pos;
  Texture *texture;
} Bunker;

typedef struct BunkerArray {
  Bunker *bunkers[BUNKER_COUNT];
  uint16_t yOffset;
} BunkerArray;

BunkerArray *CreateBunkerArray(uint16_t yOffset, Renderer *renderer);
void DestroyBunkerArray(BunkerArray *array);

bool BunkerArray_ProjectileCollision(BunkerArray *array, Projectile *proj, Game *game);

Bunker *CreateBunker(Vec2i pos, Renderer *renderer);
void DestroyBunker(Bunker *bunker);

void Bunker_UpdateTexture(Bunker *bunker);
bool Bunker_ProjectileCollision(Bunker *bunker, Projectile *proj, Game *game, uint16_t yOffset);
void Bunker_ExplodeAt(Bunker *bunker, Vec2i localPos);

#endif
#ifndef MOTHERSHIP
#define MOTHERSHIP

#include <stdbool.h>

#ifdef WIN32
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#include "config.h"
#include "math/Vector.h"
#include "game/Projectile.h"

typedef struct Renderer Renderer;
typedef struct Game Game;
typedef struct Texture Texture;

typedef struct Mothership{
  Vec2i pos;
  Texture *texture;
} Mothership;

Mothership *CreateMothership(Renderer *renderer);
void DestroyMothership(Mothership *ship);

/// @brief Moves the mothership positin
/// @return 'false' if mothership in bounds 'true' if out of bounds
bool Mothership_Move(Mothership *ship);

bool Mothership_ProjectileCollision(Mothership *ship, Projectile *proj, Game *game);


#endif
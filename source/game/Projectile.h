#ifndef PROJECTILE
#define PROJECTILE

#include <stdbool.h>

#ifdef WIN32
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#include "math/Vector.h"

typedef struct Texture Texture;

typedef struct Projectile {
  // Texture lifetime is not managed by the projectile
  Texture *texture;

  Vec2i pos;
  int velocity, lifetime;
  bool isFriendly;
} Projectile;

Projectile *CreateProjectile(Vec2i pos, int velocity, bool isFriendly, Texture *texture);
void DestroyProjectile(Projectile *projectile);

void Projectile_Move(Projectile *projectile);
bool Projectile_isOOB(const Projectile *projectile);
bool Projectile_isCollidingRect(const Projectile *projectile, SDL_Rect area);

#endif
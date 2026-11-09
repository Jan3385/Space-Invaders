#ifndef PLAYER
#define PLAYER

#include <stdint.h>
#include <stdbool.h>

#include "math/Vector.h"
#include "game/Projectile.h"

typedef struct Texture Texture;
typedef struct Renderer Renderer;
typedef struct Game Game;

typedef struct Player {
  Vec2i pos;
  Texture *projectileTexture;
  Texture *texture;
  Projectile *pProjectile;
  uint8_t shootCooldown;
} Player;

Player *CreatePlayer(Renderer *renderer);
void DestroyPlayer(Player *player);

void Player_MoveBy(Player *player, int deltaX);
void Player_Update(Player *player);
bool Player_Shoot(Player *player, Game *game);
bool Player_ProjectileCollision(Player *player, Projectile *proj);

#endif
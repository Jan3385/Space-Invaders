#include "Player.h"

#include "render/Renderer.h"
#include "Game.h"
#include "config.h"

Player *CreatePlayer(Renderer *renderer){
  Player *p = (Player*)malloc(sizeof(Player));

  p->texture = Renderer_LoadTexture(renderer, PLAYER_TEXTURE);
  p->projectileTexture = Renderer_LoadTexture(renderer, PLAYER_PROJECTILE_TEXTURE);
  p->pProjectile = NULL;
  
  p->pos = (Vec2i){WINDOW_WIDTH/2, WINDOW_HEIGHT-30};

  p->shootCooldown = PLAYER_SHOOT_COOLDOWN;
}

void DestroyPlayer(Player *player){
  Renderer_UnloadTexture(player->texture);
  if(player->pProjectile) DestroyProjectile(player->pProjectile);
  Renderer_UnloadTexture(player->projectileTexture);

  free(player);
}

void Player_MoveBy(Player *player, int deltaX){
  player->pos.x += deltaX;
  
  // Clamp position to borders of the screen
  player->pos.x = player->pos.x > 0 ? player->pos.x : 0;
  player->pos.x = player->pos.x + player->texture->width < WINDOW_WIDTH ? player->pos.x : WINDOW_WIDTH - player->texture->width;
}

void Player_Update(Player *player){
  if(player->shootCooldown > 0) player->shootCooldown--;
}

bool Player_Shoot(Player *player, Game *game){
  if(player->shootCooldown != 0) return false;

  if(player->pProjectile)
    DestroyProjectile(player->pProjectile);

  player->pProjectile = CreateProjectile(
    AddVec2i(player->pos, (Vec2i){player->texture->width/2, 0}),
    -1,
    true,
    player->projectileTexture
  );

  player->shootCooldown += PLAYER_SHOOT_COOLDOWN;
}

bool Player_ProjectileCollision(Player *player, Projectile *proj){
  SDL_Rect playerRect = {
    player->pos.x,          player->pos.y,
    player->texture->width, player->texture->height
  };

  if(Projectile_isCollidingRect(proj, playerRect)) return true;

  return false;
}
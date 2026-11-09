#include "Mothership.h"

#include "render/Renderer.h"
#include "Game.h"

Mothership *CreateMothership(Renderer *renderer){
  Mothership *ship = (Mothership*)malloc(sizeof(Mothership));

  ship->pos = (Vec2i){WINDOW_WIDTH, 27};
  ship->texture = Renderer_LoadTexture(renderer, MOTHERSHIP_TEXTURE);

  return ship;
}

void DestroyMothership(Mothership *ship){
  Renderer_UnloadTexture(ship->texture);
  free(ship);
}

bool Mothership_Move(Mothership *ship)
{
  ship->pos = AddVec2i(ship->pos, (Vec2i){-MOTHERSHIP_SPEED, 0});

  if(ship->pos.x + ship->texture->width < 0) return true;

  return false;
}

bool Mothership_ProjectileCollision(Mothership *ship, Projectile *proj, Game *game){
  SDL_Rect playerRect = {
    ship->pos.x,          ship->pos.y,
    ship->texture->width, ship->texture->height
  };

  if(Projectile_isCollidingRect(proj, playerRect)) {
    game->score += SCORE_ALIEN_MOTHERSHIP;
    return true;
  }

  return false;
}

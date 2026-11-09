#include "Projectile.h"

#include "render/Renderer.h"
#include "config.h"

Projectile *CreateProjectile(Vec2i pos, int velocity, bool isFriendly, Texture *texture){
  Projectile *p = (Projectile*)malloc(sizeof(Projectile));

  p->pos = pos;
  p->velocity = velocity;
  p->lifetime = 0;
  p->isFriendly = isFriendly;
  p->texture = texture;

  return p;
}

void DestroyProjectile(Projectile *projectile){
  free(projectile);
}

void Projectile_Move(Projectile *projectile){
  projectile->pos.y += projectile->velocity;
  projectile->lifetime++;
}

bool Projectile_isOOB(const Projectile *projectile){
  if(projectile->pos.x < 0) return true;
  if(projectile->pos.x > WINDOW_WIDTH) return true;

  if(projectile->pos.y < 0) return true;
  if(projectile->pos.y > WINDOW_HEIGHT) return true;

  return false;
}

bool Projectile_isCollidingRect(const Projectile *projectile, SDL_Rect area){
  if(projectile->pos.x < area.x) return false;
  if(projectile->pos.x > area.x + area.w) return false;

  if(projectile->pos.y < area.y) return false;
  if(projectile->pos.y > area.y + area.h) return false;

  return true;
}
#include "BunkerArray.h"

#include "render/Renderer.h"
#include "Game.h"

BunkerArray *CreateBunkerArray(uint16_t yOffset, Renderer *renderer){
  BunkerArray *array = (BunkerArray*)malloc(sizeof(BunkerArray));

  array->yOffset = yOffset;

  // Spread bunkers evenly
  uint16_t spacing = (WINDOW_WIDTH - BUNKER_WIDTH * BUNKER_COUNT) / (BUNKER_COUNT+1);

  for (uint8_t i = 0; i < BUNKER_COUNT; i++){
    uint16_t xOffset = (i+1) * spacing + i * BUNKER_WIDTH;

    array->bunkers[i] = CreateBunker((Vec2i){xOffset, 0}, renderer);
  }

  return array;
}

void DestroyBunkerArray(BunkerArray *array){
  for (uint8_t i = 0; i < BUNKER_COUNT; i++){
    DestroyBunker(array->bunkers[i]);
  }
  free(array);
}

bool BunkerArray_ProjectileCollision(BunkerArray *array, Projectile *proj, Game *game){
  for (uint8_t i = 0; i < BUNKER_COUNT; i++){
    if(Bunker_ProjectileCollision(array->bunkers[i], proj, game, array->yOffset))
      return true;
  }
  
  return false;
}

Bunker *CreateBunker(Vec2i pos, Renderer *renderer){
  Bunker *bunker = (Bunker*)malloc(sizeof(Bunker));

  // Load blocks from defined bunker map
  bunker->pos = pos;
  for (uint16_t y = 0; y < BUNKER_HEIGHT; y++)
    for (uint16_t x = 0; x < BUNKER_WIDTH; x++)
      bunker->blocks[x][y] = BUNKER_ARR[x + y * BUNKER_WIDTH] == '0' ? false : true;
  
  bunker->texture = Renderer_LoadTextureEmpty(renderer, BUNKER_WIDTH, BUNKER_HEIGHT);
  Bunker_UpdateTexture(bunker);

  return bunker;
}

void DestroyBunker(Bunker *bunker){
  Renderer_UnloadTexture(bunker->texture);
  free(bunker);
}

void Bunker_UpdateTexture(Bunker *bunker){
  uint32_t pixels[BUNKER_HEIGHT][BUNKER_WIDTH];

  for (uint16_t y = 0; y < BUNKER_HEIGHT; y++)
    for (uint16_t x = 0; x < BUNKER_WIDTH; x++)
      pixels[y][x] = bunker->blocks[x][y] ? BUNKER_PIXEL_COLOR : BUNKER_EMPTY_COLOR;

  SDL_UpdateTexture(bunker->texture->t, NULL, pixels, BUNKER_WIDTH * sizeof(uint32_t));
}

bool Bunker_ProjectileCollision(Bunker *bunker, Projectile *proj, Game *game, uint16_t yOffset){
  SDL_Rect rect = {
    bunker->pos.x, bunker->pos.y + yOffset,
    BUNKER_WIDTH,  BUNKER_HEIGHT
  };
  if(!Projectile_isCollidingRect(proj, rect)) return false;

  Vec2i bunkerRealPos = { bunker->pos.x, bunker->pos.y + yOffset };
  Vec2i localProjPos  = SubtractVec2i(proj->pos, bunkerRealPos);

  if(localProjPos.x < 0 || localProjPos.y < 0) return false;
  if(localProjPos.x >= BUNKER_WIDTH || localProjPos.y >= BUNKER_HEIGHT) return false;

  if(bunker->blocks[localProjPos.x][localProjPos.y]){
    Bunker_ExplodeAt(bunker, localProjPos);

    return true;
  }

  return false;
}

void Bunker_ExplodeAt(Bunker *bunker, Vec2i localPos){
  static const int8_t halfExplosion = BUNKER_EXPLOSION_SIZE / 2;

  for (int8_t y = -halfExplosion; y <= halfExplosion; y++)
    for (int8_t x = -halfExplosion; x <= halfExplosion; x++){{
      Vec2i currPos = AddVec2i(localPos, (Vec2i){x, y});

      if(currPos.x < 0 || currPos.y < 0) continue;
      if(currPos.x >= BUNKER_WIDTH || currPos.y >= BUNKER_HEIGHT) continue;

      float distance = DistanceVec2i(localPos, currPos);

      if(distance < BUNKER_EXPLOSION_SIZE/2.5) bunker->blocks[currPos.x][currPos.y] = false;
      else if (rand()%2 == 0){
        bunker->blocks[currPos.x][currPos.y] = false;
      }
    }
    
  }
  
  bunker->blocks[localPos.x][localPos.y] = false;

  Bunker_UpdateTexture(bunker);
}

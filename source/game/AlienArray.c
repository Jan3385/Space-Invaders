#include "AlienArray.h"

#include "render/Renderer.h"
#include "Game.h"

AlienArray *CreateAlienArray(Renderer *renderer){
  AlienArray *arr = (AlienArray*)malloc(sizeof(AlienArray));
  
  for (int x = 0; x < ALIENS_WIDTH;  x++)
    for (int y = 0; y < ALIENS_HEIGHT; y++)
      arr->isAlive[x][y] = true;
  arr->livingAliens = ALIENS_WIDTH * ALIENS_HEIGHT;

  arr->spritesheet = Renderer_LoadTexture(renderer, ALIENS_TEXTURE_SHEET);
  arr->movement = 1;
  arr->position = (Vec2i){1, 45};
  arr->secondaryTexture = true;

  arr->deadRowStart = 0;
  arr->deadRowEnd   = ALIENS_WIDTH;

  arr->projectileTexture = Renderer_LoadTexture(renderer, ENEMY_PROJECTILE_TEXTURE);
  arr->eProjectile = NULL;
}

void DestroyAlienArray(AlienArray *array){
  Renderer_UnloadTexture(array->spritesheet);

  if(array->eProjectile) DestroyProjectile(array->eProjectile);
  Renderer_UnloadTexture(array->projectileTexture);
  free(array);
}

Vec2i AlienArray_RandomAlienPosBottom(AlienArray *array){
  if(array->livingAliens == 0) return (Vec2i){0,0};

  // Tried to make this faster and better but went clinically insane so this has to suffice
  int x=0, y=0;
  do{
    x = rand()%(array->deadRowEnd - array->deadRowStart) + array->deadRowStart;

    for (uint8_t iy = 1; iy < ALIENS_HEIGHT; iy++)
      if(array->isAlive[x][iy]) y = iy;
    
  } while(!array->isAlive[x][y]);
  

  // Move by actual alien size
  x *= ALIENS_GAP_X;
  y *= ALIENS_GAP_Y;
  
  x += ALIENS_GAP_X/3; 

  Vec2i pos = AddVec2i((Vec2i){x,y}, array->position);

  return pos;
}

bool AlienArray_isOnScreenBorder(AlienArray *array){
  if(array->position.x <= -array->deadRowStart*ALIENS_GAP_X) return true;
  if(array->position.x + ALIENS_GAP_X * ALIENS_WIDTH > WINDOW_WIDTH + (ALIENS_WIDTH-array->deadRowEnd)*ALIENS_GAP_X) return true;

  return false;
}

void AlienArray_Move(AlienArray *array){
  // Check if Out Of Bounds
  if(AlienArray_isOnScreenBorder(array)){
    array->movement = -array->movement;
    array->position.y += 10;
  }

  // Flip texture
  array->secondaryTexture = !array->secondaryTexture;

  array->position = AddVec2i(array->position, (Vec2i){array->movement, 0});

  // Random shooting
  if(!array->eProjectile && (rand() % 10 <= 7)){ // 80% chance if no projectile present
    Vec2i projPos = AlienArray_RandomAlienPosBottom(array);

    array->eProjectile = CreateProjectile(
      projPos,
      1,
      false,
      array->projectileTexture
    );
  }
}

void AlienArray_CheckStartEnd(AlienArray *array){
  // Search for beginning of living aliens
  for (uint8_t x = 0; x < ALIENS_WIDTH; x++){
    bool rowAlive = false;
    for (uint8_t y = 0; y < ALIENS_HEIGHT; y++){
      if(array->isAlive[x][y]) {
        array->deadRowStart = x;
        rowAlive = true;
        break;
      }
    }
    if(rowAlive) break;
  }

  // Search for end of living aliens
  for (uint8_t x = ALIENS_WIDTH-1; x < ALIENS_WIDTH; x--){ // Condition works even when unsigned since underflowing below 0 also triggers the condition
    bool rowAlive = false;
    for (uint8_t y = ALIENS_HEIGHT-1; y < ALIENS_HEIGHT; y--){
      if(array->isAlive[x][y]) {
        array->deadRowEnd = x+1;
        rowAlive = true;
        break;
      }
    }
    if(rowAlive) break;
  }
}

bool AlienArray_ProjectileCollision(AlienArray *array, Projectile *proj, Game *game){
  // Check if inside the alien grid
  SDL_Rect aliens = {
    array->position.x, array->position.y,
    ALIENS_GAP_X * ALIENS_WIDTH, ALIENS_GAP_Y * ALIENS_HEIGHT
  };
  if(!Projectile_isCollidingRect(proj, aliens)) return false;

  // Calculate position inside the grid
  Vec2i normalPos = SubtractVec2i(proj->pos, array->position);
  normalPos.x /= ALIENS_GAP_X;
  normalPos.y /= ALIENS_GAP_Y;

  // Check bounds
  if(normalPos.x < 0 || normalPos.y < 0) return false;
  if(normalPos.x >= ALIENS_WIDTH || normalPos.y >= ALIENS_HEIGHT) return false;
  
  // Kill alien
  if(array->isAlive[normalPos.x][normalPos.y]){
    array->isAlive[normalPos.x][normalPos.y] = false;
    array->livingAliens--;

    // Add score
    if(normalPos.y == 0)      game->score += SCORE_ALIEN_A;
    else if(normalPos.y <= 2) game->score += SCORE_ALIEN_B;
    else                      game->score += SCORE_ALIEN_C;

    AlienArray_CheckStartEnd(array);

    return true;
  }

  return false;
}

SDL_Rect AlienArray_GetSpritesheetRect(int y, bool secondaryTexture){
  if (y == 0)
    return secondaryTexture ? (SDL_Rect)ALIEN_TEXTURE_A1 : (SDL_Rect)ALIEN_TEXTURE_A2;
  else if (y <= 2)
    return secondaryTexture ? (SDL_Rect)ALIEN_TEXTURE_B1 : (SDL_Rect)ALIEN_TEXTURE_B2;
  else
    return secondaryTexture ? (SDL_Rect)ALIEN_TEXTURE_C1 : (SDL_Rect)ALIEN_TEXTURE_C2;
}

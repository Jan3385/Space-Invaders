#include "Renderer.h"

#include "config.h"
#include "Game.h"

void InitSDL(){
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL INIT ERR:\n%s\n", SDL_GetError());
    exit(1);
  }

  if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
    printf("IMG INIT ERR:\n%s\n", IMG_GetError());
    SDL_Quit();
    exit(2);
  }

  if(TTF_Init() != 0){
    printf("TTF INIT ERR:\n%s\n", TTF_GetError());
    IMG_Quit();
    SDL_Quit();
    exit(3);
  }
}

void QuitSDL(){
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

Renderer *CreateRenderer(Window *window)
{
  Renderer *renderer = (Renderer*)malloc(sizeof(Renderer));

  renderer->r = SDL_CreateRenderer(
    window->w,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  renderer->font = TTF_OpenFont(USED_FONT, 8);
  if(renderer->font == NULL){
    SDL_Log("SDL FONT LOAD ERR:\n%s\n", TTF_GetError());
  }

  renderer->spaceBar = Renderer_LoadTexture(renderer, SPACEBAR_TEXTURE);
  renderer->alienTexture = Renderer_LoadTexture(renderer, SINGLE_ALIEN_TEXTURE);

  return renderer;
}

void DestroyRenderer(Renderer *renderer){
  if(renderer == NULL) return;

  SDL_DestroyRenderer(renderer->r);
  TTF_CloseFont(renderer->font);
  Renderer_UnloadTexture(renderer->spaceBar);
  Renderer_UnloadTexture(renderer->alienTexture);

  free(renderer);
}

Texture *Renderer_LoadTexture(Renderer *renderer, const char *file){
  Texture *texture = (Texture*)malloc(sizeof(Texture));
  texture->t = IMG_LoadTexture(renderer->r, file);

  if(!texture->t){
    printf("ERROR LOADING TEXTURE [%s]\n...\n%s\n", file, IMG_GetError());
    return NULL;
  }

  SDL_QueryTexture(texture->t, NULL, NULL, &texture->width, &texture->height);
  
  SDL_SetTextureScaleMode(texture->t, SDL_ScaleModeNearest);
  SDL_SetTextureBlendMode(texture->t, SDL_BLENDMODE_BLEND);

  return texture;
}

Texture *Renderer_LoadTextureEmpty(Renderer *renderer, uint16_t width, uint16_t height){
  Texture *texture = (Texture*)malloc(sizeof(Texture));
  texture->t = SDL_CreateTexture(renderer->r, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

  if(!texture->t){
    printf("ERROR CREATING EMPTY TEXTURE\n...\n", SDL_GetError());
    return NULL;
  }

  texture->width = width;
  texture->height = height;
  
  SDL_SetTextureScaleMode(texture->t, SDL_ScaleModeNearest);
  SDL_SetTextureBlendMode(texture->t, SDL_BLENDMODE_BLEND);

  return texture;
}

void Renderer_UnloadTexture(Texture *texture){
  if(texture == NULL) return;

  SDL_DestroyTexture(texture->t);
  free(texture);
}

void Renderer_RenderProjectile(Renderer *renderer, Projectile *proj){
  if(!proj) return;

  SDL_Rect rect = { 
    proj->pos.x * WINDOW_SCALE,           proj->pos.y * WINDOW_SCALE, 
    proj->texture->width * WINDOW_SCALE,  proj->texture->height * WINDOW_SCALE
  };

  uint8_t flip = proj->lifetime%10 < 5 ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
  SDL_RenderCopyEx(renderer->r, proj->texture->t, NULL, &rect, 0, NULL, flip);
}

void Renderer_RenderPlayer(Renderer *renderer, Player *p){
  SDL_Rect rect = { 
    p->pos.x * WINDOW_SCALE,          p->pos.y * WINDOW_SCALE, 
    p->texture->width * WINDOW_SCALE, p->texture->height * WINDOW_SCALE
  };

  SDL_RenderCopy(renderer->r, p->texture->t, NULL, &rect);

  Renderer_RenderProjectile(renderer, p->pProjectile);
}

void Renderer_RenderAlienArray(Renderer *renderer, AlienArray *array){
  for (int y = 0; y < ALIENS_HEIGHT; y++){
    SDL_Rect src = AlienArray_GetSpritesheetRect(y, array->secondaryTexture);

    const int yGap = y * ALIENS_GAP_Y;
    for (int x = 0; x < ALIENS_WIDTH; x++){
      if(!array->isAlive[x][y]) continue;
      const int xGap = x * ALIENS_GAP_X;

      SDL_Rect rect = {
        (array->position.x + xGap) * WINDOW_SCALE,
        (array->position.y + yGap) * WINDOW_SCALE,
        src.w * WINDOW_SCALE, src.h * WINDOW_SCALE
      };
      SDL_RenderCopy(renderer->r, array->spritesheet->t, &src, &rect);
    }
  }

  Renderer_RenderProjectile(renderer, array->eProjectile);
}

void Renderer_RenderMothership(Renderer *renderer, Mothership *mothership){
  SDL_Rect rect = { 
    mothership->pos.x * WINDOW_SCALE,          mothership->pos.y * WINDOW_SCALE, 
    mothership->texture->width * WINDOW_SCALE, mothership->texture->height * WINDOW_SCALE
  };

  SDL_RenderCopy(renderer->r, mothership->texture->t, NULL, &rect);
}

void Renderer_RenderBunkerArray(Renderer *renderer, BunkerArray *array){
  for (uint8_t i = 0; i < BUNKER_COUNT; i++){
    SDL_Rect rect = {
      array->bunkers[i]->pos.x * WINDOW_SCALE, (array->bunkers[i]->pos.y + array->yOffset) * WINDOW_SCALE,
      BUNKER_WIDTH * WINDOW_SCALE,             BUNKER_HEIGHT * WINDOW_SCALE
    };

    SDL_RenderCopy(renderer->r, array->bunkers[i]->texture->t, NULL, &rect);
  }
}

void Renderer_RenderText(Renderer *renderer, const char *str, Vec2i pos){
  SDL_Surface *s = TTF_RenderUTF8_Solid(
    renderer->font,
    str,
    (SDL_Color){255, 255, 255}
  );

  SDL_Texture *t = SDL_CreateTextureFromSurface(
    renderer->r,
    s
  );
  
  int tW=0,tH=0;
  SDL_QueryTexture(t, NULL, NULL, &tW, &tH);
  SDL_SetTextureScaleMode(t, SDL_ScaleModeNearest);

  SDL_Rect msgRect = {
    pos.x * WINDOW_SCALE, pos.y * WINDOW_SCALE,
    tW     * WINDOW_SCALE, tH     * WINDOW_SCALE
  };

  SDL_RenderCopy(renderer->r, t, NULL, &msgRect);

  SDL_FreeSurface(s);
  SDL_DestroyTexture(t);
}

void Renderer_RenderTextCentered(Renderer *renderer, const char *str, Vec2i pos){
  SDL_Surface *s = TTF_RenderUTF8_Solid(
    renderer->font,
    str,
    (SDL_Color){255, 255, 255}
  );

  SDL_Texture *t = SDL_CreateTextureFromSurface(
    renderer->r,
    s
  );
  
  int tW=0,tH=0;
  SDL_QueryTexture(t, NULL, NULL, &tW, &tH);
  SDL_SetTextureScaleMode(t, SDL_ScaleModeNearest);

  SDL_Rect msgRect = {
    (pos.x - tW/2) * WINDOW_SCALE, (pos.y - tH/2) * WINDOW_SCALE,
    tW    * WINDOW_SCALE,          tH    * WINDOW_SCALE
  };

  SDL_RenderCopy(renderer->r, t, NULL, &msgRect);

  SDL_FreeSurface(s);
  SDL_DestroyTexture(t);
}

void Renderer_RenderLine(Renderer *renderer, int yOffset, uint8_t height){
  SDL_SetRenderDrawColor(renderer->r, LINE_COLOR);

  SDL_Rect rect = { 
    0,                         yOffset * WINDOW_SCALE, 
    WINDOW_WIDTH*WINDOW_SCALE, WINDOW_SCALE * height
  };

  SDL_RenderFillRect(renderer->r, &rect);

  SDL_SetRenderDrawColor(renderer->r, BACKGROUND_COLOR);
}

void Renderer_RenderLineProgress(Renderer *renderer, int yOffset, uint8_t height, float progress){
  SDL_SetRenderDrawColor(renderer->r, LINE_COLOR);

  SDL_Rect rect = { 
    0,                                  yOffset * WINDOW_SCALE, 
    WINDOW_WIDTH*WINDOW_SCALE*progress, WINDOW_SCALE * height
  };

  SDL_RenderFillRect(renderer->r, &rect);

  SDL_SetRenderDrawColor(renderer->r, BACKGROUND_COLOR);
}

void Renderer_RenderTexture(Renderer *renderer, Texture *texture, Vec2i pos, uint8_t scale)
{
  SDL_Rect rect = { 
    (pos.x-(texture->width*scale)/2) * WINDOW_SCALE,  (pos.y-(texture->height*scale)/2) * WINDOW_SCALE, 
    texture->width * WINDOW_SCALE * scale,            texture->height * WINDOW_SCALE * scale
  };

  SDL_RenderCopy(renderer->r, texture->t, NULL, &rect);
}

void Renderer_RenderMenu(Renderer *renderer, uint16_t highScore){

  Renderer_RenderTextCentered(renderer, PROJECT_NAME, (Vec2i){WINDOW_WIDTH/2, 30});

  char bestScoreText[20]; // "Best Score: " (7) + '-' + "######" (6) + '\0'
  sprintf(bestScoreText, "Best Score: %d", highScore%100000);
  Renderer_RenderTextCentered(renderer, bestScoreText, (Vec2i){WINDOW_WIDTH/2, WINDOW_HEIGHT/2-53});

  Renderer_RenderTexture(renderer, renderer->alienTexture, (Vec2i){WINDOW_WIDTH/2, WINDOW_HEIGHT/2-20}, 6);

  Renderer_RenderTextCentered(renderer, "Press SPACE to play", (Vec2i){WINDOW_WIDTH/2, WINDOW_HEIGHT-80});
  Renderer_RenderTexture(renderer, renderer->spaceBar, (Vec2i){WINDOW_WIDTH/2, WINDOW_HEIGHT-50}, 1);

  Renderer_RenderTextCentered(renderer, "Controls: Arrow keys+Space", (Vec2i){WINDOW_WIDTH/2, WINDOW_HEIGHT-10});

}

void Renderer_RenderEndScreen(Renderer *renderer, uint16_t score, uint16_t highScore){
  Renderer_RenderTextCentered(renderer, "Game Over!", (Vec2i){WINDOW_WIDTH/2, 30});
  
  char scoreText[15]; // "Score: " (7) + '-' + "######" (6) + '\0'
  sprintf(scoreText, "Score: %d", score%100000);
  Renderer_RenderTextCentered(renderer, scoreText, (Vec2i){WINDOW_WIDTH/2, 65});

  char bestScoreText[20]; // "Best Score: " (7) + '-' + "######" (6) + '\0'
  sprintf(bestScoreText, "Best Score: %d", highScore%100000);
  Renderer_RenderTextCentered(renderer, bestScoreText, (Vec2i){WINDOW_WIDTH/2, 85});

  Renderer_RenderTexture(renderer, renderer->spaceBar, (Vec2i){WINDOW_WIDTH/2, WINDOW_HEIGHT-40}, 1);
}

void Renderer_RenderLives(Renderer *renderer, Game *game){
  if(game->lives > GAME_LIVES) return; // Do not render for invalid lives

  char livesStr[2]; // '0' + '\n'
  sprintf(livesStr, "%d", game->lives%10);
  Renderer_RenderText(renderer, livesStr, (Vec2i){5, WINDOW_HEIGHT - 5 - 7});

  const Vec2i offset = { 
    15, 
    WINDOW_HEIGHT - 5 - game->player->texture->height
  };
  for (uint8_t i = 0; i < game->lives; i++){
    SDL_Rect rect = { 
      (offset.x + i * (game->player->texture->width+2)) * WINDOW_SCALE, 
      offset.y * WINDOW_SCALE, 
      game->player->texture->width  * WINDOW_SCALE, 
      game->player->texture->height * WINDOW_SCALE
    };

    SDL_RenderCopy(renderer->r, game->player->texture->t, NULL, &rect);
  }
  
}
#include "Game.h"

#include "config.h"

Game *CreateGame(Renderer *renderer){
  Game *g = (Game*)malloc(sizeof(Game));

  g->player = CreatePlayer(renderer);
  g->aliens = CreateAlienArray(renderer);
  g->mothership = NULL;
  g->bunkers = CreateBunkerArray(WINDOW_HEIGHT - 55, renderer);

  g->lives = GAME_LIVES;
  g->score = 0;
  g->highScore = Game_GetSaveScore();

  return g;
}

void DestroyGame(Game *game){
  if(game->score > game->highScore){
    Game_SaveScore(game->score);
  }

  DestroyPlayer(game->player);
  DestroyAlienArray(game->aliens);
  
  if(game->mothership) DestroyMothership(game->mothership);

  DestroyBunkerArray(game->bunkers);

  free(game);
}

void Game_Tick(Game *game, Renderer *renderer, Window *window){
  static int frame = 0;
  frame++;

  Player_Update(game->player);

  // Player Projectiles
  if(game->player->pProjectile != NULL){
    for(uint8_t _ = 0; _ < PLAYER_PROJECTILE_SPEED && game->player->pProjectile; _++){
      Projectile_Move(game->player->pProjectile);

      if(AlienArray_ProjectileCollision(game->aliens, game->player->pProjectile, game) || 
        Projectile_isOOB(game->player->pProjectile)
        ){

        DestroyProjectile(game->player->pProjectile);
        game->player->pProjectile = NULL;
      }
      else if(game->mothership && Mothership_ProjectileCollision(game->mothership, game->player->pProjectile, game)){
        DestroyMothership(game->mothership);
        game->mothership = NULL;

        DestroyProjectile(game->player->pProjectile);
        game->player->pProjectile = NULL;

      } else if(BunkerArray_ProjectileCollision(game->bunkers, game->player->pProjectile, game)){
        DestroyProjectile(game->player->pProjectile);
        game->player->pProjectile = NULL;
      }
    }
  }

  // Alien Projectiles
  if(game->aliens->eProjectile != NULL){
    for(uint8_t _ = 0; _ < ENEMY_PROJECTILE_SPEED && game->aliens->eProjectile; _++){
      Projectile_Move(game->aliens->eProjectile);

      if(Projectile_isOOB(game->aliens->eProjectile)){
        DestroyProjectile(game->aliens->eProjectile);
        game->aliens->eProjectile = NULL;
      }
      else if(Player_ProjectileCollision(game->player, game->aliens->eProjectile)){
        DestroyProjectile(game->aliens->eProjectile);
        game->aliens->eProjectile = NULL;

        Game_RemoveLife(game);

      } else if(BunkerArray_ProjectileCollision(game->bunkers, game->aliens->eProjectile, game)){
        DestroyProjectile(game->aliens->eProjectile);
        game->aliens->eProjectile = NULL;
      }
    }
  }

  // Mothership
  if(game->mothership){
    if(Mothership_Move(game->mothership)){
      DestroyMothership(game->mothership);
      game->mothership = NULL;
    }

  } else if (rand()%MOTHERSHIP_SPAWN_ROLL == 0) {
    game->mothership = CreateMothership(renderer);
  }

  // Mouse inputs
  if(Window_KeyDown(window, KEY_LEFT))  Player_MoveBy(game->player, -PLAYER_SPEED);
  if(Window_KeyDown(window, KEY_RIGHT)) Player_MoveBy(game->player,  PLAYER_SPEED);
  if(Window_KeyDown(window, KEY_SPACE)) Player_Shoot (game->player, game);

  // Alien movement
  static const float aliensTotal = ALIENS_WIDTH * ALIENS_HEIGHT;
  int waitTime = ((game->aliens->livingAliens / aliensTotal) * (ENEMY_WAIT_MAX-ENEMY_WAIT_MIN)) + ENEMY_WAIT_MIN;
  if(frame >= waitTime){
    AlienArray_Move(game->aliens);
    frame = 0;
  }
}

void Game_Draw(Game *game, Renderer *renderer){
  Renderer_RenderPlayer(renderer, game->player);
  Renderer_RenderLives(renderer, game);
  
  Renderer_RenderAlienArray(renderer, game->aliens);
  if(game->mothership)
    Renderer_RenderMothership(renderer, game->mothership);

  Renderer_RenderBunkerArray(renderer, game->bunkers);

  // Score
  Renderer_RenderText(renderer, "SCORE", (Vec2i){5, 5});
  
  char scoreStr[8]; // '-' + '000000' + '\0'
  sprintf(scoreStr, "%04d", game->score%1000000); 
  Renderer_RenderText(renderer, scoreStr,(Vec2i){5, 14});
  // ------

  // Best Score
  Renderer_RenderText(renderer, "BST SCR", (Vec2i){60, 5});
  
  char bestScoreStr[8]; // '-' + '000000' + '\0'
  sprintf(bestScoreStr, "%04d", game->highScore%1000000);
  Renderer_RenderText(renderer, bestScoreStr,(Vec2i){60, 14});
  // ------

  // Idk every version of this game has a green line at the bottom that does nothing
  Renderer_RenderLine(renderer, WINDOW_HEIGHT - 16, 1);

  // Very cool cooldown line
  Renderer_RenderLineProgress(renderer, WINDOW_HEIGHT - 18, 2, 1 - game->player->shootCooldown / (float)PLAYER_SHOOT_COOLDOWN);
}

void Game_RemoveLife(Game *game){
  game->lives--;
}

void Game_SaveScore(uint16_t score){
  FILE *f = fopen(SAVE_FILE, "w");
  
  if(!f){
    SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Could not open file '%d'! Score was not saved");
    return;
  }

  fprintf(f, "%d", score);

  fclose(f);
}

uint16_t Game_GetSaveScore(){
  FILE *f = fopen(SAVE_FILE, "r");
  
  if(!f){
    SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Could not open '%d'. This may be because no save was yet created");
    return 0;
  }

  char line[6];
  fgets(line, 6, f);

  fclose(f);

  return atoi(line);
}

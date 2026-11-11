#include "config.h"
#include "render/Renderer.h"
#include "Game.h"

#include <time.h>

void RenderApp(Renderer* renderer, Game *game, GameState gameState, uint16_t score, uint16_t highScore);

int main(int argc, char *argv[])
{
  srand((int)time(NULL));
  InitSDL();

  Window *window = CreateWindow(
    PROJECT_NAME,
    WINDOW_WIDTH, WINDOW_HEIGHT
  );

  Renderer *renderer = CreateRenderer(
    window
  );

  Game *game = NULL;
  uint16_t score = 0;
  uint16_t highScore = Game_GetSaveScore();

  GameState gameState = GameStart;

  SDL_SetRenderDrawColor(renderer->r, BACKGROUND_COLOR);
  
  // Main loop
  while(window->isOpen){
    Window_PollEvents(window);

    if(game) Game_Tick(game, renderer, window);
    else if(Window_KeyDown(window, KEY_SPACE)){
      game = CreateGame(renderer);
      gameState = GamePlay;
      Window_ResetKeyEvents(window);
    }

    if(game && game->lives == 0) {
      Window_ResetKeyEvents(window);

      score = game->score;
      highScore = Game_GetSaveScore();
      DestroyGame(game);
      game = NULL;
      gameState = GameEnd;
    }

    RenderApp(renderer, game, gameState, score, highScore);
  }

  DestroyGame(game);          game     = NULL;
  DestroyRenderer(renderer);  renderer = NULL;
  DestroyWindow(window);      window   = NULL;
  
  QuitSDL();

  return 0;
}

void RenderApp(Renderer* renderer, Game *game, GameState gameState, uint16_t score, uint16_t highScore){
  SDL_RenderClear(renderer->r);

  switch (gameState)
  {
  case GameStart:
    Renderer_RenderMenu(renderer, highScore);
  break;
  case GamePlay:
    if(game) Game_Draw(game, renderer);
    else SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Game state PLAY but no game object is present!");
    break;
  case GameEnd:
    Renderer_RenderEndScreen(renderer, score, highScore);
    break;
  default:
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Invalid GameState present!");
    break;
  }

  SDL_RenderPresent(renderer->r);
}
#include "config.h"
#include "render/Renderer.h"
#include "Game.h"

#include <time.h>

void RenderApp(Renderer* renderer, Game *game, GameState gameState, uint16_t score, Scores highScores);

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
  Scores highScores = { 0 };
  Game_LoadSaveScore(highScores);

  GameState gameState = GameStart;

  SDL_SetRenderDrawColor(renderer->r, BACKGROUND_COLOR);
  
  // Main loop
  while(window->isOpen){
    Window_PollEvents(window);

    if(game) Game_Tick(game, renderer, window);
    else if(Window_KeyDown(window, KEY_SPACE)){
      game = CreateGame(renderer, highScores);
      gameState = GamePlay;
      Window_ResetKeyEvents(window);
    }

    if(game && game->lives == 0) {
      Window_ResetKeyEvents(window);

      score = game->score;
      DestroyGame(game);
      Game_LoadSaveScore(highScores);
      game = NULL;
      gameState = GameEnd;
    }

    RenderApp(renderer, game, gameState, score, highScores);
  }

  if(game) DestroyGame(game); game     = NULL;
  DestroyRenderer(renderer);  renderer = NULL;
  DestroyWindow(window);      window   = NULL;
  
  QuitSDL();

  return 0;
}

void RenderApp(Renderer* renderer, Game *game, GameState gameState, uint16_t score, Scores highScores){
  SDL_RenderClear(renderer->r);

  switch (gameState)
  {
  case GameStart:
    Renderer_RenderMenu(renderer, highScores[0]);
  break;
  case GamePlay:
    if(game) Game_Draw(game, renderer);
    else SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Game state PLAY but no game object is present!");
    break;
  case GameEnd:
    Renderer_RenderEndScreen(renderer, score, highScores);
    break;
  default:
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Invalid GameState present!");
    break;
  }

  SDL_RenderPresent(renderer->r);
}
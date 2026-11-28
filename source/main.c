#include "config.h"
#include "render/Renderer.h"
#include "Audio.h"
#include "Game.h"

#include <time.h>

typedef struct Slider {
  char *name;
  char sValue[8];
  uint8_t value;
} Slider;

// clean main() is just a hoax by big C
int main(int argc, char *argv[])
{
  // /path/to/app -> /path/to
  strrchr(argv[0], '/')[0] = '\0';

  // Basic initialization
  srand((int)time(NULL));
  InitSDL();

  // Init of game structs
  Window *window = CreateWindow(
    PROJECT_NAME,
    WINDOW_WIDTH, WINDOW_HEIGHT,
    2
  );

  Renderer *renderer = CreateRenderer(
    window,
    argv[0],
    2
  );

  Audio *audio = CreateAudio(renderer);

  // Game variables
  Game *game = NULL;
  uint16_t score = 0;
  Scores highScores = { 0 };
  Game_LoadSaveScore(highScores, renderer);
  GameState gameState = GameStart;

  SDL_SetRenderDrawColor(renderer->r, BACKGROUND_COLOR);

  if(audio) Audio_PlayAudio(audio);

  // Game options
  Slider options[3] = {
    (Slider){"Lifes:",    "<  3  >", 3},
    (Slider){"Music:",    "< OFF >", 0},
    (Slider){"W. Scale:", "<  2  >", 2},
  };
  uint8_t selectedOption = 0;
  
  // Main loop -------------------
  while(window->isOpen){
    Window_PollEvents(window);

    // If game is created, run game
    if(game) Game_Tick(game, renderer, window);
    else {
      // Starting the game
      if(Window_KeyDown(window, KEY_SPACE)){
        // If game ended, go back to start screen
        if(gameState == GameEnd) {
          gameState = GameStart;
          Window_ResetKeyEvents(window);
          continue;
        }

        game = CreateGame(renderer, highScores, options[0].value);
        gameState = GamePlay;
      }

      // Menu options
      if(gameState == GameStart){
        // left/right arrows for options
        if(Window_KeyDown(window, KEY_UP))
          selectedOption = selectedOption-1 >= 0 ? selectedOption-1 : 0;
        if(Window_KeyDown(window, KEY_DOWN))
          selectedOption = selectedOption+1 < 3 ? selectedOption+1 : 2;
  
        uint8_t valueChange = 0;
        if(Window_KeyDown(window, KEY_LEFT))  valueChange--;
        if(Window_KeyDown(window, KEY_RIGHT)) valueChange++;
  
        // Logic for changing options values
        if(valueChange != 0){
          switch (selectedOption)
          {
          case 0: // Lifes
            options[0].value += valueChange;
            options[0].value = options[0].value < GAME_LIVES_MIN ? GAME_LIVES_MIN : options[0].value;
            options[0].value = options[0].value > GAME_LIVES_MAX ? GAME_LIVES_MAX : options[0].value;
  
            snprintf(options[0].sValue, 8, "< %2d  >", options[0].value);
            break;
          case 1: // Music ON/OFF
            options[1].value = !options[1].value;
  
            if(options[1].value)
              strcpy(options[1].sValue, "< ON  >");
            else
              strcpy(options[1].sValue, "< OFF >");
  
            Audio_PauseAudio(audio, !options[1].value);
            break;
          case 2: // Window scale
            options[2].value += valueChange;
            options[2].value = options[2].value < WINDOW_SCALE_MIN ? WINDOW_SCALE_MIN : options[2].value;
            options[2].value = options[2].value > WINDOW_SCALE_MAX ? WINDOW_SCALE_MAX : options[2].value;
  
            snprintf(options[2].sValue, 8, "<  %d  >", options[2].value);
            Renderer_SetWindowScale(renderer, window, options[2].value);
            break;
          }
        }
  
        Window_ResetKeyEvents(window);
      }
    }
    // -----------------------------

    // Game over
    if(game && (game->lives == 0 || game->aliens->livingAliens == 0)) {
      Window_ResetKeyEvents(window);

      score = game->score;
      DestroyGame(game, renderer);
      Game_LoadSaveScore(highScores, renderer);
      game = NULL;
      gameState = GameEnd;
    }

    // Rendering ---
    SDL_RenderClear(renderer->r);

    switch (gameState)
    {
    case GameStart:
      Renderer_RenderMenu(renderer, highScores[0]);

      for(uint8_t i = 0; i < 3; i++)
        Renderer_RenderSlider(renderer, 100 + i*20, i == selectedOption, options[i].name, options[i].sValue);
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
    // -------------

    if(audio) Audio_TryLoopAudio(audio);
  }

  // Destructors when app ends
  if(game) 
    DestroyGame(game, renderer); game     = NULL;

  if(audio)
    DestroyAudio(audio);         audio    = NULL;
    
  DestroyRenderer(renderer);     renderer = NULL;
  DestroyWindow(window);         window   = NULL;
  
  QuitSDL();

  return 0;
}
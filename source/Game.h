#ifndef GAME
#define GAME

#include "render/Renderer.h"
#include "game/Player.h"
#include "game/AlienArray.h"
#include "game/Mothership.h"
#include "game/BunkerArray.h"

typedef enum GameState : uint8_t{
  GameStart = 0,
  GamePlay  = 1,
  GameEnd,
} GameState;

typedef uint16_t Scores[5];

typedef struct Game{
  Scores highScores;
  Player *player;
  AlienArray *aliens;
  Mothership *mothership;
  BunkerArray *bunkers;
  uint16_t score;
  uint8_t lives;
} Game;

Game *CreateGame(Renderer *renderer, Scores highScores);
void DestroyGame(Game *game);

void Game_Tick(Game *game, Renderer *renderer, Window *window);
void Game_Draw(Game *game, Renderer *renderer);
void Game_RemoveLife(Game *game);

void Game_SaveScore(Scores highScores, uint16_t score);
void Game_LoadSaveScore(Scores hScores);

#endif
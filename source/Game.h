#ifndef GAME
#define GAME

#include "render/Renderer.h"
#include "game/Player.h"
#include "game/AlienArray.h"
#include "game/Mothership.h"
#include "game/BunkerArray.h"

typedef struct Game{
  Player *player;
  AlienArray *aliens;
  Mothership *mothership;
  BunkerArray *bunkers;
  uint16_t score;
  uint16_t highScore;
  uint8_t lives;
} Game;

Game *CreateGame(Renderer *renderer);
void DestroyGame(Game *game);

void Game_Tick(Game *game, Renderer *renderer, Window *window);
void Game_Draw(Game *game, Renderer *renderer);
void Game_RemoveLife(Game *game);

void Game_SaveScore(uint16_t score);
uint16_t Game_GetSaveScore();

#endif
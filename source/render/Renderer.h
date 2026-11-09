#ifndef RENDERER
#define RENDERER

#ifdef WIN32
#include <SDL_image.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#include "Window.h"
#include "game/Player.h"
#include "game/AlienArray.h"
#include "game/Mothership.h"
#include "game/BunkerArray.h"

typedef struct Texture {
  SDL_Texture *t;
  int width, height;
} Texture;

typedef struct Renderer {
  SDL_Renderer *r;

  TTF_Font *font;
  Texture *spaceBar;
  Texture *alienTexture;
} Renderer;

void InitSDL();
void QuitSDL();

Renderer *CreateRenderer(Window *window);
void DestroyRenderer(Renderer *renderer);

Texture *Renderer_LoadTexture(Renderer *renderer, const char* file);
Texture *Renderer_LoadTextureEmpty(Renderer *renderer, uint16_t width, uint16_t height);
void Renderer_UnloadTexture(Texture *texture);

void Renderer_RenderPlayer(Renderer *renderer, Player *p);
void Renderer_RenderAlienArray(Renderer *renderer, AlienArray *array);
void Renderer_RenderMothership(Renderer *renderer, Mothership *mothership);
void Renderer_RenderBunkerArray(Renderer *renderer, BunkerArray *array);
void Renderer_RenderLives(Renderer *renderer, Game *game);

void Renderer_RenderText(Renderer *renderer, const char* str, Vec2i pos);
void Renderer_RenderTextCentered(Renderer *renderer, const char* str, Vec2i pos);
void Renderer_RenderLine(Renderer *renderer, int yOffset, uint8_t height);

/// @brief Draws a progress line from left to right
/// @param renderer 
/// @param yOffset y offset from top of the screen
/// @param height height of the rendered line
/// @param progress 0.0 - 1.0 progress value
void Renderer_RenderLineProgress(Renderer *renderer, int yOffset, uint8_t height, float progress);
void Renderer_RenderTexture(Renderer *renderer, Texture *texture, Vec2i pos, uint8_t scale);

void Renderer_RenderMenu(Renderer *renderer, uint16_t highScore);
void Renderer_RenderEndScreen(Renderer *renderer, uint16_t score, uint16_t highScore);

#endif
#include "Window.h"
#include "config.h"

Window *CreateWindow(const char *title, int w, int h){
  w *= WINDOW_SCALE;
  h *= WINDOW_SCALE;

  Window *window = (Window*)malloc(sizeof(Window));

  window->w = SDL_CreateWindow(
    title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    w, h,
    SDL_WINDOW_SHOWN // | SDL_WINDOW_ALWAYS_ON_TOP
  );

  for (uint8_t i = 0; i < KEY_EVENT_MAX; i++) window->keysDown[i] = false;
  

  window->isOpen = true;

  return window;
}

void DestroyWindow(Window *window){
  if(!window) return;

  SDL_DestroyWindow(window->w);
  free(window);
}

void Window_PollEvents(Window *window){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    switch (event.type){
    case SDL_QUIT: window->isOpen = false; break;
    case SDL_KEYDOWN: {
      SDL_Keycode key = event.key.keysym.sym;
      if(key == SDLK_LEFT  || key == SDLK_a) window->keysDown[KEY_LEFT]  = true;
      if(key == SDLK_RIGHT || key == SDLK_d) window->keysDown[KEY_RIGHT] = true;
      if(key == SDLK_SPACE) window->keysDown[KEY_SPACE] = true;
    }
      break;
    case SDL_KEYUP:  {
      SDL_Keycode key = event.key.keysym.sym;
      if(key == SDLK_LEFT  || key == SDLK_a) window->keysDown[KEY_LEFT]  = false;
      if(key == SDLK_RIGHT || key == SDLK_d) window->keysDown[KEY_RIGHT] = false;
      if(key == SDLK_SPACE) window->keysDown[KEY_SPACE] = false;
    }
    default:
      break;
    }
  }
}

bool Window_KeyDown(Window *window, WindowKeyEvent e){
  return window->keysDown[e];
}

void Window_ResetKeyEvents(Window *window){
  for (uint8_t i = 0; i < KEY_EVENT_MAX; i++)
    window->keysDown[i] = false;
}

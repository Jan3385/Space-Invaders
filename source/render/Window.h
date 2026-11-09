#ifndef WINDOW
#define WINDOW

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef enum WindowKeyEvent{
  KEY_SPACE = 0,
  KEY_LEFT  = 1,
  KEY_RIGHT,
  KEY_EVENT_MAX
} WindowKeyEvent;

typedef struct Window{
  SDL_Window *w;
  bool isOpen;
  bool keysDown[KEY_EVENT_MAX];
} Window;

Window *CreateWindow(const char *title, int w, int h);
void DestroyWindow(Window *window);

void Window_PollEvents(Window *window);
bool Window_KeyDown(Window *window, WindowKeyEvent e);
void Window_ResetKeyEvents(Window *window);

#endif
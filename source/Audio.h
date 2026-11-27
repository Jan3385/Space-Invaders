#ifndef AUDIO
#define AUDIO

#include <SDL2/SDL.h>

typedef struct Renderer Renderer;

typedef struct Audio{
  uint8_t *buffer;
  uint32_t audioLen;
  SDL_AudioDeviceID deviceID;
  SDL_AudioSpec spec;
} Audio;

Audio *CreateAudio(Renderer *renderer);
void DestroyAudio(Audio *audio);

void Audio_PlayAudio(Audio *audio);
void Audio_TryLoopAudio(Audio *audio);

void Audio_PauseAudio(Audio *audio, bool pause);

#endif
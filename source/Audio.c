#include "Game.h"
#include "Audio.h"

Audio *CreateAudio(Renderer *renderer){
  if(!AUDIO_ENABLE) return NULL;

  Audio *audio = (Audio*)malloc(sizeof(Audio));

  // Load .wav
  if(!SDL_LoadWAV(Renderer_FileGetPathTo(renderer, USED_MUSIC), &audio->spec, &audio->buffer, &audio->audioLen)){
    printf("AUDIO LOAD ERR:\n%s\n", SDL_GetError());
    free(audio);
    return NULL;
  }

  // Set audio loudness
  if(audio->spec.format == AUDIO_U8){
    for(uint32_t i = 0; i < audio->audioLen; i++){
      audio->buffer[i] = (uint8_t)(audio->buffer[i] * MUSIC_LOUDNESS);  // audio sounds wierd when scaled too low idk
    }
  }

  audio->deviceID = 0;
}

void DestroyAudio(Audio *audio){
  if(audio->deviceID != 0)
    SDL_CloseAudioDevice(audio->deviceID);

  SDL_FreeWAV(audio->buffer);
  free(audio);
}

void Audio_PlayAudio(Audio *audio){
  audio->deviceID = SDL_OpenAudioDevice(NULL, 0, &audio->spec, NULL, 0);
  SDL_QueueAudio(audio->deviceID, audio->buffer, audio->audioLen);

  //SDL_PauseAudioDevice(audio->deviceID, 0); // Unpause
}

void Audio_TryLoopAudio(Audio *audio){
  if(SDL_GetQueuedAudioSize(audio->deviceID) < audio->audioLen/10)
    SDL_QueueAudio(audio->deviceID, audio->buffer, audio->audioLen);
}

void Audio_PauseAudio(Audio *audio, bool pause){
  SDL_PauseAudioDevice(audio->deviceID, pause);
}

#ifndef SDLHOST
#define SDLHOST

#include <vector>
#include <algorithm>

#include <SDL.h>
#define SDL_main main
//#undef main


#include "AKUCallbacks.h"


int SdlHost (int argc, char** arg);
void SdlRefreshContext();


#endif
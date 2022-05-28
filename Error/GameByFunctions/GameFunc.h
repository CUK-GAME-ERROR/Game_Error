#pragma once

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"

void InitGame();
void ClearGame();

// Game Phases
const int PHASE_INTRO = 0;
const int PHASE_STAGE1 = 1;
const int PHASE_STAGE2 = 2;
const int PHASE_STAGE3 = 3;
const int PHASE_ENDING = 4;

// extern : 다른 파일에서 사용 (전역변수 너무 많으면 위험)
extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern int g_player_heart; // 2 g_player_heart = 1 heart in game

#include "SDL.h"
#include <iostream>
#include "GameFunc.h"
#include "Intro.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Ending.h"



/////////////////////////////////////////////////
// Declaration
SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;

// Game Phases
int g_current_game_phase;

// Player stamina
int g_player_heart;

// Game ending
int g_game_ending;


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}

	g_window = SDL_CreateWindow("ERROR", 100, 100, 800, 700, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	InitGame();
	Init_Intro();
	Init_Stage1();
	Init_Stage2();
	Init_Stage3();
	Init_Ending();

	g_current_game_phase = PHASE_STAGE2;

	g_last_time_ms = SDL_GetTicks();

	while ( g_flag_running )
	{
		Uint32 cur_time_ms = SDL_GetTicks();
		if ( cur_time_ms-g_last_time_ms < 33 )
			continue;

		if ( g_current_game_phase ==  PHASE_INTRO )
		{
			HandleEvents_Intro();
			Update_Intro();
			Render_Intro();
		}
		else if (g_current_game_phase == PHASE_STAGE1)
		{
			HandleEvents_Stage1();
			Update_Stage1();
			Render_Stage1();
		}
		else if ( g_current_game_phase == PHASE_STAGE2 )
		{
			HandleEvents_Stage2();
			Update_Stage2();
			Render_Stage2();
		}
		else if ( g_current_game_phase == PHASE_STAGE3 )
		{
			HandleEvents_Stage3();
			Update_Stage3();
			Render_Stage3();
		}
		else if (g_current_game_phase == PHASE_ENDING)
		{
			HandleEvents_Ending();
			Update_Ending();
			Render_Ending();
		}

		g_last_time_ms = cur_time_ms;

	}

	Clear_Intro();
	Clear_Stage1();
	Clear_Stage2();
	Clear_Stage3();
	Clear_Ending();
	ClearGame();

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	Mix_CloseAudio();
	SDL_Quit();


	return 0;
}

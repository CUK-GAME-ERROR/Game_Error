#include "GameFunc.h"
#include "Ending.h"

using namespace std;

// success
SDL_Rect g_success_source_rect;
SDL_Rect g_success_destination_rect;
SDL_Texture* g_success_texture;

// fail
SDL_Rect g_fail_source_rect;
SDL_Rect g_fail_destination_rect;
SDL_Texture* g_fail_texture;

// restart
SDL_Rect g_restart_source_rect;
SDL_Rect g_restart_destination_rect;
SDL_Texture* g_restart_texture;

// Music
static Mix_Music* g_bgm_stage1;

void Init_Ending()
{
	// sucess
	SDL_Surface* success_surface = IMG_Load("../../Resources/gameclear.png");
	g_success_texture = SDL_CreateTextureFromSurface(g_renderer, success_surface);
	SDL_FreeSurface(success_surface);

	g_success_source_rect.x = 0;
	g_success_source_rect.y = 0;
	g_success_source_rect.w = 800;
	g_success_source_rect.h = 700;

	g_success_destination_rect.x = 0;
	g_success_destination_rect.y = 0;
	g_success_destination_rect.w = 800;
	g_success_destination_rect.h = 700;



	// fail
	SDL_Surface* failure_surface = IMG_Load("../../Resources/gameover.png");
	g_fail_texture = SDL_CreateTextureFromSurface(g_renderer, failure_surface);
	SDL_FreeSurface(failure_surface);

	g_fail_source_rect.x = 0;
	g_fail_source_rect.y = 0;
	g_fail_source_rect.w = 800;
	g_fail_source_rect.h = 700;

	g_fail_destination_rect.x = 0;
	g_fail_destination_rect.y = 0;
	g_fail_destination_rect.w = 800;
	g_fail_destination_rect.h = 700;


	// restart
	SDL_Surface* restart_surface = IMG_Load("../../Resources/ending_restart.png");
	g_restart_texture = SDL_CreateTextureFromSurface(g_renderer, restart_surface);
	SDL_FreeSurface(restart_surface);

	g_restart_source_rect.x = 0;
	g_restart_source_rect.y = 0;
	g_restart_source_rect.w = 842;
	g_restart_source_rect.h = 174;

	g_restart_destination_rect.x = 600;
	g_restart_destination_rect.y = 5;
	g_restart_destination_rect.w = 200;
	g_restart_destination_rect.h = 40;

	g_bgm_stage1 = Mix_LoadMUS("../../Resources/stage1.mp3");
}

void Update_Ending()
{
}


void Render_Ending()
{
	SDL_RenderClear(g_renderer);

	if (g_game_ending == 1) 	// 성공
		SDL_RenderCopy(g_renderer, g_success_texture, &g_success_source_rect, &g_success_destination_rect);
	else if (g_game_ending == 0)// 실패
		SDL_RenderCopy(g_renderer, g_fail_texture, &g_fail_source_rect, &g_fail_destination_rect);

	SDL_RenderCopy(g_renderer, g_restart_texture, &g_restart_source_rect, &g_restart_destination_rect);

	SDL_RenderPresent(g_renderer);
}



void HandleEvents_Ending()
{
	SDL_Event event;
	if(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
		
			// If the mouse left button is pressed. 
			if ( event.button.button == SDL_BUTTON_LEFT )
			{
				g_current_game_phase = PHASE_STAGE1;
				Mix_PlayMusic(g_bgm_stage1, -1);
			}
			break;

		default:
			break;
		}
	}
}


void Clear_Ending()
{
	SDL_DestroyTexture(g_success_texture);
	SDL_DestroyTexture(g_fail_texture);
	SDL_DestroyTexture(g_restart_texture);
	Mix_FreeMusic(g_bgm_stage1);
}
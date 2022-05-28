#include "GameFunc.h"
#include "Intro.h"

using namespace std;

// Intro1
SDL_Rect g_intro1_source_rect;
SDL_Rect g_intro1_destination_rect;
SDL_Texture* g_intro1_texture;

// Intro2
SDL_Rect g_intro2_source_rect;
SDL_Rect g_intro2_destination_rect;
SDL_Texture* g_intro2_texture;

// Title
SDL_Rect g_title_source_rect;
SDL_Rect g_title_destination_rect;
SDL_Texture* g_title_texture;

// Button
SDL_Rect g_button_source_rect;
SDL_Rect g_button_destination_rect;
SDL_Texture* g_button_texture;

// Music
Mix_Music* g_intro_bg_music;

// intro state 
int intro_state;	// 1 : intro 1
						// 2 : intro 2
						// 3 : title & start

void Init_Intro()
{
	intro_state = 1;

	// bg music
	g_intro_bg_music = Mix_LoadMUS("../../Resources/bgm1.mp3");
	if (g_intro_bg_music == 0) {
		std::cout << "Mix_LoadMUS(\"intro_bg_music.mp3\"): " << Mix_GetError() << std::endl;
	}
	Mix_VolumeMusic(60);
	Mix_FadeInMusic(g_intro_bg_music, -1, 2000);


	// intro 1
	SDL_Surface* intro1_surface = IMG_Load("../../Resources/intro1.png");
	g_intro1_texture = SDL_CreateTextureFromSurface(g_renderer, intro1_surface);
	SDL_FreeSurface(intro1_surface);

	g_intro1_source_rect.x = 0;
	g_intro1_source_rect.y = 0;
	g_intro1_source_rect.w = 800;
	g_intro1_source_rect.h = 700;

	g_intro1_destination_rect.x = 0;
	g_intro1_destination_rect.y = 0;
	g_intro1_destination_rect.w = 800;
	g_intro1_destination_rect.h = 700;


	// intro 2
	SDL_Surface* intro2_surface = IMG_Load("../../Resources/intro2.png");
	g_intro2_texture = SDL_CreateTextureFromSurface(g_renderer, intro2_surface);
	SDL_FreeSurface(intro2_surface);

	g_intro2_source_rect.x = 0;
	g_intro2_source_rect.y = 0;
	g_intro2_source_rect.w = 800;
	g_intro2_source_rect.h = 700;

	g_intro2_destination_rect.x = 0;
	g_intro2_destination_rect.y = 0;
	g_intro2_destination_rect.w = 800;
	g_intro2_destination_rect.h = 700;


	// title
	SDL_Surface* title_surface = IMG_Load("../../Resources/title.png");
	g_title_texture = SDL_CreateTextureFromSurface(g_renderer, title_surface);
	SDL_FreeSurface(title_surface);

	g_title_source_rect.x = 240;
	g_title_source_rect.y = 190;
	g_title_source_rect.w = 1260;
	g_title_source_rect.h = 410;

	g_title_destination_rect.x = 150;
	g_title_destination_rect.y = 100;
	g_title_destination_rect.w = 500;
	g_title_destination_rect.h = 150;


	// button
	SDL_Surface* start_button_surface = IMG_Load("../../Resources/button.png");
	g_button_texture = SDL_CreateTextureFromSurface(g_renderer, start_button_surface);
	SDL_FreeSurface(start_button_surface);

	g_button_source_rect.x = 0;
	g_button_source_rect.y = 0;
	g_button_source_rect.w = 493;
	g_button_source_rect.h = 493;

	g_button_destination_rect.x = 310;
	g_button_destination_rect.y = 410;
	g_button_destination_rect.w = 180;
	g_button_destination_rect.h = 180;
}

void Update_Intro()
{
	
}


void Render_Intro()
{
	SDL_RenderClear(g_renderer);


	if (intro_state == 1)
		SDL_RenderCopy(g_renderer, g_intro1_texture, &g_intro1_source_rect, &g_intro1_destination_rect);
	else if (intro_state == 2)
		SDL_RenderCopy(g_renderer, g_intro2_texture, &g_intro2_source_rect, &g_intro2_destination_rect);
	else if (intro_state == 3) {
		SDL_SetRenderDrawColor(g_renderer, 180, 199, 231, 255);
		SDL_RenderCopy(g_renderer, g_title_texture, &g_title_source_rect, &g_title_destination_rect);
		SDL_RenderCopy(g_renderer, g_button_texture, &g_button_source_rect, &g_button_destination_rect);

	}


	//SDL_RenderCopy(g_renderer, g_title_texture, &g_title_source_rect, &g_title_destination_rect);

	SDL_RenderPresent(g_renderer);
}



void HandleEvents_Intro()
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
				int mouse_x = event.button.x;
				int mouse_y = event.button.y;

				if (intro_state == 3 &&
					mouse_x > g_button_destination_rect.x &&
					mouse_y > g_button_destination_rect.y &&
					mouse_x < g_button_destination_rect.x + g_button_destination_rect.w &&
					mouse_y < g_button_destination_rect.y + g_button_destination_rect.h)
				{
					Mix_FreeMusic(g_intro_bg_music);
					g_current_game_phase = PHASE_STAGE1;
				}
			}
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {
				if (intro_state == 1) {
					intro_state = 2;
				}
					
				else if (intro_state == 2) {
					intro_state = 3;
				}
					
			}
			break;

		default:
			break;
		}
	}
}


void Clear_Intro()
{
	SDL_DestroyTexture(g_intro1_texture);
	SDL_DestroyTexture(g_intro2_texture);
	SDL_DestroyTexture(g_title_texture);
	SDL_DestroyTexture(g_button_texture);
	
}
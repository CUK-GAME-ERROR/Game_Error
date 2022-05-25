#include "GameFunc.h"
#include "Intro.h"

void Init_Intro()
{
}

void Update_Intro()
{
}


void Render_Intro()
{
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
				g_current_game_phase = PHASE_STAGE1;
			}
			break;

		default:
			break;
		}
	}
}


void Clear_Intro()
{
}

#include "GameFunc.h"
#include "Ending.h"



void Init_Ending()
{
}

void Update_Ending()
{
}


void Render_Ending()
{
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
				g_current_game_phase = PHASE_INTRO;
			}
			break;

		default:
			break;
		}
	}
}


void Clear_Ending()
{
}
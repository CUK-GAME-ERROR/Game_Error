#include "GameFunc.h"
#include "Stage1.h"


void Init_Stage1()
{
}

void Update_Stage1()
{
}


void Render_Stage1()
{
}



void HandleEvents_Stage1()
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
				g_current_game_phase = PHASE_STAGE2;
			}
			break;

		default:
			break;
		}
	}
}



void Clear_Stage1()
{
}


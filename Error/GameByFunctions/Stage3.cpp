#include "GameFunc.h"
#include "Stage3.h"

int g_elapsed_time_ms;
bool g_input[3] = { false, false, false };

//player
SDL_Texture* g_player_sheet_texture;
SDL_Rect g_source_rect;
SDL_Rect g_destination_rect;

float power;
float Power;
float jumpSpeed;
bool isJump;

//boss
SDL_Rect b_destination_rect;
int ran;
float ranPos_x[3] = { 500, 150, 400 };
float ranPos_y[3] = { 600, 200, 350 };


void Init_Stage3()
{
	power = 100;
	jumpSpeed = 20;
	Power = power;

	g_flag_running = true;
	g_elapsed_time_ms = 0;

	SDL_Surface* player_sheet_surface = IMG_Load("../../Resources/Player.png");
	g_player_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, player_sheet_surface);
	SDL_FreeSurface(player_sheet_surface);

	g_source_rect.x = 63;
	g_source_rect.y = 41;
	g_source_rect.w = 374;
	g_source_rect.h = 416;

	g_destination_rect.x = 150;
	g_destination_rect.y = 600;
	g_destination_rect.w = 50;
	g_destination_rect.h = 100;

	b_destination_rect.x = 500;
	b_destination_rect.y = 600;
	b_destination_rect.w = 100;
	b_destination_rect.h = 100;
}



void HandleEvents_Stage3()
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			g_flag_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_input[0] = true;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_input[1] = true;
			}

			if (event.key.keysym.sym == SDLK_SPACE) {
				g_input[2] = true;
			}

			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_input[0] = false;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_input[1] = false;
			}

			break;
		}
	}
}

void Update_Stage3()
{
	if (g_input[0]) {
		//std::cout << "Left" << std::endl;
		g_destination_rect.x -= 10;
	}
	else if (g_input[1]) {
		//std::cout << "Right" << std::endl;
		g_destination_rect.x += 10;
	}

	if (g_input[2]) {
		if (power > 0) {
			//std::cout << "UP" << std::endl;
			g_destination_rect.y-= jumpSpeed;
			power-= jumpSpeed;
		}
		else {
			if (power <= -1 * Power + jumpSpeed) {
				g_input[2] = false;
				power = Power + 1;
			}
		    power-= jumpSpeed;
			//std::cout << "Down" << std::endl;
			g_destination_rect.y+= jumpSpeed;
		}
	}

	srand(time(NULL));
	ran = rand() % 3;
	b_destination_rect.x = ranPos_x[ran];
	b_destination_rect.y = ranPos_y[ran];

	g_elapsed_time_ms += 33;
}


void Render_Stage3()
{
	// Background
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer);

	{
		SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);

		SDL_Rect backGround;
		backGround.x = 50;
		backGround.y = 100;
		backGround.w = 600;
		backGround.h = 650;

		SDL_RenderFillRect(g_renderer, &backGround);
	}

	{
		SDL_SetRenderDrawColor(g_renderer, 128, 128, 128, 255);

		SDL_Rect ground1;
		ground1.x = 50;
		ground1.y = 700;
		ground1.w = 600;
		ground1.h = 50;

		SDL_RenderFillRect(g_renderer, &ground1);
	}

	{
		SDL_SetRenderDrawColor(g_renderer, 188, 229, 92, 255);

		SDL_Rect player;
		player.x = g_destination_rect.x;
		player.y = g_destination_rect.y;
		player.w = g_destination_rect.w;
		player.h = g_destination_rect.h;

		SDL_RenderFillRect(g_renderer, &player);
	}

	{
		SDL_SetRenderDrawColor(g_renderer, 188, 229, 92, 255);

		SDL_Rect boss;
		boss.x = b_destination_rect.x;
		boss.y = b_destination_rect.y;
		boss.w = b_destination_rect.w;
		boss.h = b_destination_rect.h;

		SDL_RenderFillRect(g_renderer, &boss);
	}

	//Player
	//SDL_RenderCopy(g_renderer, g_player_sheet_texture, &g_source_rect, &g_destination_rect);

	SDL_RenderPresent(g_renderer);
}



void Clear_Stage3()
{
	SDL_DestroyTexture(g_player_sheet_texture);
}


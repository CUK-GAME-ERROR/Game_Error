#include "GameFunc.h"
#include "Stage2.h"
#include <iostream>
#include <vector>

class Pos
{
public:
	Pos()
	{
		x = 0;
		y = 0;
	}
	Pos(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	int x;
	int y;
};

static std::vector<Pos> map;

static SDL_Texture* g_texture_player;
static SDL_Rect g_source_rectangle_player;
static SDL_Rect g_destination_rectangle_player;

static SDL_Texture* g_texture_ladder;
static SDL_Rect g_source_rectangle_ladder;
static SDL_Rect g_destination_rectangle_ladder[4];

static bool g_player_go_left;
static bool g_player_go_right;
static bool g_player_go_up;
static bool g_player_go_down;

int IndextoX(int i)
{
	return (i % 32) * 25;
}

int IndextoY(int i)
{
	return (i / 32) * 25 + 125;
}

void Init_Map()
{
	for (int i = 0; i <= 63; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 672; i <= 735; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 64; i <= 640; i += 32)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 95; i <= 671; i += 32)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 161; i <= 171; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 176; i <= 187; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 289; i <= 294; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 298; i <= 306; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 310; i <= 312; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 316; i <= 318; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 422; i <= 429; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 435; i <= 446; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 545; i <= 552; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 558; i <= 562; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 568; i <= 574; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
}



void Init_Stage2()
{
	// map
	Init_Map();

	// player
	SDL_Surface* player_surface = IMG_Load("../../Resources/player_stop_left.png");
	g_texture_player = SDL_CreateTextureFromSurface(g_renderer, player_surface);
	SDL_FreeSurface(player_surface);

	SDL_QueryTexture(g_texture_player, NULL, NULL, &g_source_rectangle_player.w, &g_source_rectangle_player.h);
	g_source_rectangle_player.x = 0;
	g_source_rectangle_player.y = 0;
	g_destination_rectangle_player.x = IndextoX(670);
	g_destination_rectangle_player.y = IndextoY(702) - g_source_rectangle_player.h;
	g_destination_rectangle_player.w = g_source_rectangle_player.w;
	g_destination_rectangle_player.h = g_source_rectangle_player.h;

	g_player_go_left = false;
	g_player_go_right = false;
	g_player_go_up = false;
	g_player_go_down = false;

	// ladder
	SDL_Surface* lader_surface = IMG_Load("../../Resources/ladder25.png");
	g_texture_ladder = SDL_CreateTextureFromSurface(g_renderer, lader_surface);
	SDL_FreeSurface(lader_surface);

	SDL_QueryTexture(g_texture_ladder, NULL, NULL, &g_source_rectangle_ladder.w, &g_source_rectangle_ladder.h);
	g_source_rectangle_ladder.x = 0;
	g_source_rectangle_ladder.y = 0;
	g_destination_rectangle_ladder[0] = { IndextoX(656), IndextoY(560), g_source_rectangle_ladder.w, g_source_rectangle_ladder.h };
	g_destination_rectangle_ladder[1] = { IndextoX(541), IndextoY(445), g_source_rectangle_ladder.w, g_source_rectangle_ladder.h };
	g_destination_rectangle_ladder[2] = { IndextoX(395), IndextoY(299), g_source_rectangle_ladder.w, g_source_rectangle_ladder.h };
	g_destination_rectangle_ladder[3] = { IndextoX(279), IndextoY(183), g_source_rectangle_ladder.w, g_source_rectangle_ladder.h };

}

void Update_Stage2()
{
	if (g_player_go_left)
	{
		if ((g_destination_rectangle_player.y == IndextoY(702) - g_source_rectangle_player.h && g_destination_rectangle_player.x > IndextoX(641)) ||
			(g_destination_rectangle_player.y == IndextoY(574) - g_source_rectangle_player.h && g_destination_rectangle_player.x > IndextoX(513)) ||
			(g_destination_rectangle_player.y == IndextoY(446) - g_source_rectangle_player.h && g_destination_rectangle_player.x > IndextoX(385)) ||
			(g_destination_rectangle_player.y == IndextoY(318) - g_source_rectangle_player.h && g_destination_rectangle_player.x > IndextoX(257)) ||
			(g_destination_rectangle_player.y == IndextoY(190) - g_source_rectangle_player.h && g_destination_rectangle_player.x > IndextoX(129)))
		{
			g_destination_rectangle_player.x -= 5;
			std::cout << g_destination_rectangle_player.x << std::endl;
		}
	}

	if (g_player_go_right)
	{
		if ((g_destination_rectangle_player.y == IndextoY(702) - g_source_rectangle_player.h && g_destination_rectangle_player.x < IndextoX(670)) ||
			(g_destination_rectangle_player.y == IndextoY(574) - g_source_rectangle_player.h && g_destination_rectangle_player.x < IndextoX(542)) ||
			(g_destination_rectangle_player.y == IndextoY(446) - g_source_rectangle_player.h && g_destination_rectangle_player.x < IndextoX(414)) ||
			(g_destination_rectangle_player.y == IndextoY(318) - g_source_rectangle_player.h && g_destination_rectangle_player.x < IndextoX(286)) ||
			(g_destination_rectangle_player.y == IndextoY(190) - g_source_rectangle_player.h && g_destination_rectangle_player.x < IndextoX(158)))
		{
			g_destination_rectangle_player.x += 5;
			std::cout << g_destination_rectangle_player.x << std::endl;
		}
	}

	if (g_player_go_up)
	{
		if ((g_destination_rectangle_player.x >= g_destination_rectangle_ladder[0].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[0].x + 5 &&
			g_destination_rectangle_player.y > g_destination_rectangle_ladder[0].y - g_source_rectangle_player.h && g_destination_rectangle_player.y <= IndextoY(688) - g_source_rectangle_player.h) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[1].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[1].x + 5 &&
				g_destination_rectangle_player.y > g_destination_rectangle_ladder[1].y - g_source_rectangle_player.h && g_destination_rectangle_player.y <= IndextoY(573) - g_source_rectangle_player.h) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[2].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[2].x + 5 &&
				g_destination_rectangle_player.y > g_destination_rectangle_ladder[2].y - g_source_rectangle_player.h && g_destination_rectangle_player.y <= IndextoY(427) - g_source_rectangle_player.h) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[3].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[3].x + 5 &&
				g_destination_rectangle_player.y > g_destination_rectangle_ladder[3].y - g_source_rectangle_player.h && g_destination_rectangle_player.y <= IndextoY(311) - g_source_rectangle_player.h))
		{
			g_destination_rectangle_player.y -= 5;
			std::cout << g_destination_rectangle_player.y << std::endl;
		}
	}

	if (g_player_go_down)
	{
		if ((g_destination_rectangle_player.x >= g_destination_rectangle_ladder[0].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[0].x + 5 &&
			g_destination_rectangle_player.y >= g_destination_rectangle_ladder[0].y - g_source_rectangle_player.h && g_destination_rectangle_player.y < IndextoY(688) - g_source_rectangle_player.h) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[1].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[1].x + 5 &&
				g_destination_rectangle_player.y >= g_destination_rectangle_ladder[1].y - g_source_rectangle_player.h && g_destination_rectangle_player.y < IndextoY(573) - g_source_rectangle_player.h) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[2].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[2].x + 5 &&
				g_destination_rectangle_player.y >= g_destination_rectangle_ladder[2].y - g_source_rectangle_player.h && g_destination_rectangle_player.y < IndextoY(427) - g_source_rectangle_player.h) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[3].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[3].x + 5 &&
				g_destination_rectangle_player.y >= g_destination_rectangle_ladder[3].y - g_source_rectangle_player.h && g_destination_rectangle_player.y < IndextoY(311) - g_source_rectangle_player.h))
		{
			g_destination_rectangle_player.y += 5;
			std::cout << g_destination_rectangle_player.y << std::endl;
		}
	}
}


void Render_Stage2()
{
	SDL_SetRenderDrawColor(g_renderer, 197, 224, 181, 255);
	SDL_RenderClear(g_renderer);

	SDL_Rect r;
	r.w = r.h = 25;

	// map
	SDL_SetRenderDrawColor(g_renderer, 143, 170, 220, 255);
	for (int i = 0; i < map.size(); i++) {
		r.x = map[i].x;
		r.y = map[i].y;
		SDL_RenderFillRect(g_renderer, &r);
	}

	// ladder
	for (int i = 0; i < 4; i++) {
		SDL_RenderCopy(g_renderer, g_texture_ladder, &g_source_rectangle_ladder, &g_destination_rectangle_ladder[i]);
	}

	// player
	SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player, &g_destination_rectangle_player);


	SDL_RenderPresent(g_renderer);
}



void HandleEvents_Stage2()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			// If the left arrow key is pressed. 
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				g_player_go_left = true;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				g_player_go_right = true;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				g_player_go_up = true;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				g_player_go_down = true;
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				g_player_go_left = false;
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				g_player_go_right = false;
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				g_player_go_up = false;
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				g_player_go_down = false;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				g_current_game_phase = PHASE_STAGE3;
			}
			break;

		default:
			break;
		}
	}
}



void Clear_Stage2()
{
	SDL_DestroyTexture(g_texture_player);
	SDL_DestroyTexture(g_texture_ladder);
}


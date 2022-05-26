#include "GameFunc.h"
#include "Stage2.h"
#include <iostream>
#include "player.h"

static std::vector<Pos> map;

static SDL_Texture* g_texture_player;
static SDL_Rect g_source_rectangle_player[10];
static SDL_Rect g_destination_rectangle_player;

static SDL_Texture* g_texture_ladder;
static SDL_Rect g_source_rectangle_ladder;
static SDL_Rect g_destination_rectangle_ladder[4];
static bool onladder;

static int g_player_height;
static int g_player_head; // 0:right, 1:left, 2: front
static int g_flag; // 1~10, 11~20
static float power;
static float Power;
static float jumpSpeed;
static bool isJump;
static bool g_player_go_left;
static bool g_player_go_right;
static bool g_player_go_up;
static bool g_player_go_down;

static SDL_Texture* g_texture_timer;
static SDL_Rect g_source_rectangle_timer;
static SDL_Rect g_destination_rectangle_timer;

static SDL_Texture* g_texture_timeW;
static SDL_Rect g_source_rectangle_timeW;
static SDL_Rect g_destination_rectangle_timeW;

static SDL_Texture* g_texture_timeG;
static SDL_Rect g_source_rectangle_timeG;
static SDL_Rect g_destination_rectangle_timeG;
static int g_time;
static int time_ms_;

static int monsterANum = 4;
static std::list<Monster> monsterA;
static std::vector<Pos> monsterA_Pos;
static float movementA[5] = { 100, 100, 125, 100 };
static bool directionA[5] = { true, false, true, false };
static int monsterBNum = 4;
static std::list<Monster> monsterB;
static std::vector<Pos> monsterB_Pos;
static float movementB[4] = { 150, 50, 100, 125 };
static bool directionB[4] = { true, true, true, true };
static int g_monster_size;
static SDL_Texture* g_texture_monster;
static SDL_Rect g_source_rectangle_monster[4];
static SDL_Rect g_destination_rectangle_monster;

static SDL_Texture* g_texture_pointer;
static SDL_Rect g_source_rectangle_pointer[2];
static SDL_Rect g_destination_rectangle_pointer;
static Pos pointer_Pos;
static int pointer_head;
static bool isShot;

void Init_Stage2()
{
	// map
	map = Init_Map();

	// player
	SDL_Surface* player_surface = IMG_Load("../../Resources/player_spritesheet.png");
	g_texture_player = SDL_CreateTextureFromSurface(g_renderer, player_surface);
	SDL_FreeSurface(player_surface);

	g_player_height = 30;
	for (int i = 0; i < 10; i++)
		g_source_rectangle_player[i] = { 25 * i, 0, 25, 30 };
	g_destination_rectangle_player.x = IndextoX(670);
	g_destination_rectangle_player.y = IndextoY(702) - g_player_height;
	g_destination_rectangle_player.w = 25;
	g_destination_rectangle_player.h = g_player_height;

	g_player_go_left = false;
	g_player_go_right = false;
	g_player_go_up = false;
	g_player_go_down = false;
	isJump = false;
	power = 40;
	jumpSpeed = 5;
	Power = power;
	g_player_head = 1;
	g_flag = 1;

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

	onladder = false;

	// time
	g_time = 60000;
	time_ms_ = 0;

	SDL_Surface* timeW_surface = IMG_Load("../../Resources/time_white.png");
	g_texture_timeW = SDL_CreateTextureFromSurface(g_renderer, timeW_surface);
	SDL_FreeSurface(timeW_surface);

	SDL_QueryTexture(g_texture_timeW, NULL, NULL, &g_source_rectangle_timeW.w, &g_source_rectangle_timeW.h);
	g_source_rectangle_timeW.x = 0;
	g_source_rectangle_timeW.y = 0;
	g_destination_rectangle_timeW.x = 25;
	g_destination_rectangle_timeW.y = 25;
	g_destination_rectangle_timeW.w = 750;
	g_destination_rectangle_timeW.h = 25;

	SDL_Surface* timeG_surface = IMG_Load("../../Resources/time_green.png");
	g_texture_timeG = SDL_CreateTextureFromSurface(g_renderer, timeG_surface);
	SDL_FreeSurface(timeG_surface);

	SDL_QueryTexture(g_texture_timeG, NULL, NULL, &g_source_rectangle_timeG.w, &g_source_rectangle_timeG.h);
	g_source_rectangle_timeG.x = 0;
	g_source_rectangle_timeG.y = 0;
	g_destination_rectangle_timeG.x = 25;
	g_destination_rectangle_timeG.y = 25;
	g_destination_rectangle_timeG.w = g_time * 0.0125;
	g_destination_rectangle_timeG.h = 25;

	SDL_Surface* timer_surface = IMG_Load("../../Resources/timer.png");
	g_texture_timer = SDL_CreateTextureFromSurface(g_renderer, timer_surface);
	SDL_FreeSurface(timer_surface);

	SDL_QueryTexture(g_texture_timer, NULL, NULL, &g_source_rectangle_timer.w, &g_source_rectangle_timer.h);
	g_source_rectangle_timer.x = 0;
	g_source_rectangle_timer.y = 0;
	g_destination_rectangle_timer.x = 25;
	g_destination_rectangle_timer.y = 0;
	g_destination_rectangle_timer.w = 50;
	g_destination_rectangle_timer.h = 50;

	// monster
	SDL_Surface* monster_surface = IMG_Load("../../Resources/monster_spritesheet.png");
	g_texture_monster = SDL_CreateTextureFromSurface(g_renderer, monster_surface);
	SDL_FreeSurface(monster_surface);

	for (int i = 0; i < 4; i++)
		g_source_rectangle_monster[i] = { 25 * i, 0, 25, 25 };
	g_monster_size = 25;

	monsterA_Pos.push_back(Pos(IndextoX(549), IndextoY(549) - g_monster_size));
	monsterA_Pos.push_back(Pos(IndextoX(426), IndextoY(426) - g_monster_size));
	monsterA_Pos.push_back(Pos(IndextoX(166), IndextoY(166) - g_monster_size));
	monsterA_Pos.push_back(Pos(IndextoX(183), IndextoY(183) - g_monster_size));
	for (int i = 0; i < monsterANum; i++)
		monsterA.push_back(Monster(monsterA_Pos[i], movementA[i], directionA[i]));

	monsterB_Pos.push_back(Pos(IndextoX(440), IndextoY(440) - g_monster_size));
	monsterB_Pos.push_back(Pos(IndextoX(291), IndextoY(291) - g_monster_size));
	monsterB_Pos.push_back(Pos(IndextoX(302), IndextoY(302) - g_monster_size));
	monsterB_Pos.push_back(Pos(IndextoX(181), IndextoY(181) - g_monster_size));
	for (int i = 0; i < monsterBNum; i++)
		monsterB.push_back(Monster(monsterB_Pos[i], movementB[i], directionB[i]));

	// pointer
	SDL_Surface* pointer_surface = IMG_Load("../../Resources/pointer_spritesheet.png");
	g_texture_pointer = SDL_CreateTextureFromSurface(g_renderer, pointer_surface);
	SDL_FreeSurface(pointer_surface);

	for (int i = 0; i < 2; i++)
		g_source_rectangle_pointer[i] = { i * 25, 0, 25, 25 };
	g_destination_rectangle_pointer.w = 25;
	g_destination_rectangle_pointer.h = 25;

	isShot = false;
}

void Update_Stage2()
{
	if (g_player_go_left)
	{
		if ((g_destination_rectangle_player.y == IndextoY(702) - g_player_height && g_destination_rectangle_player.x > IndextoX(641)) ||
			(g_destination_rectangle_player.y == IndextoY(574) - g_player_height && g_destination_rectangle_player.x > IndextoX(513)) ||
			(g_destination_rectangle_player.y == IndextoY(446) - g_player_height && g_destination_rectangle_player.x > IndextoX(385)) ||
			(g_destination_rectangle_player.y == IndextoY(318) - g_player_height && g_destination_rectangle_player.x > IndextoX(257)) ||
			(g_destination_rectangle_player.y == IndextoY(190) - g_player_height && g_destination_rectangle_player.x > IndextoX(129)))
		{
			g_destination_rectangle_player.x -= 5;
		}
	}

	if (g_player_go_right)
	{
		if ((g_destination_rectangle_player.y == IndextoY(702) - g_player_height && g_destination_rectangle_player.x < IndextoX(670)) ||
			(g_destination_rectangle_player.y == IndextoY(574) - g_player_height && g_destination_rectangle_player.x < IndextoX(542)) ||
			(g_destination_rectangle_player.y == IndextoY(446) - g_player_height && g_destination_rectangle_player.x < IndextoX(414)) ||
			(g_destination_rectangle_player.y == IndextoY(318) - g_player_height && g_destination_rectangle_player.x < IndextoX(286)) ||
			(g_destination_rectangle_player.y == IndextoY(190) - g_player_height && g_destination_rectangle_player.x < IndextoX(158)))
		{
			g_destination_rectangle_player.x += 5;
		}
	}

	if (g_player_go_up)
	{
		if ((g_destination_rectangle_player.x >= g_destination_rectangle_ladder[0].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[0].x + 5 &&
			g_destination_rectangle_player.y > g_destination_rectangle_ladder[0].y - g_player_height && g_destination_rectangle_player.y <= IndextoY(688) - g_player_height) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[1].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[1].x + 5 &&
				g_destination_rectangle_player.y > g_destination_rectangle_ladder[1].y - g_player_height && g_destination_rectangle_player.y <= IndextoY(573) - g_player_height) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[2].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[2].x + 5 &&
				g_destination_rectangle_player.y > g_destination_rectangle_ladder[2].y - g_player_height && g_destination_rectangle_player.y <= IndextoY(427) - g_player_height) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[3].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[3].x + 5 &&
				g_destination_rectangle_player.y > g_destination_rectangle_ladder[3].y - g_player_height && g_destination_rectangle_player.y <= IndextoY(311) - g_player_height))
		{
			onladder = true;
			g_player_head = 2;
			g_destination_rectangle_player.y -= 5;
		}
		else
			onladder = false;
	}

	if (g_player_go_down)
	{
		if ((g_destination_rectangle_player.x >= g_destination_rectangle_ladder[0].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[0].x + 5 &&
			g_destination_rectangle_player.y >= g_destination_rectangle_ladder[0].y - g_player_height && g_destination_rectangle_player.y < IndextoY(688) - g_player_height) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[1].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[1].x + 5 &&
				g_destination_rectangle_player.y >= g_destination_rectangle_ladder[1].y - g_player_height && g_destination_rectangle_player.y < IndextoY(573) - g_player_height) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[2].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[2].x + 5 &&
				g_destination_rectangle_player.y >= g_destination_rectangle_ladder[2].y - g_player_height && g_destination_rectangle_player.y < IndextoY(427) - g_player_height) ||
			(g_destination_rectangle_player.x >= g_destination_rectangle_ladder[3].x - 5 && g_destination_rectangle_player.x <= g_destination_rectangle_ladder[3].x + 5 &&
				g_destination_rectangle_player.y >= g_destination_rectangle_ladder[3].y - g_player_height && g_destination_rectangle_player.y < IndextoY(311) - g_player_height))
		{
			onladder = true;
			g_player_head = 2;
			g_destination_rectangle_player.y += 5;
		}
		else
			onladder = false;
	}

	if (isJump)
	{
		if (g_player_go_left && (g_destination_rectangle_player.x > IndextoX(641)))
			g_destination_rectangle_player.x -= 5;
		else if (g_player_go_right && (g_destination_rectangle_player.x < IndextoX(670)))
			g_destination_rectangle_player.x += 5;

		if (power > 0) {
			g_destination_rectangle_player.y -= jumpSpeed;
			power -= jumpSpeed;
		}
		else {
			if (power <= -1 * Power + jumpSpeed) {
				isJump = false;
				power = Power + 1;
			}
			power -= jumpSpeed;
			g_destination_rectangle_player.y += jumpSpeed;
		}
	}

	// monster
	for (auto iter = monsterA.begin(); iter != monsterA.end(); iter++) {
		iter->Move();
	}
	for (auto iter = monsterB.begin(); iter != monsterB.end(); iter++) {
		iter->Move();
	}

	// pointer
	if (isShot) {
		if (pointer_head == 0 && (g_destination_rectangle_pointer.x < IndextoX(670)) && (pointer_Pos.x + 100 > g_destination_rectangle_pointer.x))
			g_destination_rectangle_pointer.x += 10;
		else if (pointer_head == 1 && (g_destination_rectangle_pointer.x > IndextoX(641)) && (pointer_Pos.x - 100 < g_destination_rectangle_pointer.x))
			g_destination_rectangle_pointer.x -= 10;
		else
			isShot = false;
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

	// pointer
	if (isShot) {
		if (pointer_head == 0)
			SDL_RenderCopy(g_renderer, g_texture_pointer, &g_source_rectangle_pointer[0], &g_destination_rectangle_pointer);
		else if (pointer_head == 1)
			SDL_RenderCopy(g_renderer, g_texture_pointer, &g_source_rectangle_pointer[1], &g_destination_rectangle_pointer);
	}

	// player
	if (onladder) {
		SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[9], &g_destination_rectangle_player);
	}
	else {
		if (g_player_go_right) {
			if (g_flag <= 10) {
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[2], &g_destination_rectangle_player);
				g_flag++;
			}
			else {
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[3], &g_destination_rectangle_player);
				g_flag++;
				if (g_flag > 20)
					g_flag = 1;
			}
		}
		else if (g_player_go_left) {
			if (g_flag <= 10) {
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[6], &g_destination_rectangle_player);
				g_flag++;
			}
			else {
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[7], &g_destination_rectangle_player);
				g_flag++;
				if (g_flag > 20)
					g_flag = 1;
			}
		}
		else if (isShot) {
			if(g_player_head == 0)
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[4], &g_destination_rectangle_player);
			else if (g_player_head == 1)
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[5], &g_destination_rectangle_player);
		}
		else if (g_player_head == 0)
			SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[1], &g_destination_rectangle_player);
		else if (g_player_head == 1)
			SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[8], &g_destination_rectangle_player);
		else if (g_player_head == 2)
			SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[0], &g_destination_rectangle_player);
	}

	// timer
	static Uint32 last_ticks = SDL_GetTicks();
	Uint32 current_ticks = SDL_GetTicks();

	time_ms_ += current_ticks - last_ticks;
	last_ticks = current_ticks;

	g_destination_rectangle_timeG.w = (g_time - time_ms_) * 0.0125;

	SDL_RenderCopy(g_renderer, g_texture_timeW, &g_source_rectangle_timeW, &g_destination_rectangle_timeW);
	SDL_RenderCopy(g_renderer, g_texture_timeG, &g_source_rectangle_timeG, &g_destination_rectangle_timeG);
	SDL_RenderCopy(g_renderer, g_texture_timer, &g_source_rectangle_timer, &g_destination_rectangle_timer);

	// monster
	for (auto iter = monsterA.begin(); iter != monsterA.end(); iter++) {
		g_destination_rectangle_monster.x = iter->pos.x;
		g_destination_rectangle_monster.y = iter->pos.y;
		g_destination_rectangle_monster.w = g_monster_size;
		g_destination_rectangle_monster.h = g_monster_size;

		if (iter->isRight)
			SDL_RenderCopy(g_renderer, g_texture_monster, &g_source_rectangle_monster[1], &g_destination_rectangle_monster);
		else
			SDL_RenderCopy(g_renderer, g_texture_monster, &g_source_rectangle_monster[0], &g_destination_rectangle_monster);
	}

	for (auto iter = monsterB.begin(); iter != monsterB.end(); iter++) {
		g_destination_rectangle_monster.x = iter->pos.x;
		g_destination_rectangle_monster.y = iter->pos.y;
		g_destination_rectangle_monster.w = g_monster_size;
		g_destination_rectangle_monster.h = g_monster_size;

		if (iter->isRight)
			SDL_RenderCopy(g_renderer, g_texture_monster, &g_source_rectangle_monster[2], &g_destination_rectangle_monster);
		else
			SDL_RenderCopy(g_renderer, g_texture_monster, &g_source_rectangle_monster[3], &g_destination_rectangle_monster);
	}

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
				g_player_head = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				g_player_go_right = true;
				g_player_head = 0;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				if (!isJump)
					g_player_go_up = true;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				if (!isJump)
					g_player_go_down = true;
			}

			if (event.key.keysym.sym == SDLK_SPACE)
			{
				if (!onladder)
					isJump = true;
			}

			if (event.key.keysym.sym == SDLK_z)
			{
				if (!onladder && !isShot && !(g_player_head == 2)) {
					isShot = true;
					g_destination_rectangle_pointer.x = g_destination_rectangle_player.x;
					g_destination_rectangle_pointer.y = g_destination_rectangle_player.y;
					pointer_Pos.x = g_destination_rectangle_pointer.x;
					pointer_Pos.y = g_destination_rectangle_pointer.y;
					pointer_head = g_player_head;
				}
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				g_player_go_left = false;
				g_flag = 1;
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				g_player_go_right = false;
				g_flag = 1;
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
	SDL_DestroyTexture(g_texture_timeW);
	SDL_DestroyTexture(g_texture_timeG);
	SDL_DestroyTexture(g_texture_timer);
	SDL_DestroyTexture(g_texture_monster);
	SDL_DestroyTexture(g_texture_pointer);
}


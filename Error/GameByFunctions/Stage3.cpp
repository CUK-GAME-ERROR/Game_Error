#include "GameFunc.h"
#include "Stage3.h"
#include "player.h"

bool g_input[3] = { false, false, false };

//player
SDL_Texture* g_player_sheet_texture;
SDL_Rect g_source_rectangle_player;
SDL_Rect g_destination_rectangle_player;
SDL_Rect player_attack_rect;

float power;
float Power;
float jumpSpeed;
bool isJump;
int hp_P;


//boss
SDL_Texture* b_sheet_texture;
SDL_Rect b_source_rect;
int hp_B = 5;
SDL_Rect b_destination_rect;
int ran;
float ranPos_x[3] = { 50, 500,  350};
float ranPos_y[3] = { 195, 295, 395 };
float attackCool_Down;
SDL_Rect b_attackDown_destination;

std::list<Pos> a_position;
bool isDown;
bool isTime;
bool isMove;

//player 조작법
int ran_m[3] = { 0, 1, 2 };
bool isChange; //발동 제한
bool isRan; //랜덤 한번만 실행
float changeCool; //바뀐 방향키 유지 쿨타임
float randCool; //발동 쿨타임

//timer
int g_elapsed_time_ms;
int g_last_time_ms;
int c_last_time;

//monster
int monsterNum = 4;
std::list<Monster> sub_M;
std::vector<Pos> monster_Pos;
float movement[4] = { 55, 140, 70, 60 };
bool direction[4] = { true, false, true, false };

SDL_Texture* m_sheet_texture;
SDL_Rect m_source_rect;
SDL_Rect m_destination_rect;

//map
static std::vector<Pos> map_stage3;

//player 공격
std::list<Pos> c_state[3];
bool c_direction;

void Init_Stage3()
{
	//map
	map_stage3 = Init_Map();

	power = 100;
	jumpSpeed = 20;
	Power = power;
	attackCool_Down = 2000;
	randCool = 10000;
	changeCool = 2000;

	b_attackDown_destination.x = 51;
	b_attackDown_destination.y = 184;
	b_attackDown_destination.w = 50;
	b_attackDown_destination.h = 50;

	for (int i = 0; i < 4; i++) {
		//std::cout << "x: " << b_attackDown_destination.x << " y: " << b_attackDown_destination.y << std::endl;
		a_position.push_back(Pos(b_attackDown_destination.x, b_attackDown_destination.y));
		b_attackDown_destination.x += 200;
	}

	//monster
	monster_Pos.push_back(Pos(100, 300));
	monster_Pos.push_back(Pos(500, 200));
	monster_Pos.push_back(Pos(220, 400));
	monster_Pos.push_back(Pos(500, 500));

	for (int k = 0; k < monsterNum; k++) {
		sub_M.push_back(Monster(monster_Pos[k], movement[k], direction[k]));
	}

	g_flag_running = true;
	g_elapsed_time_ms = 0;

	SDL_Surface* player_sheet_surface = IMG_Load("../../Resources/player_stop.png");
	g_player_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, player_sheet_surface);
	SDL_FreeSurface(player_sheet_surface);

	SDL_QueryTexture(g_player_sheet_texture, NULL, NULL, &g_source_rectangle_player.w, &g_source_rectangle_player.h);

	g_source_rectangle_player.x = 0;
	g_source_rectangle_player.y = 0;
	g_destination_rectangle_player.x = IndextoX(670);
	g_destination_rectangle_player.y = IndextoY(702) - g_source_rectangle_player.h;
	g_destination_rectangle_player.w = g_source_rectangle_player.w;
	g_destination_rectangle_player.h = g_source_rectangle_player.h;

	SDL_Surface* boss_sheet_surface = IMG_Load("../../Resources/boss.png");
	b_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, boss_sheet_surface);
	SDL_FreeSurface(boss_sheet_surface);

	b_source_rect.x = 61;
	b_source_rect.y = 38;
	b_source_rect.w = 396;
	b_source_rect.h = 427;

	b_destination_rect.x = 50;
	b_destination_rect.y = 195;
	b_destination_rect.w = 50;
	b_destination_rect.h = 60;

	//sub_Monster
	SDL_Surface* monster_sheet_surface = IMG_Load("../../Resources/sub.png");
	m_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, monster_sheet_surface);
	SDL_FreeSurface(monster_sheet_surface);

	m_source_rect.x = 27;
	m_source_rect.y = 27;
	m_source_rect.w = 267;
	m_source_rect.h = 254;

	m_destination_rect.x = sub_M.begin()->pos.x;
	m_destination_rect.y = sub_M.begin()->pos.y;
	m_destination_rect.w = 50;
	m_destination_rect.h = 50;
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
				c_direction = true;
				g_input[ran_m[0]] = true;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				c_direction = false;
				g_input[ran_m[1]] = true;
			}

			if (event.key.keysym.sym == SDLK_SPACE) {
				g_input[2] = true;
			}
			if (event.key.keysym.sym == SDLK_z) {

			}

			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_input[ran_m[0]] = false;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_input[ran_m[1]] = false;
			}

			break;


		case SDL_MOUSEBUTTONDOWN:
			
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				g_current_game_phase = PHASE_STAGE2;
			}
			break;

		default:
			break;
		}
	}
}

void ranMove() {
	isRan = true;
	std::cout << "random!" << std::endl;
	srand(time(NULL));

	ran_m[0] = rand() % 2;
	while (ran_m[1] != ran_m[0]) {
		ran_m[1] = rand() % 2;
	}

	std::cout << ran_m[0] << std::endl;
	isRan = false;
}

void Update_Stage3()
{
	if (isDown) {
		b_attackDown_destination.y += 10;
	}
	else if (!isDown) {
		b_attackDown_destination.y = 184;
		isTime = true;
		g_last_time_ms = g_elapsed_time_ms;
	}

	if (isChange) {
		if (g_elapsed_time_ms - c_last_time > randCool) {
			isChange = false;
		}

		if (g_elapsed_time_ms - c_last_time > changeCool) {
			for (int j = 0; j < 3; j++) {
				ran_m[j] = j;
			}
		}
	}
	else if (!isChange && !isRan) {
		isChange = true;
		c_last_time = g_elapsed_time_ms;
		ranMove();
	}

	if (g_input[0]) {
		g_destination_rectangle_player.x -= 10;
	}
	else if (g_input[1]) {
		g_destination_rectangle_player.x += 10;
	}

	if (g_input[2]) {
		if (power > 0) {
			g_destination_rectangle_player.y-= jumpSpeed;
			power-= jumpSpeed;
		}
		else {
			if (power <= -1 * Power + jumpSpeed) {
				g_input[2] = false;
				power = Power + 1;
			}
		    power-= jumpSpeed;
			g_destination_rectangle_player.y+= jumpSpeed;
		}
	}

	srand(time(NULL));
	if (isMove) {
		isMove = false;
		ran = rand() % 3;
		b_destination_rect.x = ranPos_x[ran];
		b_destination_rect.y = ranPos_y[ran];
	}

	for (auto iter = sub_M.begin(); iter != sub_M.end(); iter++) {
		iter->Move();
	}

	if (checkCollision(g_destination_rectangle_player, b_attackDown_destination)) {
		std::cout << "Attack" << std::endl;
	}

	if (checkCollision(b_destination_rect, player_attack_rect)) {
		isMove = true;
		hp_B--;
		std::cout << "Attack_B" << std::endl;
	}

	g_elapsed_time_ms += 33;
}


void Render_Stage3()
{
	// Background
	SDL_SetRenderDrawColor(g_renderer, 197, 224, 181, 255);
	SDL_RenderClear(g_renderer);

	// map
	SDL_SetRenderDrawColor(g_renderer, 143, 170, 220, 255);
	SDL_Rect r;

	for (int i = 0; i < map_stage3.size(); i++) {
		r.x = map_stage3[i].x;
		r.y = map_stage3[i].y;
		r.w = 25;
		r.h = 25;
		SDL_RenderFillRect(g_renderer, &r);
	}

	{
		SDL_SetRenderDrawColor(g_renderer, 188, 229, 92, 255);

		SDL_Rect player;
		player.x = g_destination_rectangle_player.x;
		player.y = g_destination_rectangle_player.y;
		player.w = g_destination_rectangle_player.w;
		player.h = g_destination_rectangle_player.h;

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


	for (auto iter = sub_M.begin(); iter != sub_M.end(); iter++) {
		m_destination_rect.x = iter->pos.x;
		m_destination_rect.y = iter->pos.y;

		SDL_RenderCopy(g_renderer, m_sheet_texture, &m_source_rect, &m_destination_rect);
	}

	if (isTime) {
		isDown = true;

		if (b_attackDown_destination.y < 600) {
			SDL_SetRenderDrawColor(g_renderer, 188, 229, 92, 255);

			for (auto iter = a_position.begin(); iter != a_position.end(); iter++) {
				b_attackDown_destination.x = iter->x;
				SDL_RenderFillRect(g_renderer, &b_attackDown_destination);
			}
		}

		if (g_elapsed_time_ms - g_last_time_ms > attackCool_Down) {
			isDown = false;
		}
	}

	//Player
	//SDL_RenderCopy(g_renderer, g_player_sheet_texture, &g_source_rectangle_player, &g_destination_rectangle_player);

	//boss
	//SDL_RenderCopy(g_renderer, b_sheet_texture, &b_source_rect, &b_destination_rect);

	SDL_RenderPresent(g_renderer);
}



void Clear_Stage3()
{
	SDL_DestroyTexture(g_player_sheet_texture);
}


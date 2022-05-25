#include "GameFunc.h"
#include "Stage3.h"
#include <vector>
#include <list>

class Pos
{
public:
	Pos() {
		x = 0;
		y = 0;
	}

	Pos(float _x, float _y) {
		x = _x;
		y = _y;
	}

	float x;
	float y;
};

class Monster {
public:
	Pos pos;
	Pos init_pos;
	float move_destination;
	bool isRight;

	Monster() {
		init_pos.x = 0;
		init_pos.y = 0;
		pos.x = 0;
		pos.y = 0;
		move_destination = 0;
	}

	Monster(Pos p, float move, bool right) {
		pos = p;
		init_pos = p;
		move_destination = move;
		isRight = right;
	}

	void Move() {
		if (isRight) {
			if (pos.x < init_pos.x + move_destination) {
				pos.x += 5;
			}
			else
				isRight = false;
		}
		else {
			if (pos.x > init_pos.x - move_destination) {
				pos.x -= 5;
			}
			else {
				isRight = true;
			}
		}
	}
};

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
float attackCool_Down;
SDL_Rect b_attackDown_destination;

std::list<Pos> a_position;
bool isDown;
bool isTime;

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
int monsterNum = 3;
std::list<Monster> sub_M;
std::vector<Pos> monster_Pos;
float movement[3] = { 100, 50, 30 };
bool direction[3] = { true, false, true };

void Init_Stage3()
{
	power = 100;
	jumpSpeed = 20;
	Power = power;
	attackCool_Down = 2000;
	randCool = 3000;
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

	//sub_monster
	monster_Pos.push_back(Pos(150, 300));
	monster_Pos.push_back(Pos(200, 350));
	monster_Pos.push_back(Pos(150, 400));

	for (int k = 0; k < monsterNum; k++) {
		sub_M.push_back(Monster(monster_Pos[k], movement[k], direction[k]));
	}

	g_flag_running = true;
	g_elapsed_time_ms = 0;

	SDL_Surface* player_sheet_surface = IMG_Load("../../Resources/Player.png");
	g_player_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, player_sheet_surface);
	SDL_FreeSurface(player_sheet_surface);

	//SDL_QueryTexture(g_player_sheet_texture, NULL, NULL, &g_source_rect.w, &g_source_rect.h);

	g_source_rect.x = 63;
	g_source_rect.y = 41;
	g_source_rect.w = 374;
	g_source_rect.h = 416;

	g_destination_rect.x = 300;
	g_destination_rect.y = 550;
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
				g_input[ran_m[0]] = true;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_input[ran_m[1]] = true;
			}

			if (event.key.keysym.sym == SDLK_SPACE) {
				g_input[ran_m[2]] = true;
			}

			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_input[ran_m[0]] = false;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_input[ran_m[1]] = false;
			}

			if (event.key.keysym.sym == SDLK_SPACE) {
				g_input[ran_m[2]] = false;
			}

			break;
		}
	}
}

void ranMove() {
	isRan = true;
	std::cout << "random!" << std::endl;
	srand(time(NULL));

	ran_m[0] = rand() % 3;
	for (int k = 1; k < 3; k++) {
		ran_m[k] = rand() % 3;

		if (ran_m[k] == ran_m[k - 1]) {
			continue;
		}
		else if (k == 2 && ran_m[k] == ran_m[k - 2]) {
			continue;
		}
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
		//std::cout << "isTime" << std::endl;
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

	for (auto iter = sub_M.begin(); iter != sub_M.end(); iter++) {
		iter->Move();
	}

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
		backGround.x = 34;
		backGround.y = 167;
		backGround.w = 733;
		backGround.h = 500;

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

	SDL_SetRenderDrawColor(g_renderer, 188, 229, 92, 255);
	SDL_Rect monster;

	for (auto iter = sub_M.begin(); iter != sub_M.end(); iter++) {
		monster.x = iter->pos.x;
		monster.y = iter->pos.y;
		monster.w = 30;
		monster.h = 30;

		SDL_RenderFillRect(g_renderer, &monster);
	}

	if (isTime) {
		isDown = true;

		if (b_attackDown_destination.y < 600) {
			SDL_SetRenderDrawColor(g_renderer, 188, 229, 92, 255);

			for (auto iter = a_position.begin(); iter != a_position.end(); iter++) {
				b_attackDown_destination.x = iter->x;
				//b_attackDown_destination.y = iter->y;
				SDL_RenderFillRect(g_renderer, &b_attackDown_destination);
			}
		}

		//std::cout << "Time: " << g_elapsed_time_ms - g_last_time_ms << std::endl;
		if (g_elapsed_time_ms - g_last_time_ms > attackCool_Down) {
			//std::cout << "CoolTime" << std::endl;
			isDown = false;
		}
	}

	//Player
	//SDL_RenderCopy(g_renderer, g_player_sheet_texture, &g_source_rect, &g_destination_rect);

	SDL_RenderPresent(g_renderer);
}



void Clear_Stage3()
{
	SDL_DestroyTexture(g_player_sheet_texture);
}


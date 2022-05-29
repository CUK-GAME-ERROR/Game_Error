#include "GameFunc.h"
#include "Stage3.h"
#include "player.h"

SDL_Texture* g_texture_stage3Intro;
SDL_Rect g_source_rectangle_stage3Intro;
SDL_Rect g_destination_rectangle_stage3Intro;
bool read_intro;

//player
int move_state;
static SDL_Texture* g_texture_player;
static SDL_Rect g_source_rectangle_player[10];
static SDL_Rect g_destination_rectangle_player;

float power;
float Power;
float jumpSpeed;
bool isJump;
int hp_P;

static int g_player_height;
static int g_player_head; // 0:right, 1:left, 2: front
static int g_running_flag; // 1~10, 11~20
//static int g_player_heart; // 1 heart = 2 g_player_heart
static bool g_player_unbeatable;
static int g_unbeatable_flag;

/*
* static bool g_player_go_left;
static bool g_player_go_right;
static bool g_player_go_up;
static bool g_player_go_down;

*/
bool g_player_state[4] = { false, false, false, false };


//boss
SDL_Texture* b_sheet_texture;
SDL_Rect b_source_rect;
int hp_B = 5;
SDL_Rect b_destination_rect;
int ran;
//float ranPos_x[3] = { IndextoX(98), IndextoX(252), IndextoX(610) };
//float ranPos_y[3] = { IndextoY(98), IndextoY(252), IndextoY(610) };
std::vector<Pos> ranPos;
float attackCool_Down;
SDL_Rect b_attackDown_destination;
SDL_Rect b_attackDown_rect;

std::list<Pos> a_position;
bool isDown;
bool isTime;
bool isMove;
bool isEffect;

SDL_Rect b_face_source_rect;
SDL_Rect b_face_destination_rect;

//player 조작법
int ran_m[4] = { 0, 1, 2, 3 };
//std::vector<int> randomNum;
int randomNum;
int randomIndex;
bool isChange; //발동 제한
bool isRan; //랜덤 한번만 실행
float changeCool; //바뀐 방향키 유지 쿨타임
float randCool; //발동 쿨타임
int currentKey;
bool isWarn;



//timer
int g_elapsed_time_ms;
int g_last_time_ms;
int c_last_time;

//monster
static int monsterANum = 3;
static std::list<Monster> monsterA;
static std::vector<Pos> monsterA_Pos;
static float movementA[5] = { 100, 100, 100 };
static bool directionA[5] = { true, false, false };

static int monsterBNum = 3;
static std::list<Monster> monsterB;
static std::vector<Pos> monsterB_Pos;
static float movementB[3] = { 150, 100, 98};
static bool directionB[3] = { true, true, true};

static SDL_Texture* g_texture_monster;
static SDL_Rect g_source_rectangle_monster[4];
static SDL_Rect g_destination_rectangle_monster;
static int g_monster_size;

bool isGone_A;
bool isGone_B;
bool isOnce_A;
bool isOnce_B;
int monsterA_waiting;
int monsterB_waiting;


//map
static std::vector<Pos> map_stage3;
static std::vector<Pos> ground;
static std::vector<Pos> hole;
static bool isFall;

//ladder
static SDL_Texture* g_texture_ladder;
static SDL_Rect g_source_rectangle_ladder;
static SDL_Rect g_destination_rectangle_ladder[4];
static bool onladder;

//link
static SDL_Texture* g_texture_link;
static SDL_Rect g_source_rectangle_link;
static SDL_Rect g_destination_rectangle_link;

//pointer
static SDL_Texture* g_texture_pointer;
static SDL_Rect g_source_rectangle_pointer[2];
static SDL_Rect g_destination_rectangle_pointer;
static int pointer_num; // 10
static Pos pointer_Pos;
static int pointer_head;
static bool isShot;

static SDL_Texture* g_texture_pointer50;
static SDL_Rect g_source_rectangle_pointer50;
static SDL_Rect g_destination_rectangle_pointer50;
static SDL_Texture* pointerNum_text;
static SDL_Rect pointerNum_rect;

//heart
static SDL_Texture* g_texture_heart;
static SDL_Rect g_source_rectangle_heart[3];
static SDL_Rect g_destination_rectangle_heart;

//boss Hp
static SDL_Texture* bHp_texture;
static SDL_Rect bHp_source_rect;
static SDL_Rect bHp_destination_rect;

static SDL_Texture* g_texture_timeW;
static SDL_Rect g_source_rectangle_timeW;
static SDL_Rect g_destination_rectangle_timeW;

//boss Attack
SDL_Texture* boxx_attack_texture;
SDL_Rect boss_attack_source;
SDL_Rect boss_attack_destination;

//sound
static Mix_Chunk* g_shooting_sound;
static Mix_Chunk* g_hit_sound;
static Mix_Chunk* g_attack_sound;
static Mix_Chunk* g_failure_sound;
static Mix_Chunk* b_attack_sound;
static Mix_Chunk* interact_sound_;

static Mix_Music* g_bgm_ending;

//font
TTF_Font* font;
SDL_Color red;
SDL_Texture* warnChange;
SDL_Rect warn_source;

void Reset_Stage3() {
	g_flag_running = true;
	g_elapsed_time_ms = 0;

	g_destination_rectangle_player.x = IndextoX(670);
	g_destination_rectangle_player.y = IndextoY(702) - g_player_height;

	for (int i = 0; i < 4; i++)
		g_player_state[i] = false;

	isJump = false;
	isFall = false;
	isShot = false;
	onladder = false;
	g_player_unbeatable = false;
	g_player_heart = 2;
	g_player_head = 1;
	g_running_flag = 1;

	read_intro = false;

	for (auto iter = monsterA.begin(); iter != monsterA.end(); iter++)
		iter->isAlive = true;
	for (auto iter = monsterB.begin(); iter != monsterB.end(); iter++)
		iter->isAlive = true;
}

void responeMonster(bool isA) {
	if (isA) {
		for (auto iter = monsterA.begin(); iter != monsterA.end(); iter++) {
			iter->isAlive = true;
		}
		isOnce_A = false;
	}
	else {
		for (auto iter = monsterB.begin(); iter != monsterB.end(); iter++) {
			iter->isAlive = true;
		}
		isOnce_B = false;
	}
}


void Init_Stage3()
{
	// intro
	SDL_Surface* stage3Intro_surface = IMG_Load("../../Resources/stage3_intro.png");
	g_texture_stage3Intro = SDL_CreateTextureFromSurface(g_renderer, stage3Intro_surface);
	SDL_FreeSurface(stage3Intro_surface);

	SDL_QueryTexture(g_texture_stage3Intro, NULL, NULL, &g_source_rectangle_stage3Intro.w, &g_source_rectangle_stage3Intro.h);
	g_source_rectangle_stage3Intro.x = 0;
	g_source_rectangle_stage3Intro.y = 0;
	g_destination_rectangle_stage3Intro = { 100, 100, 600, 500 };

	read_intro = false;

	//map
	map_stage3 = Init_Map();
	ground = Init_Ground();
	hole = Init_Hole();

	attackCool_Down = 5000;
	randCool = 25000;
	changeCool = 10000;

	//boss_attack
	SDL_Surface* boss_attack_surface = IMG_Load("../../Resources/lightning.png");
	boxx_attack_texture = SDL_CreateTextureFromSurface(g_renderer, boss_attack_surface);
	SDL_FreeSurface(boss_attack_surface);

	boss_attack_source.x = 529;
	boss_attack_source.y = 139;
	boss_attack_source.w = 112;
	boss_attack_source.h = 274;

	b_attackDown_destination.x = 51;
	b_attackDown_destination.y = 184;
	b_attackDown_destination.w = 50;
	b_attackDown_destination.h = 60;

	for (int i = 0; i < 4; i++) {
		a_position.push_back(Pos(b_attackDown_destination.x, b_attackDown_destination.y));
		b_attackDown_destination.x += 200;
	}

	g_flag_running = true;
	g_elapsed_time_ms = 0;

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

	isJump = false;
	power = 40;
	jumpSpeed = 5;
	Power = power;
	isChange = true;

	g_player_head = 1;
	g_running_flag = 1;
	//g_player_heart = 4;
	g_player_unbeatable = false;

	SDL_Surface* boss_sheet_surface = IMG_Load("../../Resources/boss.png");
	b_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, boss_sheet_surface);
	SDL_FreeSurface(boss_sheet_surface);

	b_source_rect.x = 61;
	b_source_rect.y = 38;
	b_source_rect.w = 396;
	b_source_rect.h = 427;

	b_destination_rect.x = 50;
	b_destination_rect.y = 500;
	b_destination_rect.w = 50;
	b_destination_rect.h = 55;

	b_face_source_rect.x = 74;
	b_face_source_rect.y = 51;
	b_face_source_rect.w = 357;
	b_face_source_rect.h = 270;

	b_face_destination_rect.x = 20;
	b_face_destination_rect.y = 10;
	b_face_destination_rect.w = 50;
	b_face_destination_rect.h = 50;

	//bossRandomMove
	ranPos.push_back(Pos(IndextoX(252), IndextoY(252)));
	ranPos.push_back(Pos(IndextoX(610), IndextoY(610)));
	ranPos.push_back(Pos(IndextoX(98), IndextoY(98)));
	ranPos.push_back(Pos(IndextoX(375), IndextoY(375)));

	//sub_Monster
	SDL_Surface* monster_surface = IMG_Load("../../Resources/monster_spritesheet.png");
	g_texture_monster = SDL_CreateTextureFromSurface(g_renderer, monster_surface);
	SDL_FreeSurface(monster_surface);

	for (int i = 0; i < 4; i++)
		g_source_rectangle_monster[i] = { 25 * i, 0, 25, 25 };
	g_monster_size = 25;

	monsterA_Pos.push_back(Pos(IndextoX(549), IndextoY(549) - g_monster_size));
	monsterA_Pos.push_back(Pos(IndextoX(426), IndextoY(426) - g_monster_size));
	monsterA_Pos.push_back(Pos(IndextoX(183), IndextoY(183) - g_monster_size));
	for (int i = 0; i < monsterANum; i++)
		monsterA.push_back(Monster(monsterA_Pos[i], movementA[i], directionA[i], true));

	monsterB_Pos.push_back(Pos(IndextoX(440), IndextoY(440) - g_monster_size));
	monsterB_Pos.push_back(Pos(IndextoX(571), IndextoY(571) - g_monster_size));
	monsterB_Pos.push_back(Pos(IndextoX(302), IndextoY(302) - g_monster_size));
	for (int i = 0; i < monsterBNum; i++)
		monsterB.push_back(Monster(monsterB_Pos[i], movementB[i], directionB[i], true));

	//ladder
	SDL_Surface* lader_surface = IMG_Load("../../Resources/ladder25.png");
	g_texture_ladder = SDL_CreateTextureFromSurface(g_renderer, lader_surface);
	SDL_FreeSurface(lader_surface);

	SDL_QueryTexture(g_texture_ladder, NULL, NULL, &g_source_rectangle_ladder.w, &g_source_rectangle_ladder.h);
	g_source_rectangle_ladder.x = 0;
	g_source_rectangle_ladder.y = 0;
	g_destination_rectangle_ladder[0] = { IndextoX(644), IndextoY(548), g_source_rectangle_ladder.w, g_source_rectangle_ladder.h };
	g_destination_rectangle_ladder[1] = { IndextoX(541), IndextoY(445), g_source_rectangle_ladder.w, g_source_rectangle_ladder.h };
	g_destination_rectangle_ladder[2] = { IndextoX(395), IndextoY(299), g_source_rectangle_ladder.w, g_source_rectangle_ladder.h };
	g_destination_rectangle_ladder[3] = { IndextoX(279), IndextoY(183), g_source_rectangle_ladder.w, g_source_rectangle_ladder.h };

	onladder = false;

	// heart
	SDL_Surface* heart_surface = IMG_Load("../../Resources/heart_spritesheet.png");
	g_texture_heart = SDL_CreateTextureFromSurface(g_renderer, heart_surface);
	SDL_FreeSurface(heart_surface);

	for (int i = 0; i < 3; i++)
		g_source_rectangle_heart[i] = { 25 * i, 0, 25, 25 };

	// link
	SDL_Surface* link_surface = IMG_Load("../../Resources/link.png");
	g_texture_link = SDL_CreateTextureFromSurface(g_renderer, link_surface);
	SDL_FreeSurface(link_surface);

	SDL_QueryTexture(g_texture_link, NULL, NULL, &g_source_rectangle_link.w, &g_source_rectangle_link.h);
	g_source_rectangle_link.x = 0;
	g_source_rectangle_link.y = 0;
	g_destination_rectangle_link.x = IndextoX(0);
	g_destination_rectangle_link.y = IndextoY(0);
	g_destination_rectangle_link.w = g_source_rectangle_link.w;
	g_destination_rectangle_link.h = g_source_rectangle_link.h;


	//pointer
	SDL_Surface* pointer_surface = IMG_Load("../../Resources/pointer_spritesheet.png");
	g_texture_pointer = SDL_CreateTextureFromSurface(g_renderer, pointer_surface);

	pointer_surface = IMG_Load("../../Resources/pointer50.png");
	g_texture_pointer50 = SDL_CreateTextureFromSurface(g_renderer, pointer_surface);

	SDL_FreeSurface(pointer_surface);

	for (int i = 0; i < 2; i++)
		g_source_rectangle_pointer[i] = { i * 25, 0, 25, 25 };
	g_destination_rectangle_pointer.w = 25;
	g_destination_rectangle_pointer.h = 25;

	SDL_QueryTexture(g_texture_pointer50, NULL, NULL, &g_source_rectangle_pointer50.w, &g_source_rectangle_pointer50.h);
	g_source_rectangle_pointer50.x = 0;
	g_source_rectangle_pointer50.y = 0;
	g_destination_rectangle_pointer50 = { 25, 75, 50, 50 };

	pointer_num = 10;
	isShot = false;

	//boss Hp
	SDL_Surface* bossHp_surface = IMG_Load("../../Resources/time_red.png");
	bHp_texture = SDL_CreateTextureFromSurface(g_renderer, bossHp_surface);
	SDL_FreeSurface(bossHp_surface);
	SDL_QueryTexture(bHp_texture, NULL, NULL, &bHp_source_rect.w, &bHp_source_rect.h);

	bHp_source_rect.x = 0;
	bHp_source_rect.y = 0;
	bHp_destination_rect.x = 25;
	bHp_destination_rect.y = 25;
	bHp_destination_rect.w = 750;
	bHp_destination_rect.h = 25;

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

	//sound
	g_bgm_ending = Mix_LoadMUS("../../Resources/gameClear.mp3");

	g_shooting_sound = Mix_LoadWAV("../../Resources/shooting.mp3");
	g_hit_sound = Mix_LoadWAV("../../Resources/hit.wav");
	g_attack_sound = Mix_LoadWAV("../../Resources/attack.mp3");
	interact_sound_ = Mix_LoadWAV("../../Resources/interact.wav");
	g_failure_sound = Mix_LoadWAV("../../Resources/failure.mp3");
	Mix_VolumeChunk(g_failure_sound, 50);
	b_attack_sound = Mix_LoadWAV("../../Resources/lightning.mp3");
	Mix_VolumeChunk(b_attack_sound, 30);

	// text
	font = TTF_OpenFont("../../Resources/DungGeunMo.ttf", 50);
	red = { 255, 0, 0, 255 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font, "Warning: left <-> right / up <-> down", red);
	
	warn_source.x = 0;
	warn_source.y = 0;
	warn_source.w = tmp_surface->w;
	warn_source.h = tmp_surface->h;

	warnChange = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font);
}

void playerRanKeyDown(int move_state) {
	for (int i = 0; i < 4; i++) {
		g_player_state[i] = false;
	}

	switch (move_state) {
	case 0:
		g_player_state[0] = true;
		g_player_head = 1;
		break;
	case 1:
		g_player_state[1] = true;
		g_player_head = 0;
		break;
	case 2:
		if (!isJump)
			g_player_state[2] = true;
		break;
	case 3:
		if (!isJump)
			g_player_state[3] = true;
		break;
	}
}

void playerRanKeyUp(int move_state) {
	switch (move_state) {
	case 0:
		g_player_state[0] = false;
		g_running_flag = 1;
		break;
	case 1:
		g_player_state[1] = false;
		g_running_flag = 1;
		break;
	case 2:
		g_player_state[2] = false;
		break;
	case 3:
		g_player_state[3] = false;
		break;
	}
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
			if (!isFall && read_intro) {
				if (event.key.keysym.sym == SDLK_LEFT)
				{
					playerRanKeyDown(ran_m[0]);
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					std::cout << "KeyDown" << std::endl;
					for (int i = 0; i < 4; i++) {
						std::cout << i << ": " << ran_m[i] << std::endl;
					}
					playerRanKeyDown(ran_m[1]);
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					playerRanKeyDown(ran_m[2]);
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					playerRanKeyDown(ran_m[3]);
				}

				if (event.key.keysym.sym == SDLK_SPACE)
				{
					if (!onladder)
						isJump = true;
				}

				if (event.key.keysym.sym == SDLK_z) {
					if (!onladder && !isShot && !(g_player_head == 2)) {
						Mix_PlayChannel(-1, g_shooting_sound, 0);
						isShot = true;
						g_destination_rectangle_pointer.x = g_destination_rectangle_player.x;
						g_destination_rectangle_pointer.y = g_destination_rectangle_player.y;
						pointer_Pos.x = g_destination_rectangle_pointer.x;
						pointer_Pos.y = g_destination_rectangle_pointer.y;
						pointer_head = g_player_head;
					}
				}

				if (event.key.keysym.sym == SDLK_n)
				{
					Reset_Stage3();
					g_current_game_phase = PHASE_ENDING;
					g_game_ending = 1;
					Mix_PlayMusic(g_bgm_ending, -1);
				}
			}

			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				currentKey = 0;
				playerRanKeyUp(ran_m[0]);
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				currentKey = 1;
				playerRanKeyUp(ran_m[1]);
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				currentKey = 2;
				playerRanKeyUp(ran_m[2]);
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				currentKey = 3;
				playerRanKeyUp(ran_m[3]);
			}

			break;


		case SDL_MOUSEBUTTONDOWN:
			
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (!read_intro) {
					Mix_PlayChannel(-1, interact_sound_, 0);
					read_intro = true;
				}
			}
			break;

		default:
			break;
		}
	}
}


void ranMove() {
	isRan = true;

	ran_m[0] = 1;
	ran_m[1] = 0;
	ran_m[2] = 3;
	ran_m[3] = 2;

	isRan = false;
}

void Update_Stage3()
{
	if (isDown) {
		if (!isEffect && read_intro) {
			isEffect = true;
			//Mix_PlayChannel(-1, b_attack_sound, 0);
		}
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
			for (int j = 0; j < 4; j++) {
				ran_m[j] = j;
			}
			isWarn = false;
		}
	}
	else if (!isChange && !isRan && !isJump && read_intro) {
		isChange = true;
		isWarn = true;
		c_last_time = g_elapsed_time_ms;
		ranMove();
	}

	for (auto iter = monsterA.begin(); iter != monsterA.end(); iter++) {
		if (iter->isAlive) {
			isGone_A = false;
			break;
		}

		isGone_A = true;
	}

	if (isGone_A) {
		if (!isOnce_A) {
			isOnce_A = true;
			monsterA_waiting = g_elapsed_time_ms;
		}
		if(g_elapsed_time_ms- monsterA_waiting > 2000)
			responeMonster(true);
	}

	for (auto iter = monsterB.begin(); iter != monsterB.end(); iter++) {
		if (iter->isAlive) {
			isGone_B = false;
			break;
		}

		isGone_B = true;
	}

	if (isGone_B) {
		if (!isOnce_B) {
			isOnce_B= true;
			monsterB_waiting = g_elapsed_time_ms;
		}
		if (g_elapsed_time_ms - monsterB_waiting > 2000) {
			responeMonster(false);
		}
	}
	


	if (g_player_state[0])
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

	if (g_player_state[1])
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

	if (g_player_state[2])
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

	if (g_player_state[3])
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
		if (g_player_state[0] && (g_destination_rectangle_player.x > IndextoX(641)))
			g_destination_rectangle_player.x -= 5;
		else if (g_player_state[1] && (g_destination_rectangle_player.x < IndextoX(670)))
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

	if (isShot) {
		if (pointer_head == 0 && (g_destination_rectangle_pointer.x < IndextoX(670)) && (pointer_Pos.x + 100 > g_destination_rectangle_pointer.x))
			g_destination_rectangle_pointer.x += 10;
		else if (pointer_head == 1 && (g_destination_rectangle_pointer.x > IndextoX(641)) && (pointer_Pos.x - 100 < g_destination_rectangle_pointer.x))
			g_destination_rectangle_pointer.x -= 10;
		else
			isShot = false;
	}

	// collision detection
	// (1) player - monster
	if (g_player_unbeatable) {
		g_unbeatable_flag++;
	}
	if (g_unbeatable_flag > 40) {
		g_player_unbeatable = false;
		g_unbeatable_flag = 0;
	}
	int left, right, top, bottom;
	for (auto iter = monsterA.begin(); iter != monsterA.end(); iter++) {
		if (iter->isAlive == false)
			continue;

		left = iter->pos.x;
		right = iter->pos.x + g_monster_size;
		top = iter->pos.y;
		bottom = iter->pos.y + g_monster_size;

		if (!((bottom < g_destination_rectangle_player.y) ||
			(top > g_destination_rectangle_player.y + g_player_height) ||
			(right < g_destination_rectangle_player.x) ||
			(left > g_destination_rectangle_player.x + 25)) &&
			g_player_unbeatable == false) {
			g_player_heart -= 2;
			Mix_PlayChannel(-1, g_attack_sound, 0);
			g_player_unbeatable = true;
			break;
		}
	}
	for (auto iter = monsterB.begin(); iter != monsterB.end(); iter++) {
		if (iter->isAlive == false)
			continue;

		left = iter->pos.x;
		right = iter->pos.x + g_monster_size;
		top = iter->pos.y;
		bottom = iter->pos.y + g_monster_size;

		if (!((bottom < g_destination_rectangle_player.y) ||
			(top > g_destination_rectangle_player.y + g_player_height) ||
			(right < g_destination_rectangle_player.x) ||
			(left > g_destination_rectangle_player.x + 25)) &&
			g_player_unbeatable == false) {
			g_player_heart -= 2;
			Mix_PlayChannel(-1, g_attack_sound, 0);
			g_player_unbeatable = true;
			break;
		}
	}
	// (2) pointer - monster
	if (isShot) {
		for (auto iter = monsterA.begin(); iter != monsterA.end(); iter++) {
			if (iter->isAlive == false)
				continue;

			left = iter->pos.x + 10;
			right = iter->pos.x + g_monster_size - 10;
			top = iter->pos.y + 10;
			bottom = iter->pos.y + g_monster_size - 10;

			if (!((bottom < g_destination_rectangle_pointer.y) ||
				(top > g_destination_rectangle_pointer.y + 25) ||
				(right < g_destination_rectangle_pointer.x) ||
				(left > g_destination_rectangle_pointer.x + 25))) {
				Mix_PlayChannel(-1, g_hit_sound, 0);
				iter->isAlive = false;
				isShot = false;
				break;
			}
		}
	}
	if (isShot) {
		for (auto iter = monsterB.begin(); iter != monsterB.end(); iter++) {
			if (iter->isAlive == false)
				continue;

			left = iter->pos.x + 10;
			right = iter->pos.x + g_monster_size - 10;
			top = iter->pos.y + 10;
			bottom = iter->pos.y + g_monster_size - 10;

			if (!((bottom < g_destination_rectangle_pointer.y) ||
				(top > g_destination_rectangle_pointer.y + 25) ||
				(right < g_destination_rectangle_pointer.x) ||
				(left > g_destination_rectangle_pointer.x + 25))) {
				Mix_PlayChannel(-1, g_hit_sound, 0);
				iter->isAlive = false;
				isShot = false;
				break;
			}
		}
	}

	for (int i = 0; i < hole.size(); i++) {
		if ((g_destination_rectangle_player.x == hole[i].x) &&
			(g_destination_rectangle_player.y + g_destination_rectangle_player.h == hole[i].y))
		{
			isFall = true;
			break;
		}
	}

	if (isFall)
	{
		g_destination_rectangle_player.y += 10;
		for (int i = 0; i < ground.size(); i++) {
			if ((g_destination_rectangle_player.x == ground[i].x) &&
				(g_destination_rectangle_player.y + g_destination_rectangle_player.h == ground[i].y))
			{
				Mix_PlayChannel(-1, g_attack_sound, 0);
				g_player_heart -= 1;
				g_player_unbeatable = true;
				isFall = false;
				break;
			}
		}
	}

	if (g_player_heart <= 0) {
		g_game_ending = 0;
		g_current_game_phase = PHASE_ENDING;
		Mix_HaltMusic();
		Mix_PlayChannel(-1, g_failure_sound, 0);
		Reset_Stage3();
	}

	srand(time(NULL));
	if (isMove) {
		isMove = false;

		Pos b_prevRan_Pos;
		b_prevRan_Pos.x = b_destination_rect.x;
		b_prevRan_Pos.y = b_destination_rect.y;

		ran = rand() % 4;
		b_destination_rect.x = ranPos[ran].x;
		b_destination_rect.y = ranPos[ran].y;

		ranPos.erase(ranPos.begin() + ran, ranPos.begin() + ran + 1);
		ranPos.push_back(b_prevRan_Pos);
	}

	for (auto iter = monsterA.begin(); iter != monsterA.end(); iter++) {
		if (iter->isAlive)
			iter->Move();
	}
	for (auto iter = monsterB.begin(); iter != monsterB.end(); iter++) {
		if (iter->isAlive)
			iter->Move();
	}

	for (auto iter = a_position.begin(); iter != a_position.end(); iter++) {
		b_attackDown_rect.x = iter->x;
		b_attackDown_rect.y = b_attackDown_destination.y;
		b_attackDown_rect.w = b_attackDown_destination.w;
		b_attackDown_rect.w = b_attackDown_destination.h;

		if (checkCollision(g_destination_rectangle_player, b_attackDown_rect) && g_player_unbeatable == false) {
			Mix_PlayChannel(-1, g_hit_sound, 0);
			g_player_heart -= 2;
			g_player_unbeatable = true;
		}
	}

	if (checkCollision(b_destination_rect, g_destination_rectangle_pointer)) {
		isMove = true;
		hp_B--;
		bHp_destination_rect.w = 750 * hp_B / 5;
	}

	if (hp_B == 0) {
		Reset_Stage3();
		g_current_game_phase = PHASE_ENDING;
		g_game_ending = 1;
		Mix_PlayMusic(g_bgm_ending, -1);
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

	//ladder
	for (int i = 0; i < 4; i++) {
		SDL_RenderCopy(g_renderer, g_texture_ladder, &g_source_rectangle_ladder, &g_destination_rectangle_ladder[i]);
	}

	// link
	SDL_RenderCopy(g_renderer, g_texture_link, &g_source_rectangle_link, &g_destination_rectangle_link);

	//boss Hp
	SDL_RenderCopy(g_renderer, g_texture_timeW, &g_source_rectangle_timeW, &g_destination_rectangle_timeW);
	SDL_RenderCopy(g_renderer, bHp_texture, &bHp_source_rect, &bHp_destination_rect);
	SDL_RenderCopy(g_renderer, b_sheet_texture, &b_face_source_rect, &b_face_destination_rect);

	// heart
	g_destination_rectangle_heart = { 650, 75, 25, 25 };
	int full_heart = g_player_heart / 2;
	int half_heart = g_player_heart % 2;
	int empty_heart = 10 - full_heart - half_heart;
	for (int i = 0; i < 10; i++)
	{
		if (full_heart) {
			SDL_RenderCopy(g_renderer, g_texture_heart, &g_source_rectangle_heart[0], &g_destination_rectangle_heart);
			full_heart--;
		}
		else if (half_heart) {
			SDL_RenderCopy(g_renderer, g_texture_heart, &g_source_rectangle_heart[1], &g_destination_rectangle_heart);
			half_heart--;
		}
		else
			SDL_RenderCopy(g_renderer, g_texture_heart, &g_source_rectangle_heart[2], &g_destination_rectangle_heart);

		g_destination_rectangle_heart.x += 25;
		if (g_destination_rectangle_heart.x > 750) {
			g_destination_rectangle_heart.x = 650;
			g_destination_rectangle_heart.y = 100;
		}
	}

	//random Move Player
	SDL_Rect warning_ran;
	warning_ran.x = 100;
	warning_ran.y = 100;
	warning_ran.w = warn_source.w / 2;
	warning_ran.h = warn_source.h / 2;
	if(isWarn)
		SDL_RenderCopy(g_renderer, warnChange, &warn_source, &warning_ran);

	//pointer
	//SDL_RenderCopy(g_renderer, g_texture_pointer50, &g_source_rectangle_pointer50, &g_destination_rectangle_pointer50);

	if (isShot) {
		if (pointer_head == 0)
			SDL_RenderCopy(g_renderer, g_texture_pointer, &g_source_rectangle_pointer[0], &g_destination_rectangle_pointer);
		else if (pointer_head == 1)
			SDL_RenderCopy(g_renderer, g_texture_pointer, &g_source_rectangle_pointer[1], &g_destination_rectangle_pointer);
	}

	//sub_Monster
	for (auto iter = monsterA.begin(); iter != monsterA.end(); iter++) {
		if (iter->isAlive == false)
			continue;

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
		if (iter->isAlive == false)
			continue;
		g_destination_rectangle_monster.x = iter->pos.x;
		g_destination_rectangle_monster.y = iter->pos.y;
		g_destination_rectangle_monster.w = g_monster_size;
		g_destination_rectangle_monster.h = g_monster_size;

		if (iter->isRight)
			SDL_RenderCopy(g_renderer, g_texture_monster, &g_source_rectangle_monster[2], &g_destination_rectangle_monster);
		else
			SDL_RenderCopy(g_renderer, g_texture_monster, &g_source_rectangle_monster[3], &g_destination_rectangle_monster);
	}

	// player
	if (g_player_unbeatable) {
		if (g_unbeatable_flag % 10 == 0) {
			SDL_SetTextureColorMod(g_texture_player, 255, 255, 255);
			SDL_SetTextureAlphaMod(g_texture_player, 255);
		}
		else {
			SDL_SetTextureColorMod(g_texture_player, 255, 0, 0);
			SDL_SetTextureAlphaMod(g_texture_player, 170);
		}
	}
	else {
		SDL_SetTextureColorMod(g_texture_player, 255, 255, 255);
		SDL_SetTextureAlphaMod(g_texture_player, 255);
	}
	if (onladder) {
		SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[9], &g_destination_rectangle_player);
	}
	else {
		if (g_player_state[1]) {
			if (g_running_flag <= 10) {
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[2], &g_destination_rectangle_player);
				g_running_flag++;
			}
			else {
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[3], &g_destination_rectangle_player);
				g_running_flag++;
				if (g_running_flag > 20)
					g_running_flag = 1;
			}
		}
		else if (g_player_state[0]) {
			if (g_running_flag <= 10) {
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[6], &g_destination_rectangle_player);
				g_running_flag++;
			}
			else {
				SDL_RenderCopy(g_renderer, g_texture_player, &g_source_rectangle_player[7], &g_destination_rectangle_player);
				g_running_flag++;
				if (g_running_flag > 20)
					g_running_flag = 1;
			}
		}
		else if (isShot) {
			if (g_player_head == 0)
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

	//boss
	SDL_RenderCopy(g_renderer, b_sheet_texture, &b_source_rect, &b_destination_rect);

	if (isTime) {
		isDown = true;

		if (b_attackDown_destination.y < 600) {
			for (auto iter = a_position.begin(); iter != a_position.end(); iter++) {
				b_attackDown_destination.x = iter->x;
				SDL_RenderCopy(g_renderer, boxx_attack_texture, &boss_attack_source, &b_attackDown_destination);
			}
		}

		if (g_elapsed_time_ms - g_last_time_ms > attackCool_Down) {
			isDown = false;
		}
		else if (g_elapsed_time_ms - g_last_time_ms > attackCool_Down - 1000) {
			isEffect = false;
		}
	}

	// intro
	if (!read_intro)
		SDL_RenderCopy(g_renderer, g_texture_stage3Intro, &g_source_rectangle_stage3Intro, &g_destination_rectangle_stage3Intro);

	SDL_RenderPresent(g_renderer);
}



void Clear_Stage3()
{
	SDL_DestroyTexture(g_texture_player);
	SDL_DestroyTexture(g_texture_ladder);
	SDL_DestroyTexture(g_texture_monster);
	SDL_DestroyTexture(g_texture_pointer);
	SDL_DestroyTexture(g_texture_pointer50);
	SDL_DestroyTexture(g_texture_heart);
	SDL_DestroyTexture(g_texture_link);
	SDL_DestroyTexture(bHp_texture);
	SDL_DestroyTexture(g_texture_timeW);
	Mix_FreeChunk(g_shooting_sound);
	Mix_FreeChunk(g_hit_sound);
	Mix_FreeChunk(g_attack_sound);
	Mix_FreeChunk(g_failure_sound);
	Mix_FreeMusic(g_bgm_ending);
}


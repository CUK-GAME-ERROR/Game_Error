#include "GameFunc.h"
#include "Stage1.h"
#include <string>

static SDL_Texture* bg_texture_stage1_; // the SDL_Texture 
static SDL_Rect bg_source_rectangle_stage1_; // the rectangle for source image
static SDL_Rect bg_destination_rectangle_stage1_; // for destination

static SDL_Texture* character_stage1_;
static SDL_Rect character_source_rectangle_stage1_[8];
static SDL_Rect character_destination_rectangle_stage1_;

static SDL_Rect escape_;

static TTF_Font* hint_font_;
static SDL_Texture* hint_id_;
static SDL_Rect hint_id_texture_rectangle_;
static SDL_Rect hint_id_destination_rectangle_;
static SDL_Texture* hint_password_;
static SDL_Rect hint_password_texture_rectangle_;
static SDL_Rect hint_password_destination_rectangle_;
static SDL_Texture* hint_fake_;
static SDL_Rect hint_fake_texture_rectangle_;
static SDL_Rect hint_fake_destination_rectangle_;

static SDL_Texture* answer_id_texture_;
static SDL_Rect answer_id_texture_rectangle_;
static SDL_Rect answer_id_destination_rectangle_;

static SDL_Texture* answer_password_texture_;
static SDL_Rect answer_password_texture_rectangle_;
static SDL_Rect answer_password_destination_rectangle_;

static SDL_Texture* typing_;
static SDL_Rect typing_source_rectangle_;
static SDL_Rect typing_destination_rectangle_[2];

static int character_state_;
static int character_state_before_;
static int character_num;

static int left_;
static int right_;
static int cnt_[2];
static bool lup_;
static bool rup_;

static bool interact[6];
static bool typing;

static int alpha;

static std::string answer_id_ = "";
static std::string answer_password_ = "";

static SDL_Color red;
static SDL_Color black;

static Mix_Music* g_bgm_stage2;


void Init_Stage1()
{
	character_state_ = 0;
	character_state_before_ = 4;
	left_ = 5;
	right_ = 2;
	cnt_[0] = 0;
	cnt_[1] = 0;
	alpha = 255;
	lup_ = true;
	rup_ = true;
	for (int i = 0; i < 4; i++)
		interact[i] = false;
	typing = false;

	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/stage1.png");
	bg_texture_stage1_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(bg_texture_stage1_, NULL, NULL, &bg_source_rectangle_stage1_.w, &bg_source_rectangle_stage1_.h);
	bg_source_rectangle_stage1_.x = 0;
	bg_source_rectangle_stage1_.y = 0;
	bg_destination_rectangle_stage1_.x = 0;
	bg_destination_rectangle_stage1_.y = 0;
	bg_destination_rectangle_stage1_.w = bg_source_rectangle_stage1_.w;
	bg_destination_rectangle_stage1_.h = bg_source_rectangle_stage1_.h;

	SDL_Surface* temp_ch_surface = IMG_Load("../../Resources/Player.png");
	character_stage1_ = SDL_CreateTextureFromSurface(g_renderer, temp_ch_surface);
	SDL_FreeSurface(temp_ch_surface);

	character_source_rectangle_stage1_[0] = { 0, 0, 500, 500 };
	character_source_rectangle_stage1_[2] = { 500, 0, 500, 500 };
	character_source_rectangle_stage1_[4] = { 1000, 0, 500, 500 };
	character_source_rectangle_stage1_[5] = { 0, 500, 500, 500 };
	character_source_rectangle_stage1_[7] = { 500, 500, 500, 500 };
	character_source_rectangle_stage1_[6] = { 1000, 500, 500, 500 };
	character_source_rectangle_stage1_[3] = { 0, 1000, 500, 500 };
	character_source_rectangle_stage1_[1] = { 500, 1000, 500, 500 };

	character_destination_rectangle_stage1_.x = 200;
	character_destination_rectangle_stage1_.y = 400;
	character_destination_rectangle_stage1_.w = 60;
	character_destination_rectangle_stage1_.h = 60;

	escape_ = { 706, 610, 80, 0 };

	hint_font_ = TTF_OpenFont("../../Resources/DungGeunMo.ttf", 20);
	red = { 255, 0, 0, 0 };
	black = { 0, 0, 0, 0 };

	SDL_Surface* tmp_hint_id_surface = TTF_RenderText_Blended(hint_font_, "Hint : Your game name", red);
	hint_id_ = SDL_CreateTextureFromSurface(g_renderer, tmp_hint_id_surface);
	hint_id_texture_rectangle_.x = hint_id_texture_rectangle_.y = 0;
	hint_id_texture_rectangle_.w = tmp_hint_id_surface->w;
	hint_id_texture_rectangle_.h = tmp_hint_id_surface->h;
	SDL_FreeSurface(tmp_hint_id_surface);
	hint_id_destination_rectangle_.x = 170;
	hint_id_destination_rectangle_.y = 615;
	hint_id_destination_rectangle_.w = hint_id_texture_rectangle_.w;
	hint_id_destination_rectangle_.h = hint_id_texture_rectangle_.h;

	SDL_Surface* tmp_hint_password_surface = TTF_RenderText_Blended(hint_font_, "Hint : Your enemy", red);
	hint_password_ = SDL_CreateTextureFromSurface(g_renderer, tmp_hint_password_surface);
	hint_password_texture_rectangle_.x = hint_password_texture_rectangle_.y = 0;
	hint_password_texture_rectangle_.w = tmp_hint_password_surface->w;
	hint_password_texture_rectangle_.h = tmp_hint_password_surface->h;
	SDL_FreeSurface(tmp_hint_password_surface);
	hint_password_destination_rectangle_.x = 300;
	hint_password_destination_rectangle_.y = 195;
	hint_password_destination_rectangle_.w = hint_password_texture_rectangle_.w;
	hint_password_destination_rectangle_.h = hint_password_texture_rectangle_.h;

	SDL_Surface* tmp_hint_fake_surface = TTF_RenderText_Blended(hint_font_, "This is fake. Hint : address", red);
	hint_fake_ = SDL_CreateTextureFromSurface(g_renderer, tmp_hint_fake_surface);
	hint_fake_texture_rectangle_.x = hint_fake_texture_rectangle_.y = 0;
	hint_fake_texture_rectangle_.w = tmp_hint_fake_surface->w;
	hint_fake_texture_rectangle_.h = tmp_hint_fake_surface->h;
	SDL_FreeSurface(tmp_hint_fake_surface);
	hint_fake_destination_rectangle_.x = 420;
	hint_fake_destination_rectangle_.y = 615;
	hint_fake_destination_rectangle_.w = hint_fake_texture_rectangle_.w;
	hint_fake_destination_rectangle_.h = hint_fake_texture_rectangle_.h;

	SDL_Surface* tmp_typing_surface = TTF_RenderText_Blended(hint_font_, "Try typing!(limit : 15)", black);
	typing_ = SDL_CreateTextureFromSurface(g_renderer, tmp_typing_surface);
	typing_source_rectangle_.x = typing_source_rectangle_.y = 0;
	typing_source_rectangle_.w = tmp_typing_surface->w;
	typing_source_rectangle_.h = tmp_typing_surface->h;
	SDL_FreeSurface(tmp_typing_surface);
	SDL_SetTextureAlphaMod(typing_, 100);
	typing_destination_rectangle_[0].w = typing_destination_rectangle_[1].w = typing_source_rectangle_.w;
	typing_destination_rectangle_[0].h = typing_destination_rectangle_[1].h = typing_source_rectangle_.h;
	typing_destination_rectangle_[0].x = 420;
	typing_destination_rectangle_[0].y = 440;
	typing_destination_rectangle_[1].x = 420;
	typing_destination_rectangle_[1].y = 500;

	SDL_StartTextInput();

	g_bgm_stage2 = Mix_LoadMUS("../../Resources/stage2.mp3");
}

void Update_Answer()
{

	if (interact[3]) {
		if (answer_id_texture_)
			SDL_DestroyTexture(answer_id_texture_);
		if (answer_id_.length() > 0) {
			char tmp[16] = "";
			strcpy(tmp, answer_id_.c_str());
			SDL_Surface* tmp_answer_id_ = TTF_RenderText_Blended(hint_font_, tmp, black);
			answer_id_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_answer_id_);
			answer_id_texture_rectangle_.x = answer_id_texture_rectangle_.y = 0;
			answer_id_texture_rectangle_.w = tmp_answer_id_->w;
			answer_id_texture_rectangle_.h = tmp_answer_id_->h;
			SDL_FreeSurface(tmp_answer_id_);
			answer_id_destination_rectangle_.x = 420;
			answer_id_destination_rectangle_.y = 440;
			answer_id_destination_rectangle_.w = answer_id_texture_rectangle_.w;
			answer_id_destination_rectangle_.h = answer_id_texture_rectangle_.h;
		}
	}
	if (interact[4]) {
		if (answer_password_texture_)
			SDL_DestroyTexture(answer_password_texture_);
		if (answer_password_.length() > 0) {
			char tmp[16] = "";
			strcpy(tmp, answer_password_.c_str());
			SDL_Surface* tmp_answer_password_ = TTF_RenderText_Blended(hint_font_, tmp, black);
			answer_password_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_answer_password_);
			answer_password_texture_rectangle_.x = answer_id_texture_rectangle_.y = 0;
			answer_password_texture_rectangle_.w = tmp_answer_password_->w;
			answer_password_texture_rectangle_.h = tmp_answer_password_->h;
			SDL_FreeSurface(tmp_answer_password_);
			answer_password_destination_rectangle_.x = 420;
			answer_password_destination_rectangle_.y = 500;
			answer_password_destination_rectangle_.w = answer_password_texture_rectangle_.w;
			answer_password_destination_rectangle_.h = answer_password_texture_rectangle_.h;
		}
	}
}

void Update_Stage1()
{
	if (character_state_ == 1)
	{
		character_destination_rectangle_stage1_.x -= 8;
		if (character_destination_rectangle_stage1_.x <= 10)
			character_destination_rectangle_stage1_.x = 10;
		if (character_destination_rectangle_stage1_.x > 350 && character_destination_rectangle_stage1_.x < 733 && character_destination_rectangle_stage1_.y > 378 && character_destination_rectangle_stage1_.y < 525)
			character_destination_rectangle_stage1_.x = 733;
	}
	else if (character_state_ == 2)
	{
		character_destination_rectangle_stage1_.x += 8;
		if (character_destination_rectangle_stage1_.x >= 733)
			character_destination_rectangle_stage1_.x = 733;
		if (character_destination_rectangle_stage1_.x > 350 && character_destination_rectangle_stage1_.x < 733 && character_destination_rectangle_stage1_.y > 378 && character_destination_rectangle_stage1_.y < 525)
			character_destination_rectangle_stage1_.x = 350;
	}
	else if (character_state_ == 3)
	{
		character_destination_rectangle_stage1_.y -= 8;
		if (character_destination_rectangle_stage1_.y <= 118)
			character_destination_rectangle_stage1_.y = 118;
		if (character_destination_rectangle_stage1_.x > 350 && character_destination_rectangle_stage1_.x < 733 && character_destination_rectangle_stage1_.y > 378 && character_destination_rectangle_stage1_.y < 525)
			character_destination_rectangle_stage1_.y = 525;
	}
	else if (character_state_ == 4)
	{
		character_destination_rectangle_stage1_.y += 8;
		if (character_destination_rectangle_stage1_.y >= 633)
			character_destination_rectangle_stage1_.y = 633;
		if (character_destination_rectangle_stage1_.x > 350 && character_destination_rectangle_stage1_.x < 733 && character_destination_rectangle_stage1_.y > 378 && character_destination_rectangle_stage1_.y < 525)
			character_destination_rectangle_stage1_.y = 378;
	}
	if (interact[5]) {
		if (escape_.h < 80)
			escape_.h += 10;
		else
		{
			escape_.h = 80;
			if (character_destination_rectangle_stage1_.y < 633)
				character_state_ = 4;
			else if (character_destination_rectangle_stage1_.x < 720)
				character_state_ = 2;
			else if (character_destination_rectangle_stage1_.x > 730)
				character_state_ = 1;
			else {
				character_state_ = 5;
				alpha -= 40;
				if (alpha <= 0) {
					alpha = 0;
					SDL_StopTextInput();
					g_current_game_phase = PHASE_STAGE2;
					Mix_PlayMusic(g_bgm_stage2, -1);
				}
			}
		}
	}
}

void Render_Stage1()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	SDL_RenderCopy(g_renderer, bg_texture_stage1_, &bg_source_rectangle_stage1_, &bg_destination_rectangle_stage1_);

	if (interact[0])
		SDL_RenderCopy(g_renderer, hint_id_, &hint_id_texture_rectangle_, &hint_id_destination_rectangle_);
	if (interact[1])
		SDL_RenderCopy(g_renderer, hint_fake_, &hint_fake_texture_rectangle_, &hint_fake_destination_rectangle_);
	if (interact[2])
		SDL_RenderCopy(g_renderer, hint_password_, &hint_password_texture_rectangle_, &hint_password_destination_rectangle_);

	if (interact[5])
	{
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0);
		SDL_RenderFillRect(g_renderer, &escape_);
		SDL_SetTextureAlphaMod(character_stage1_, alpha);
	}

	if (character_state_ == 0) {
		cnt_[0] = cnt_[1] = 0;
		if (character_state_before_ == 1) {
			SDL_RenderCopy(g_renderer, character_stage1_, &character_source_rectangle_stage1_[6], &character_destination_rectangle_stage1_);
		}
		else if (character_state_before_ == 2) {
			SDL_RenderCopy(g_renderer, character_stage1_, &character_source_rectangle_stage1_[3], &character_destination_rectangle_stage1_);
		}
		else if (character_state_before_ == 3) {
			SDL_RenderCopy(g_renderer, character_stage1_, &character_source_rectangle_stage1_[1], &character_destination_rectangle_stage1_);
		}
		else if (character_state_before_ == 4) {
			SDL_RenderCopy(g_renderer, character_stage1_, &character_source_rectangle_stage1_[0], &character_destination_rectangle_stage1_);
		}
	}
	else if (character_state_ == 1) {
		SDL_RenderCopy(g_renderer, character_stage1_, &character_source_rectangle_stage1_[left_], &character_destination_rectangle_stage1_);
		if (cnt_[0] == 5) {
			if (left_ == 5) {
				left_ = 6;
				cnt_[0] = 0;
			}
			else if (left_ == 7) {
				left_ = 6;
				cnt_[0] = 0;
			}
			else {
				if (lup_) {
					left_ = 7;
					cnt_[0] = 0;
					lup_ = false;
				}
				else {
					left_ = 5;
					cnt_[0] = 0;
					lup_ = true;
				}
			}
		}
		cnt_[0]++;
	}
	else if (character_state_ == 2) {
		SDL_RenderCopy(g_renderer, character_stage1_, &character_source_rectangle_stage1_[right_], &character_destination_rectangle_stage1_);
		if (cnt_[1] == 5) {
			if (right_ == 2) {
				right_ = 3;
				cnt_[1] = 0;
			}
			else if (right_ == 4) {
				right_ = 3;
				cnt_[1] = 0;
			}
			else
			{
				if (rup_) {
					right_ = 4;
					cnt_[1] = 0;
					rup_ = false;
				}
				else {
					right_ = 2;
					cnt_[1] = 0;
					rup_ = true;
				}
			}
		}
		cnt_[1]++;
	}
	else if (character_state_ == 3 || character_state_ == 5) {
		SDL_RenderCopy(g_renderer, character_stage1_, &character_source_rectangle_stage1_[1], &character_destination_rectangle_stage1_);
	}
	else if (character_state_ == 4) {
		SDL_RenderCopy(g_renderer, character_stage1_, &character_source_rectangle_stage1_[0], &character_destination_rectangle_stage1_);
	}
	if (interact[3] && answer_id_.length() == 0)
		SDL_RenderCopy(g_renderer, typing_, &typing_source_rectangle_, &typing_destination_rectangle_[0]);
	if (interact[4] && answer_password_.length() == 0)
		SDL_RenderCopy(g_renderer, typing_, &typing_source_rectangle_, &typing_destination_rectangle_[1]);

	if (answer_id_.length() > 0)
		SDL_RenderCopy(g_renderer, answer_id_texture_, &answer_id_texture_rectangle_, &answer_id_destination_rectangle_);
	if (answer_password_.length() > 0)
		SDL_RenderCopy(g_renderer, answer_password_texture_, &answer_password_texture_rectangle_, &answer_password_destination_rectangle_);

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void HandleEvents_Stage1()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (!interact[5]) {
			switch (event.type)
			{
			case SDL_QUIT:
				g_flag_running = false;
				break;

			case SDL_KEYDOWN:
			case SDL_TEXTINPUT:
				if (interact[3] || interact[4]) {
					if (event.key.keysym.sym == SDLK_LEFT) {
						character_state_ = 1;
						interact[3] = interact[4] = false;
						typing = false;
					}
					else if (event.key.keysym.sym == SDLK_RIGHT) {
						character_state_ = 2;
						interact[3] = interact[4] = false;
						typing = false;
					}
					else if (event.key.keysym.sym == SDLK_UP) {
						character_state_ = 3;
						interact[3] = interact[4] = false;
						typing = false;
					}
					else if (event.key.keysym.sym == SDLK_DOWN) {
						character_state_ = 4;
						interact[3] = interact[4] = false;
						typing = false;
					}
					else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE)
					{
						if (interact[3] && answer_id_.length() > 0) {
							answer_id_ = answer_id_.substr(0, answer_id_.length() - 1);
							Update_Answer();
						}
						else if (interact[4] && answer_password_.length() > 0) {
							answer_password_ = answer_password_.substr(0, answer_password_.length() - 1);
							Update_Answer();
						}
					}
					else if (event.type == SDL_TEXTINPUT)
						if (interact[3] && answer_id_.length() < 15) {
							answer_id_ += event.text.text;
							Update_Answer();
						}
						else if (interact[4] && answer_password_.length() < 15) {
							answer_password_ += event.text.text;
							Update_Answer();
						}
				}
				else {
					if (event.key.keysym.sym == SDLK_LEFT) {
						character_state_ = 1;
					}
					else if (event.key.keysym.sym == SDLK_RIGHT) {
						character_state_ = 2;
					}
					else if (event.key.keysym.sym == SDLK_UP) {
						character_state_ = 3;
					}
					else if (event.key.keysym.sym == SDLK_DOWN) {
						character_state_ = 4;
					}
					else if (event.key.keysym.sym == SDLK_x)
					{
						if (character_destination_rectangle_stage1_.x >= 40 && character_destination_rectangle_stage1_.x <= 700 && character_destination_rectangle_stage1_.y >= 120 && character_destination_rectangle_stage1_.y <= 170) {
							if (!interact[2])
								interact[2] = true;
							else
								interact[2] = false;
						}
						if (character_destination_rectangle_stage1_.x >= 160 && character_destination_rectangle_stage1_.x <= 340 && character_destination_rectangle_stage1_.y >= 620) {
							if (!interact[0])
								interact[0] = true;
							else
								interact[0] = false;
						}
						if (character_destination_rectangle_stage1_.x >= 400 && character_destination_rectangle_stage1_.x <= 600 && character_destination_rectangle_stage1_.y >= 620) {
							if (!interact[1])
								interact[1] = true;
							else
								interact[1] = false;
						}
						if (character_destination_rectangle_stage1_.x >= 345 && character_destination_rectangle_stage1_.x <= 350 && character_destination_rectangle_stage1_.y >= 390 && character_destination_rectangle_stage1_.y <= 520)
							typing = true;
						if (character_destination_rectangle_stage1_.x >= 560 && character_destination_rectangle_stage1_.x <= 660 && character_destination_rectangle_stage1_.y <= 550 && character_destination_rectangle_stage1_.y >= 525) {
							char tmp_id[16] = "";
							char tmp_password[16] = "";
							strcpy(tmp_id, answer_id_.c_str());
							strcpy(tmp_password, answer_password_.c_str());
							if (strcmp(tmp_id, "error") == 0 && strcmp(tmp_password, "bug") == 0)
								interact[5] = true;
						}
					}
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_LEFT)
				{
					if (character_state_ == 1) {
						character_state_ = 0;
						character_state_before_ = 1;
						left_ = 5;
					}
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					if (character_state_ == 2) {
						character_state_ = 0;
						character_state_before_ = 2;
						right_ = 2;
					}
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
					if (character_state_ == 3) {
						character_state_ = 0;
						character_state_before_ = 3;
					}
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					if (character_state_ == 4) {
						character_state_ = 0;
						character_state_before_ = 4;
					}
				}
				else if (event.key.keysym.sym == SDLK_x)
				{
					if (typing) {
						if (character_destination_rectangle_stage1_.y <= 435)
							interact[3] = true;
						if (character_destination_rectangle_stage1_.y >= 465)
							interact[4] = true;
					}

				}
				break;
			default:
				break;
			}
		}
	}
}



void Clear_Stage1()
{
	SDL_DestroyTexture(bg_texture_stage1_);
	SDL_DestroyTexture(character_stage1_);
	SDL_DestroyTexture(hint_id_);
	SDL_DestroyTexture(hint_password_);
	SDL_DestroyTexture(hint_fake_);
	SDL_DestroyTexture(answer_id_texture_);
	SDL_DestroyTexture(answer_password_texture_);
	SDL_DestroyTexture(typing_);
	Mix_FreeMusic(g_bgm_stage2);
}


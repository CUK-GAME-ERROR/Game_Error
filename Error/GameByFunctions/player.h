#pragma once
#include <vector>
#include <list>

//player HP
//extern int hp;

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


//extern std::vector<Pos> map;

inline int IndextoX(int i)
{
	return (i % 32) * 25;
}

inline int IndextoY(int i)
{
	return (i / 32) * 25 + 125;
}

inline std::vector<Pos> Init_Map()
{
	static std::vector<Pos> map;

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
	for (int i = 422; i <= 430; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 434; i <= 446; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 545; i <= 553; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 558; i <= 562; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 567; i <= 574; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });

	return map;
}

inline bool checkCollision(SDL_Rect a, SDL_Rect b) {
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	if (bottomA <= topB) {
		return false;
	}
	else if (topA >= bottomB) {
		return false;
	}
	else if (rightA <= leftB) {
		return false;
	}
	else if (leftA >= rightB) {
		return false;
	}
	else {
		return true;
	}
}
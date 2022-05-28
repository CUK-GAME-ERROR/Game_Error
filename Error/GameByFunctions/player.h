#pragma once
#include <vector>
#include <list>
#include "SDL.h"

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
	bool isAlive;

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

	Monster(Pos p, float move, bool right, bool alive) {
		pos = p;
		init_pos = p;
		move_destination = move;
		isRight = right;
		isAlive = alive;
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

class Item {
public:
	Pos pos;
	int type; // 0:coffee 1:energydrink
	bool drink;

	Item() {
		pos.x = 0;
		pos.y = 0;
		type = 0;
		drink = false;
	}

	Item(Pos p, int t) {
		pos = p;
		type = t;
		drink = false;
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
	for (int i = 174; i <= 174; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 177; i <= 188; i++)
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
	for (int i = 557; i <= 563; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });
	for (int i = 567; i <= 574; i++)
		map.push_back({ IndextoX(i), IndextoY(i) });

	return map;
}

inline std::vector<Pos> Init_Ground()
{
	static std::vector<Pos> ground;

	for (int i = IndextoX(673); i <= IndextoX(702); i += 5)
		ground.push_back({ i, IndextoY(673) });
	for (int i = IndextoX(545); i < IndextoX(554); i += 5)
		ground.push_back({ i, IndextoY(545) });
	for (int i = IndextoX(556) + 5; i < IndextoX(564); i += 5)
		ground.push_back({ i, IndextoY(545) });
	for (int i = IndextoX(566) + 5; i <= IndextoX(574); i += 5)
		ground.push_back({ i, IndextoY(545) });
	for (int i = IndextoX(421) + 5; i < IndextoX(431); i += 5)
		ground.push_back({ i, IndextoY(417) });
	for (int i = IndextoX(433) + 5; i <= IndextoX(446); i += 5)
		ground.push_back({ i, IndextoY(417) });
	for (int i = IndextoX(289); i < IndextoX(295); i += 5)
		ground.push_back({ i, IndextoY(289) });
	for (int i = IndextoX(297) + 5; i < IndextoX(307); i += 5)
		ground.push_back({ i, IndextoY(289) });
	for (int i = IndextoX(309) + 5; i < IndextoX(313); i += 5)
		ground.push_back({ i, IndextoY(289) });
	for (int i = IndextoX(315) + 5; i <= IndextoX(318); i += 5)
		ground.push_back({ i, IndextoY(289) });
	for (int i = IndextoX(161); i < IndextoX(172); i += 5)
		ground.push_back({ i, IndextoY(161) });
	for (int i = IndextoX(173) + 5; i < IndextoX(175); i += 5)
		ground.push_back({ i, IndextoY(161) });
	for (int i = IndextoX(176) + 5; i < IndextoX(189); i += 5)
		ground.push_back({ i, IndextoY(161) });

	return ground;
}

inline std::vector<Pos> Init_Hole()
{
	static std::vector<Pos> hole;

	for (int i = IndextoX(554); i <= IndextoX(556); i += 5)
		hole.push_back({ i, IndextoY(545) });
	for (int i = IndextoX(564); i <= IndextoX(566); i += 5)
		hole.push_back({ i, IndextoY(545) });
	for (int i = IndextoX(417); i <= IndextoX(421); i += 5)
		hole.push_back({ i, IndextoY(417) });
	for (int i = IndextoX(431); i <= IndextoX(433); i += 5)
		hole.push_back({ i, IndextoY(417) });
	for (int i = IndextoX(295); i <= IndextoX(297); i += 5)
		hole.push_back({ i, IndextoY(289) });
	for (int i = IndextoX(307); i <= IndextoX(309); i += 5)
		hole.push_back({ i, IndextoY(289) });
	for (int i = IndextoX(313); i <= IndextoX(315); i += 5)
		hole.push_back({ i, IndextoY(289) });
	for (int i = IndextoX(172); i <= IndextoX(173); i += 5)
		hole.push_back({ i, IndextoY(161) });
	for (int i = IndextoX(175); i <= IndextoX(176); i += 5)
		hole.push_back({ i, IndextoY(161) });
	for (int i = IndextoX(189); i <= IndextoX(190); i += 5)
		hole.push_back({ i, IndextoY(161) });

	return hole;
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

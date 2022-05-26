#pragma once
#include <vector>
#include <list>

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
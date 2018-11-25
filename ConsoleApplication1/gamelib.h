#pragma once
#include "stdafx.h"

//const int DIVISOR = 8;

class Player;
class GameInfo {
public:
	std::map<std::tuple<int, int>, class Block*> map;
	Player* player;

	unsigned int visible_x_min;
	unsigned int visible_x_max;
	unsigned int visible_y_min;
	unsigned int visible_y_max;
};

class Block {
protected:
	GameInfo* gameinfo;
	bool visible() {
		if (gameinfo->visible_x_min <= x && x <= gameinfo->visible_x_max &&
			gameinfo->visible_y_min <= y && y <= gameinfo->visible_y_max) {
			return true;
		}
		return false;
	}
	int get_drawX() {
		//return x / DIVISOR;
		return (int)x;
	}
	int get_drawY() {
		//return y / DIVISOR;
		return (int)y;
	}
	Block* foot_tile();

public:
	double x;
	double y;

	virtual void update() = 0;
	virtual void collision(double old_x, double old_y, Block* block) = 0;
	virtual void draw() = 0;
	virtual bool jumpable() = 0;

	Block(GameInfo*, int x, int y);

};

class Ground : public Block {
public:
	void draw();
	void collision(double old_x, double old_y, Block* block);
	void update();
	bool jumpable();
	using Block::Block;
};

class Player : public Block {
private:
	double vel_x;
	double vel_y;
	double acc_x;
	double acc_y;

public:
	void draw();
	void collision(double old_x, double old_y, Block* block);
	void update();
	void jump(double vel);
	void setxvel(double vel);
	Player(GameInfo*, int x, int y);
	int jumping = 0;
	int max_jump = 1;
	bool jumpable() { return false; }
	void reset_vel();
	void hit();
};

class Enemy : public Block {
public:
	void draw();
	void collision(double old_x, double old_y, Block* block);
	void update();
	bool jumpable();
	Enemy(GameInfo*, int x, int y);
private:
	double vel_x;
	double vel_y;
};
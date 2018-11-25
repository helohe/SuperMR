#include "gamelib.h"

Block::Block(GameInfo* gameinfo, int x, int y) {
	this->gameinfo = gameinfo;
	//this->x = x * DIVISOR;
	//this->y = y * DIVISOR;
	this->x = x;
	this->y = y;
	gameinfo->map[{x, y}] = this;
}

void Ground::draw(){
	if (visible())
		TCODConsole::root->putChar(get_drawX(), get_drawY(), '=');
}
void Ground::update() {}
bool Ground::jumpable() { return true; }

void Ground::collision(double old_x, double old_y, Block* block) {
	if(typeid(*block) == typeid(Player)) {
		Player* player = (Player*)block;
		player->x = old_x;
		player->y = old_y;
		player->jumping = 0;
		player->reset_vel();
	}

}


Player::Player(GameInfo* gameinfo, int x, int y)
: Block(gameinfo, x, y){
	vel_x = 0;
	acc_x = 0;
	vel_y = 0;
	acc_y = 0.01;
	max_jump = 1;
}

void Player::draw() {
	if (visible())
		TCODConsole::root->putChar(get_drawX(), get_drawY(), '@');
}

void Player::reset_vel() {
	vel_y = 0;
	acc_y = 0.01;
	jumping = 0;
}

void Player::collision(double old_x, double old_y, Block* block) {}

void Player::update() {
	vel_x += acc_x;
	vel_y += acc_y;
	if (vel_x > 1) vel_x = 1;
	if (vel_x < -1) vel_x = -1;
	if (vel_y > 1) vel_y = 1;
	if (vel_y < -1) vel_y = -1;
	int old_x = x;
	int old_y = y;
	x += vel_x;
	y += vel_y;

	if (gameinfo->map.count({ (int)x, (int)y }) == 0) {
		gameinfo->map.erase({ old_x, old_y });
		gameinfo->map[{(int)x, (int)y}] = this;
	}
	else {
		Block* block = gameinfo->map[{ (int)x, (int)y }];
		if (block == this) return;
		else block->collision(old_x, old_y, this);
	}
}

Block* Block::foot_tile() {
	if (gameinfo->map.count({(int)x, (int)y+1}) != 0) {
		return gameinfo->map[{(int)x, (int)y+1}];
	}
	else {
		return 0;
	}
}

void Player::jump(double vel) {
	if (vel == 0 && vel_y < 0) {
		vel_y = 0;
		return;
	}
	if ((foot_tile() != 0 && foot_tile()->jumpable()) || (0 < jumping && jumping < max_jump)) {
		vel_y = vel;
		jumping++;
	}
}

void Player::setxvel(double vel) {
	vel_x = vel;
}

void Player::hit() {

}


Enemy::Enemy(GameInfo* gameinfo, int x, int y) : Block(gameinfo, x, y) {
	vel_x = -.05;
	vel_y = 0;
}

void Enemy::draw() {
	if (visible())
		TCODConsole::root->putChar(get_drawX(), get_drawY(), '^');
}
void Enemy::collision(double old_x, double old_y, Block* block) {
	if (block == gameinfo->player) {
		if ((int)old_y == (int)y - 1 && (int)old_x == (int)x) {
			gameinfo->map.erase({ (int)x, (int)y });
			gameinfo->map[{(int)x, (int)y}] = block;
			gameinfo->player->jumping = 0;
			gameinfo->player->jump(-0.5);
		}
		else {
			gameinfo->player->hit();
		}
	}
}
void Enemy::update() {
	int old_x = x;
	int old_y = y;
	if (foot_tile() != 0 && typeid(*foot_tile()) == typeid(Ground))
		x += vel_x;
	else {
		y += vel_y;
		vel_y += 0.01;
	}
		

		

	if (gameinfo->map.count({ (int)x, (int)y }) == 0) {
		gameinfo->map.erase({ old_x, old_y });
		gameinfo->map[{(int)x, (int)y}] = this;
	}
	else {
		Block* block = gameinfo->map[{ (int)x, (int)y }];
		if (block == this) return;
		else if (block == gameinfo->player) {
			((Player*)block)->hit();
		}
		else {
			y = old_y;
			x = old_x;
			vel_y = 0;
			vel_x = -vel_x;
		}
	}
}
bool Enemy::jumpable() {
	return true;
}
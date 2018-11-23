// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gamelib.h"

int main() {
	TCODConsole::initRoot(80, 50, "Super M", false);
	

	GameInfo gi = GameInfo();
	gi.visible_x_min = 0;
	gi.visible_x_max = 80;
	gi.visible_y_min = 0;
	gi.visible_y_max = 50;

	for (int i = 0; i < 80; i++) {
		new Ground(&gi, i, 40);
	}

	new Ground(&gi, 0, 39);
	new Ground(&gi, 79, 39);
	new Enemy(&gi, 12, 12);
	//Ground1 ground = Ground1(&gi, 40, 24);

	Player player = Player(&gi, 40, 10);
	player.max_jump = 2;
	gi.player = &player;

	while (!TCODConsole::isWindowClosed()) {
		TCOD_key_t key;
		TCOD_event_t ev = TCODSystem::checkForEvent(TCOD_EVENT_KEY, &key, NULL);

		TCODConsole::root->clear();
		//TCODConsole::root->putChar(40, 25, '@');

		if (ev == TCOD_EVENT_KEY_PRESS){
			if (key.vk == TCODK_SPACE)
				player.jump(-0.5);
			else if (key.vk == TCODK_LEFT)
				player.setxvel(-0.2);
			else if (key.vk == TCODK_RIGHT)
				player.setxvel(0.2);
		}
		else if (ev == TCOD_EVENT_KEY_RELEASE) {
			if (key.vk == TCODK_SPACE)
				player.jump(0);
			else if (key.vk == TCODK_LEFT)
				player.setxvel(0);
			else if (key.vk == TCODK_RIGHT)
				player.setxvel(0);
		}

		// update all (visible) blocks
		for (auto const&[key, val] : gi.map) {
			val->update();
		}

		// draw all (visible) blocks
		for (auto const&[key, val] : gi.map) {
			val->draw();
		}
		TCODConsole::flush();
		_sleep(10);
	}


	return 0;
}

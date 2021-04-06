#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	player = new Player();
	levels.resize(28);

	levels[0].init(player, 0);
	levels[1].init(player, 1);
	levels[2].init(player, 2);
	levels[3].init(player, 3);

	levels[4].init(player, 0);
	levels[5].init(player, 0);
	levels[6].init(player, 0);

	levels[7].init(player, 4);
	levels[8].init(player, 5);
	levels[9].init(player, 0);

	levels[10].init(player, 10);
	levels[11].init(player, 6);
	levels[12].init(player, 0);

	levels[13].init(player, 11);
	levels[14].init(player, 0);
	levels[15].init(player, 9);

	levels[16].init(player, 12);
	levels[17].init(player, 7);
	levels[18].init(player, 8);

	levels[19].init(player, 0);
	levels[20].init(player, 0);
	levels[21].init(player, 0);

	levels[22].init(player, 13);
	levels[23].init(player, 14);
	levels[24].init(player, 15);

	levels[25].init(player, 0);
	levels[26].init(player, 0);
	levels[27].init(player, 0);


	scene = 1;

	/*
	string levelPathFile = "levels/level00.txt";
	string enemiesLocationPathFile = "levels/level1_enemies.txt";
	string itemsLocationPathFile = "levels/level1_items.txt";
	scene.init(levelPathFile, enemiesLocationPathFile, itemsLocationPathFile);
	*/
}

bool Game::update(int deltaTime)
{	
	player->setScene(&levels[scene]);
	levels[scene].update(deltaTime);
	int out = player->isOut();
	if (out != 0) {
		scene += out;
	}
	player->teleport(scene);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	levels[scene].render();
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		bPlay = false;
	else if (key == 'g' || key == 'G')
		levels[scene].changePlayerGameMode();
	else if (key == '0') //BOOTS
		levels[scene].givePlayerPowerUp(0);
	else if (key == '1') //BOOK
		levels[scene].givePlayerPowerUp(0);
	else if (key == '2') //HELMET
		levels[scene].givePlayerPowerUp(0);
	else if (key == '3') //RAINCOAT
		levels[scene].givePlayerPowerUp(0);
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}






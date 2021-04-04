#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	string levelPathFile = "levels/level00.txt";
	string enemiesLocationPathFile = "levels/level1_enemies.txt";
	string itemsLocationPathFile = "levels/level1_items.txt";
	scene.init(levelPathFile, enemiesLocationPathFile, itemsLocationPathFile);
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		bPlay = false;
	else if (key == 'g' || key == 'G')
		scene.changePlayerGameMode();
	else if (key == '0') //BOOTS
		scene.givePlayerPowerUp(0);
	else if (key == '1') //BOOK
		scene.givePlayerPowerUp(0);
	else if (key == '2') //HELMET
		scene.givePlayerPowerUp(0);
	else if (key == '3') //RAINCOAT
		scene.givePlayerPowerUp(0);
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






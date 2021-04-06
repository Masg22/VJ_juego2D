#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "BaseEnemy.h"
#include "Game.h"

BaseEnemy::BaseEnemy() {
	Character::setPathToSpriteSheet("images/SkeletonAnimations.png");
	dir = glm::ivec2(0, 0);
	isAttacking = false;
}

void BaseEnemy::init(glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Scene* scene)
{
	Character::init(tileMapPos, shaderProgram, scene);
}

void BaseEnemy::update(int deltaTime)
{
	if (scene->playerHits(this))
		isDead = true;

	Character::update(deltaTime);
}

void BaseEnemy::die() {
	isDead = true;
}

BaseEnemy::~BaseEnemy()
{
}
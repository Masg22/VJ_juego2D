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

void BaseEnemy::init(ShaderProgram& shaderProgram, Scene* scene)
{
	Character::init(shaderProgram, scene);
	/*
	spritesheet.loadFromFile("images/SkeletonAnimations.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	*/
}

void BaseEnemy::update(int deltaTime)
{
	if (scene->playerHits(this))
		isDead = true;

	Character::update(deltaTime);
}
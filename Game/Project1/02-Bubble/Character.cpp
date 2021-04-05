#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Character.h"
#include "Game.h"

#define DAMAGED_RATE 6
#define DAMAGED_TIME 40

Character::Character() {
	mStartAnimation = 1;
	mNumberAnimations = 4;
	
	mSpriteColumns = 10;
	mSpriteRows = 6;

	isDead = false;
	inmunytyFrames = 0;
}

void Character::init(glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Scene* scene) {
	this->scene = scene;

	tileMapDispl = tileMapPos;

	bJumping = false;
	bAttacking = false;

	float columnSize = 1.f / float(mSpriteColumns);
	float rowSize = 1.f / float(mSpriteRows);

	spritesheet.loadFromFile(pathToSpritesheet, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(columnSize, rowSize), &spritesheet, &shaderProgram);
}

void Character::setPathToSpriteSheet(string path) {
	pathToSpritesheet = path;
}

void Character::update(int deltaTime) {
	if (inmunytyFrames > 0) {
		if (inmunytyFrames % DAMAGED_RATE == 0) sprite->setIsDamaged(1.0f); //animacion de daño
		else sprite->setIsDamaged(0.0f);
		//knockback ¿?
		--inmunytyFrames;
	}

	sprite->update(deltaTime);
	//sprite->setPosition(glm::vec2(float(posCharacter.x), float(posCharacter.y))); //knockback ¿?
}

void Character::render() {
	sprite->render();
}


void Character::setPosition(const glm::vec2& pos) {
	posCharacter = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Character::damage() {
	if (inmunytyFrames == 0) {
		inmunytyFrames = DAMAGED_TIME;
	}
}
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Scene.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 90
#define FALL_STEP 4
#define MOVEMENT_DEFAULT 2


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, ATTACK_LEFT, ATTACK_RIGHT, CLIMB_STATIC, CLIMB_ACTIVE
};

void Player::init(ShaderProgram& shaderProgram, Scene* scene)
{
	Character::init(shaderProgram, scene);
	sprite->setNumberAnimations(10);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.25f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.50f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50f, 0.25f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.75f, 0.5f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.75f, 0.25f));

		sprite->setAnimationSpeed(ATTACK_LEFT, 8);
		sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.5f, 0.0f));

		sprite->setAnimationSpeed(ATTACK_RIGHT, 8);
		sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.75f, 0.0f));

		sprite->setAnimationSpeed(CLIMB_STATIC, 8);
		sprite->addKeyframe(CLIMB_STATIC, glm::vec2(0.0f, 0.0f));

		sprite->setAnimationSpeed(CLIMB_ACTIVE, 8);
		sprite->addKeyframe(CLIMB_ACTIVE, glm::vec2(0.0f, 0.0f));
		sprite->addKeyframe(CLIMB_ACTIVE, glm::vec2(0.25f, 0.0f));
		
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(posCharacter.x), float(posCharacter.y)));
}

void Player::update(int deltaTime) {
	if (!bAnimating) {
		computeNextMove(deltaTime);
	}
	else {
		computeAnimation(deltaTime);
	}
	Character::update(deltaTime);
}
/*
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
			if (bJumping) {
				sprite->changeAnimation(JUMP_LEFT);
			}
		}
		if (bJumping) {
			sprite->changeAnimation(JUMP_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
			if (bJumping) {
				sprite->changeAnimation(JUMP_LEFT);
			}
		}
		if (bJumping) {
			sprite->changeAnimation(JUMP_RIGHT);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle < 90 && map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				jumpAngle = 90;
			}
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);

			//cambiar animación aqui salto
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			//cambiar animación aqui salto
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}
*/

void Player::computeNextMove(int deltaTime) {
	computeAttack(deltaTime);
	computeMovement();
	computeJump();
}

void Player::computeAttack(int deltaTime) {
	if (Game::instance().getKey(' ')) {
		if (!bAttacking) {
			attackTime = 0;
			//sound here
			if (sprite->animation() % 2 == 0) {
				//LEFT
				sprite->changeAnimation(ATTACK_LEFT);
				bAttacking = true;
			}
			else {
				//RIGHT
				sprite->changeAnimation(ATTACK_RIGHT);
				bAttacking = true;
			}
		}
		else {
			attackTime += deltaTime;
			if (attackTime > attackSoundTime) {
				if (sprite->animation() % 2 == 0) {
					sprite->changeAnimation(STAND_LEFT);
					bAttacking = false;
				}
				else {
					sprite->changeAnimation(STAND_RIGHT);
					bAttacking = false;
				}
			}
		}
	}
	else {
		//Game::instance().stopSound(getAttackSound());
		bAttacking = false;
		if (sprite->animation() == ATTACK_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == ATTACK_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
}

void Player::computeMovement() {
	if (bAttacking) return;
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		if (bJumping && sprite->animation() != JUMP_LEFT) {
			sprite->changeAnimation(JUMP_LEFT);
		}
		else if (!bJumping && sprite->animation() != MOVE_LEFT) {
			sprite->changeAnimation(MOVE_LEFT);
		}
		posCharacter.x -= MOVEMENT_DEFAULT;
		if (scene->collisionMoveLeft(this)) {
			posCharacter.x += MOVEMENT_DEFAULT;
			if (!bJumping) {
				sprite->changeAnimation(STAND_LEFT);
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		if (bJumping && sprite->animation() != JUMP_RIGHT) {
			sprite->changeAnimation(JUMP_RIGHT);
		}
		else if (!bJumping && sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
		}
		posCharacter.x += MOVEMENT_DEFAULT;
		if (scene->collisionMoveRight(this)) {
			posCharacter.x -= MOVEMENT_DEFAULT;
			if (!bJumping) {
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}
	else {
		//Stops walking.
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
}

void Player::computeJump() {
	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP;
		if (scene->collisionMoveDown(this) || jumpAngle == 180) {
			bJumping = false;
		}
		else { // up 0 <= jumpAngle <= 90 , down 90<jumpAngle<=180
			posCharacter.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

			if (scene->collisionMoveUp(this)) {
				posCharacter.y = int(startY - JUMP_HEIGHT * sin(3.14159f * (jumpAngle - JUMP_ANGLE_STEP) / 180.f));
				bJumping = false;
			}
			else if (jumpAngle > 90) {
				bJumping = !scene->collisionMoveDown(this);
			}
		}
	}
	else {
		posCharacter.y += FALL_STEP;
		if (scene->collisionMoveDown(this)){
			if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				if (!bAttacking) {
					if (sprite->animation() % 2 == 0) {
						sprite->changeAnimation(JUMP_LEFT);
					}
					else {
						sprite->changeAnimation(JUMP_LEFT);
					}
					//sound
					bJumping = true;
					jumpAngle = 0;
					startY = posCharacter.y;
				}
			}
		}
	}
}

Player::~Player()
{
}





#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Scene.h"


#define JUMP_ANGLE_STEP 3
#define JUMP_HEIGHT 45
#define FALL_STEP 3
#define MOVEMENT_DEFAULT 2
#define MOVEMENT_SPEED 3
#define HEALTH 100


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, ATTACK_LEFT, ATTACK_RIGHT, CLIMB_STATIC, CLIMB_ACTIVE
};

enum PowerUps {
	BOOTS, BOOK, HELMET, RAINCOAT
};

Player::Player() {

	setPathToSpriteSheet("images/PlayerAnimations.png");

	mNumberAnimations = 10;

	bAnimating = false;
	mSpriteRows = 4;
	mSpriteColumns = 4;

	maxHealth = HEALTH;
	actHealth = HEALTH;

	attackTime = 0;

	godMode = false;
	powerUps[BOOTS] = false;
	powerUps[BOOK] = false;
	powerUps[HELMET] = false;
	powerUps[RAINCOAT] = false;

}

void Player::init(glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Scene* scene)
{
	Character::init(tileMapPos, shaderProgram, scene);
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

	initTeleports();
}

void Player::update(int deltaTime) {
	if (teleporting) {}
	else if (!bAnimating) {
		computeNextMove(deltaTime);
	}
	Character::update(deltaTime);
}

void Player::computeNextMove(int deltaTime) {
	computeAttack(deltaTime);
	computeMovement(deltaTime);
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

void Player::computeMovement(int deltaTime) {
	if (teleporting) {
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (lianaClimb) {
			lianaClimb = false;
		}
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		powerUps[BOOTS] ? posCharacter.x -= MOVEMENT_SPEED : posCharacter.x -= MOVEMENT_DEFAULT;
		if (scene->collisionMoveLeft(posCharacter, glm::ivec2(32, 32)))
		{
			powerUps[BOOTS] ? posCharacter.x += MOVEMENT_SPEED : posCharacter.x += MOVEMENT_DEFAULT;
			sprite->changeAnimation(STAND_LEFT);
			if (bJumping) {
				sprite->changeAnimation(JUMP_LEFT);
			}
		}
		if (bJumping) {
			sprite->changeAnimation(JUMP_LEFT);
		}

	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (lianaClimb) {
			lianaClimb = false;
		}
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		powerUps[BOOTS] ? posCharacter.x += MOVEMENT_SPEED : posCharacter.x += MOVEMENT_DEFAULT;

		if (scene->collisionMoveRight(posCharacter, glm::ivec2(32, 32)))
		{
			powerUps[BOOTS] ? posCharacter.x -= MOVEMENT_SPEED : posCharacter.x -= MOVEMENT_DEFAULT;
			sprite->changeAnimation(STAND_RIGHT);
			if (bJumping) {
				sprite->changeAnimation(JUMP_LEFT);
			}
		}
		if (bJumping) {
			sprite->changeAnimation(JUMP_RIGHT);
		}
	}
	else if (scene->collisionLianaUp(posCharacter, glm::ivec2(32, 32)) && Game::instance().getSpecialKey(GLUT_KEY_UP)
		|| scene->collisionLianaDown(posCharacter, glm::ivec2(32, 32)) && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		lianaClimb = true;
		bJumping = false;
		sprite->changeAnimation(CLIMB_ACTIVE);
	}
	else if (!bJumping && scene->collisionTP(posCharacter, glm::ivec2(32, 32), &posCharacter) && Game::instance().getSpecialKey(GLUT_KEY_UP)) teleporting = true;
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
		else if (sprite->animation() == CLIMB_ACTIVE)
			sprite->changeAnimation(CLIMB_STATIC);

	}

	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posCharacter.y = startY;
		}
		else
		{
			posCharacter.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle < 90 && scene->collisionMoveUp(posCharacter, glm::ivec2(32, 32), &posCharacter.y)) {
				jumpAngle = 180 - jumpAngle;
			}
			if (jumpAngle > 90)
				bJumping = !scene->collisionMoveDown(posCharacter, glm::ivec2(32, 32), &posCharacter.y);

			//cambiar animacion aqui salto
		}
	}
	else if (lianaClimb) {
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			posCharacter.y -= 2;
			if (sprite->animation() != CLIMB_ACTIVE)
				sprite->changeAnimation(CLIMB_ACTIVE);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			posCharacter.y += 2;
			if (sprite->animation() != CLIMB_ACTIVE)
				sprite->changeAnimation(CLIMB_ACTIVE);
		}
		if (!scene->collisionLiana(posCharacter, glm::ivec2(32, 32))) {
			lianaClimb = false;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if (teleporting) {
		if (count == 0) sprite->changeAnimation(STAND_RIGHT);
		else if (count == 5) sprite->changeAnimation(STAND_LEFT);
		else if (count == 10) sprite->changeAnimation(CLIMB_STATIC);
		else if (count == 18) sprite->changeAnimation(STAND_RIGHT);
		else if (count == 22) sprite->changeAnimation(STAND_LEFT);
		else if (count == 25) sprite->changeAnimation(CLIMB_STATIC);
		else if (count == 31) sprite->changeAnimation(STAND_RIGHT);
		else if (count == 34) sprite->changeAnimation(STAND_LEFT);
		else if (count == 36) sprite->changeAnimation(CLIMB_STATIC);
		else if (count == 40) sprite->changeAnimation(STAND_RIGHT);

	}
	else
	{
		posCharacter.y += FALL_STEP;
		if (scene->collisionMoveDown(posCharacter, glm::ivec2(32, 32), &posCharacter.y))
		{
			//cambiar animación aqui salto
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posCharacter.y;
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Player::damage(int attackPower) {
	if (inmunytyFrames == 0 && !godMode) {
		inmunytyFrames = 10;
		//sonido de daño
		powerUps[RAINCOAT] ? actHealth -= attackPower/2 : actHealth -= attackPower;
	}
}

Player::~Player()
{
}

void Player::changeGameMode() {
	godMode = !godMode;
}

void Player::givePowerUp(int i) {
	powerUps[i] = !powerUps[i];
}

void Player::initTeleports() {

	teleports.resize(20);

	teleports[0] = { 3, glm::ivec2(5 * 16 - 8, 3 * 16 - 8) };
	teleports[1] = { 3, glm::ivec2(25 * 16 - 8, 8 * 16 - 8) };
	teleports[2] = { 1, glm::ivec2(3 * 16 - 8, 18 * 16 - 8) };
	teleports[3] = { 2, glm::ivec2(24 * 16 - 8, 9 * 16 - 8) };

	teleports[4] = { 11, glm::ivec2(2 * 16 - 8, 7 * 16 - 8) };
	teleports[5] = { 8, glm::ivec2(3 * 16 - 8, 8 * 16 - 8) };
	teleports[6] = { 7, glm::ivec2(20 * 16 - 8, 13 * 16 - 8) };
	teleports[7] = { 7, glm::ivec2(7 * 16 - 8, 3 * 16 - 8) };

	teleports[8] = { 15, glm::ivec2(29 * 16 - 8, 3 * 16 - 8) };
	teleports[9] = { 15, glm::ivec2(2 * 16 - 8, 15 * 16 - 8) };
	teleports[10] = { 17, glm::ivec2(25 * 16 - 8, 9 * 16 - 8) };
	teleports[11] = { 17, glm::ivec2(13 * 16 - 8, 3 * 16 - 8) };

	teleports[12] = { 16, glm::ivec2(6 * 16 - 8, 17 * 16 - 8) };
	teleports[13] = { 10, glm::ivec2(28 * 16 - 8, 5 * 16 - 8) };
	teleports[14] = { 10, glm::ivec2(28 * 16 - 8, 5 * 16 - 8) };

	teleports[15] = { 23, glm::ivec2(27 * 16 - 8, 16 * 16 - 8) };
	teleports[16] = { 24, glm::ivec2(23 * 16 - 8, 13 * 16 - 8) };
	teleports[17] = { 22, glm::ivec2(28 * 16 - 8, 4 * 16 - 8) };
	teleports[18] = { 23, glm::ivec2(2 * 16 - 8, 8 * 16 - 8) };

	teleporting = false;
	count = 0;
}

int Player::getTeleportNum(int scene) {
	switch (scene) {
	case 0:
		break;
	case 1:
		return 0;
		break;
	case 2:
		return 1;
		break;
	case 3:
		if (posCharacter.x < 16 * 16) return 2;
		else return 3;
		break;


	case 7:
		if (posCharacter.x < 12 * 16) return 4;
		else return 5;
		break;
	case 8:
		return 6;
		break;
	case 11:
		return 7;
		break;


	case 17:
		if (posCharacter.x < 17 * 16) return 8;
		else return 9;
		break;
	case 18:
		break;
	case 15:
		if (posCharacter.x < 16 * 16) return 10;
		else return 11;
		break;


	case 10:
		return 12;
		break;
	case 13:
		return 13;
		break;
	case 16:
		return 14;
		break;


	case 22:
		return 15;
		break;
	case 23:
		if (posCharacter.x < 16 * 16) return 16;
		else return 17;
		break;
	case 24:
		return 18;
		break;


	default:
		break;
	}
	return 1;
}

int Player::teleport(int& scene) {
	if (teleporting && count <= 40) count++;
	if (teleporting && count > 40) {
		count = 0;
		int TeleportNum = getTeleportNum(scene);
		posCharacter = teleports[TeleportNum].second;
		scene = teleports[TeleportNum].first;
		teleporting = false;
		return 1;
	}
}

int Player::isOut() {
	if (tileMapDispl.x + posCharacter.x + 16 < tileMapDispl.x) {
		posCharacter.x += 508;
		return -1;
	}
	else if (tileMapDispl.x + posCharacter.x + 16 > tileMapDispl.x + scene->getMapSizeX()) {
		posCharacter.x -= 508;
		return 1;
	}
	else if (tileMapDispl.y + posCharacter.y + 16 < tileMapDispl.y) {
		posCharacter.y += 316;
		return -3;
	}
	else if (tileMapDispl.y + posCharacter.y + 16 > tileMapDispl.y + scene->getMapSizeY()) {
		posCharacter.y -= 316;
		return 3;
	}
	else return 0;
}


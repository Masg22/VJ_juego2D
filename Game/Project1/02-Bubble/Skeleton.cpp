#include "Skeleton.h"
#include "Scene.h"

#define FALL_STEP 4

enum SkeletonAnims
{
	MOVE_LEFT, MOVE_RIGHT
};

Skeleton::Skeleton()
{
	dir = glm::ivec2(-1, 0);
	setPathToSpriteSheet("images/SkeletonAnimations.png");
	mNumberAnimations = 2;
	mSpriteRows = 2;
	mSpriteColumns = 2;
}

void Skeleton::init(glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Scene* scene) {

	BaseEnemy::init(tileMapPos, shaderProgram, scene);
	sprite->setNumberAnimations(mNumberAnimations);
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(MOVE_LEFT);
	sprite->setPosition(glm::vec2(float(posCharacter.x), float(posCharacter.y)));  
}

void Skeleton::update(int deltaTime) {
	posCharacter += dir;
	if (scene->collisionCanFall(this)) {
		if (dir.x < 0) {
			sprite->changeAnimation(MOVE_RIGHT);
		}
		else {
			sprite->changeAnimation(MOVE_LEFT);
		}
		posCharacter -= dir;
	}
	else {
		if (dir.x < 0) {
			if (scene->collisionMoveLeft(posCharacter, glm::ivec2(32, 32))) {
				sprite->changeAnimation(MOVE_RIGHT);
				posCharacter -= dir;
				dir = -dir;
			}
			else {
				if (scene->collisionMoveRight(posCharacter, glm::ivec2(32, 32))) {
					sprite->changeAnimation(MOVE_LEFT);
					posCharacter -= dir;
					dir = -dir;
				}
			}
		}
		posCharacter.y += FALL_STEP;
		scene->collisionMoveDown(posCharacter, glm::ivec2(32, 32), &posCharacter.y);

		BaseEnemy::update(deltaTime);
	}
}
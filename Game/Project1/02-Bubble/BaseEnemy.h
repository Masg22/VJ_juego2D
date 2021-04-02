#pragma once
#include "Character.h"


// Enemy is basically a Sprite that represents the enemies. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class BaseEnemy : public Character
{

public:
	BaseEnemy();
	~BaseEnemy();

	virtual void init(ShaderProgram& shaderProgram, Scene* scene);
	virtual void update(int deltaTime);

	virtual int getScore() { return 200; }

protected:
	glm::ivec2 dir;
	bool isAttacking;
};

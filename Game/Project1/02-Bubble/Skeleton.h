#pragma once
#include "BaseEnemy.h"
class Skeleton : public BaseEnemy {
public:
	Skeleton();
	~Skeleton() {}

	void init(glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Scene* scene);
	void update(int deltaTime);

protected:
	//void searchPlayer();
};
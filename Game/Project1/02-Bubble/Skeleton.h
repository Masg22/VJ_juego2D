#pragma once
#include "BaseEnemy.h"
class Skeleton : public BaseEnemy {
public:
	Skeleton();
	~Skeleton() {}

	void init(ShaderProgram& shaderProgram, Scene* scene);
	void update(int deltaTime);

protected:
	//void searchPlayer();
};
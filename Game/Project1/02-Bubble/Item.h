#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Sprite.h"

class Scene;

class Item {
public:
	Item();
	~Item();

	void init(ShaderProgram& shaderProgram, Scene* scene);
	void update(int deltaTime);
	void render();

	int getScore() { return score; }

	void setPosition(const glm::ivec2& pos);
	glm::ivec2 getPosition() { return posObj; }

	glm::ivec2 getSize() { return glm::ivec2(16, 16); }

private:
	string pathToSpritesheet;
	Texture spritesheet;
	Sprite* sprite;
	int item_id;
	Scene* scene;
	glm::ivec2 posObj;
	int score;
};
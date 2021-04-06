#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Sprite.h"

class Scene;

class Item {
public:
	Item(int id);
	~Item();

	void init(glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Scene* scene);
	void update(int deltaTime);
	void render();

	int getScore() { return score; }

	void setPosition(const glm::ivec2& pos);
	glm::ivec2 getPosition() const { return posItem; }

	glm::ivec2 getSize() const { return glm::ivec2(16, 16); }

	int getID() const { return itemID; }

private:
	string pathToSpritesheet;
	Texture spritesheet;
	Sprite* sprite;
	int itemID;
	Scene* scene;
	glm::ivec2 posItem, tileMapDispl;
	int score;
};
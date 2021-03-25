#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
//#include "Player.h"


// Enemy is basically a Sprite that represents the enemies. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	glm::ivec2 tileMapDispl, posEnemy;
	int startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	glm::ivec2 initialPosEnemy;
	//int distance = 20;
	//double speed = 2*3.14159*100000;
};


#endif // _ENEMY_INCLUDE
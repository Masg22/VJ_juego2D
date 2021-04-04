#pragma once
#include "Character.h"
#include "Scene.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

class Character;
class Scene;

class Player : public Character
{
public:
	Player();
	~Player();

	virtual void init(ShaderProgram& shaderProgram, Scene* scene);
	virtual void update(int deltaTime);

	void computeNextMove(int deltaTime);
	void computeJumping();
	void computeAnimation(int deltaTime);

	void damage();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void computeAttack(int deltaTime);
	void computeMovement();
	void computeJump();

	
private:
	int attackSoundTime, attackTime;
	bool bAnimating;
	int animationTime;
};




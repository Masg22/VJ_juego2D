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

	void damage(int power);

	void computeAttack(int deltaTime);
	void computeMovement(int deltaTime);
	void computeJump();

	void changeGameMode();
	void givePowerUp(int i);

	
private:
	int actHealth, maxHealth;
	int attackSoundTime;
	int attackTime;
	bool bAnimating;
	//int animationTime;

	bool godMode;
	bool powerUps[4];
};




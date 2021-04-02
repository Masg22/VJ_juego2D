#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "Scene.h"

class Character {
public:
	Character();
	~Character(){}

	void init(ShaderProgram& shaderProgram, Scene* scene);
	void update(int deltaTime);
	virtual void render();

	void setPathToSpriteSheet(string path);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition() const { return posCharacter; }
	glm::ivec2 getCollisionSize() const { return getSize(); }

	virtual void damage();
	virtual void getType() {}
	void setStartAnimation(int animId) { mStartAnimation = animId; }
	int getStartAnimation() { return mStartAnimation; }

	void setStartY(int newstartY) { startY = newstartY; }
	int getStartY() { return startY; }

protected:
	int mStartAnimation;
	bool bJumping, bAttacking;
	glm::ivec2 posCharacter;
	int jumpAngle, startY;
	string pathToSpritesheet;
	Texture spritesheet;
	Sprite* sprite;
	Scene* scene;
	int mNumberAnimations, mSpriteColumns, mSpriteRows;
	bool isDead;
	int inmunytyFrames; //despues de ser dañado no te pueden volver a golpear
	//glm::ivec2 directionDamaged; //Direction to send character when being damaged





}

#endif // _PLAYER_INCLUDE
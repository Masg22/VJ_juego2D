#ifndef _CHARACTER_INCLUDE
#define _CHARACTER_INCLUDE

#include "Sprite.h"

#define CHARACTER_SIZE_X 32
#define CHARACTER_SIZE_Y 32

class Scene;

class Character {
public:
	Character();
	~Character() {};

	void init(glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Scene* scene);
	void update(int deltaTime);
	virtual void render();

	void setPathToSpriteSheet(string path);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition() const { return posCharacter; }
	glm::ivec2 getSize() const { return glm::ivec2(CHARACTER_SIZE_X, CHARACTER_SIZE_Y); }
	void setPosition(glm::ivec2 pos) { posCharacter = pos; }
	glm::ivec2 getCollisionSize() const { return getSize(); }
	virtual glm::ivec2 getCollisionPosition() const { return getPosition() + glm::ivec2(6, 10); }

	virtual void damage();
	virtual void getType() {}
	void setStartAnimation(int animId) { mStartAnimation = animId; }
	int getStartAnimation() { return mStartAnimation; }

	void setStartY(int newstartY) { startY = newstartY; }
	int getStartY() { return startY; }
	bool isCharacterDead() { return isDead; }

protected:
	int mStartAnimation;
	bool bJumping, bAttacking;

	glm::ivec2 tileMapDispl, posCharacter;

	int jumpAngle, startY;

	bool isDead;
	int inmunytyFrames; //despues de ser dañado no te pueden volver a golpear

	string pathToSpritesheet;
	Texture spritesheet;
	Sprite* sprite;
	Scene* scene;

	int mNumberAnimations, mSpriteColumns, mSpriteRows;
	
	//glm::ivec2 directionDamaged; //Direction to send character when being damaged
};

#endif // _PLAYER_INCLUDE
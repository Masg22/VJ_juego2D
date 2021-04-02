#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "BaseEnemy.h"
#include "Colisions.h"
#include <set>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene {
public:
	Scene();
	~Scene();

	void resetScene();
	void setSceneToReset() { bToReset = true; };

	void init(std::string levelPathFile, std::string backgroundPathFile, std::string enemiesLocationPathFile, std::string itemsLocationPathFile);
	void update(int deltaTime);
	void render();

	bool collisionMoveRight(Character* character) const;
	bool collisionMoveLeft(Character* character) const;
	bool collisionMoveDown(Character* character) const;
	bool collisionMoveUp(Character* character) const;

	bool characterCollidesTiles(Character* character) const;
	bool collisionCanFall(BaseEnemy* enemy) const;
	bool playerHits(BaseEnemy* enemy) const;

	Player* getPlayer() { return  player; }

private:
	void initShaders();
	void initEnemies(std::string enemiesLocationPathFile);
	bool characterCollidesEnemies(Character* character) const;

	bool bToReset;
	TileMap *map;

	Player *player;
	
	ShaderProgram texProgram;
	float currentTime;

	glm::mat4 projection;

	set<BaseEnemy*> enemies;

	Colisions* colisions;
};


#endif // _SCENE_INCLUDE


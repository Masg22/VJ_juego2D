#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"

#include "Player.h"
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

	void init(Player* playerGen, int lvl);
	void update(int deltaTime);
	void render();


	//TO MAP
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	
	int getMapSizeX() const;
	int getMapSizeY() const;

	bool collisionLiana(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionLianaUp(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionLianaDown(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionTP(const glm::ivec2& pos, const glm::ivec2& size, glm::ivec2* posPlayer) const;

	//bool characterCollidesTiles(Character* character) const;
	bool collisionCanFall(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool playerHits(BaseEnemy* enemy) const;

	Player* getPlayer() { return  player; }
	TileMap* getMap() { return map; }

	void changePlayerGameMode();
	void givePlayerPowerUp(int i);

	void playerCollidesEnemies(BaseEnemy* enemy) const;
	void playerCollidesItem(Item* it) const;

private:
	void initShaders();
	void initEnemies(std::string enemiesLocationPathFile);
	void initItems(std::string itemsLocationPathFile);
	

private:

	int level;
	vector<int> mapLevels;

	bool bToReset;

	TileMap *map;
	Player *player;
	set<BaseEnemy*> enemies;
	set<Item*> items;
	Colisions* colisions;
	
	ShaderProgram texProgram;

	float currentTime;

	glm::mat4 projection;	
};

#endif // _SCENE_INCLUDE


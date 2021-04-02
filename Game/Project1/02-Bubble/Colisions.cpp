#include "Colisions.h"
#include "TileMap.h"
#include "Player.h"
#include "BaseEnemy.h"

#include <iostream>

using namespace std;

Colisions::Colisions()
{
}

bool Colisions::mapRight(const TileMap* map, Character* character) const {
	glm::ivec2 pos = character->getPosition();
	glm::ivec2 size = character->getSize();

	return map->collisionMoveRight(pos, size);
}

bool Colisions::mapLeft(const TileMap* map, Character* character) const {
	glm::ivec2 pos = character->getPosition();
	glm::ivec2 size = character->getSize();

	return map->collisionMoveLeft(pos, size);
}

bool Colisions::mapDown(const TileMap* map, Character* character) const {
	glm::ivec2 pos = character->getPosition();
	glm::ivec2 size = character->getSize();
	int* y = character -> getY(); 

	return map->collisionMoveDown(pos, size, y);
}

bool Colisions::mapUp(const TileMap* map, Character* character) const {
	glm::ivec2 pos = character->getPosition();
	glm::ivec2 size = character->getSize();

	return map->collisionMoveRight(pos, size);
}

bool Colisions::mapFalls(const TileMap* map, BaseEnemy* enemy) const {
	glm::ivec2 pos = enemy->getPosition();
	glm::ivec2 size = enemy->getSize();

	int x0, x1, y;
	int tileSize = map->getTileSize();

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y + 1) / tileSize;
	int mapTilesWidth = map->getMapTilesWidth();
	int* m = map->getMap();
	for (int x = x0; x <= x1; x++) {
		if (map[y * mapTilesWidth + x] == 0) {
			return true;
		}
	}
	return false;
}

bool Colisions::characters(Character* myself, const Character* other) const {
	return quadsCollision(myself->getCollisionPosition(), myself->getCollisionSize(),
		other->getCollisionPosition(), other->getCollisionSize());
}

bool Colisions::object(Character* myself, const Object* obj) const {
	return quadsCollision(myself->getCollisionPosition(), myself->getCollisionSize(), obj->getPosition(), obj->getSize());
}


bool Colisions::quadsCollision(glm::vec2 q1Pos, glm::vec2 q1Size, glm::vec2 q2Pos, glm::vec2 q2Size) const {
	float q1x1 = q1Pos.x; float q1x2 = q1x1 + q1Size.x;
	float q1y1 = q1Pos.y; float q1y2 = q1y1 + q1Size.y;

	float q2x1 = q2Pos.x; float q2x2 = q2x1 + q2Size.x;
	float q2y1 = q2Pos.y; float q2y2 = q2y1 + q2Size.y;

	if (q1Pos == q2Pos) return false; //Collision with itself

	return ((q1x1 < q2x2&& q1x2 >q2x1 && q1y1 <= q2y1 && q1y2 >= q2y1) ||
		(q2x1 < q1x2&& q2x2 > q1x1 && q2y1 <= q1y1 && q2y2 >= q1y1));
}

int Colisions::distanceBetweenPositions(const glm::ivec2 pos1, const glm::ivec2 pos2) const {
	return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

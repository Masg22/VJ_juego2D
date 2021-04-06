#include "Colisions.h"
#include "TileMap.h"
#include "Player.h"
#include "BaseEnemy.h"
#include "Item.h"

#include <iostream>

using namespace std;

Colisions::Colisions()
{
}

bool Colisions::characters(Character* myself, const Character* other) const {
	return quadsCollision(myself->getCollisionPosition(), myself->getCollisionSize(),
		other->getCollisionPosition(), other->getCollisionSize());
}

bool Colisions::object(Character* myself, const Item* it) const {
	return quadsCollision(myself->getCollisionPosition(), myself->getCollisionSize(), it->getPosition(), it->getSize());
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

#pragma once
#include <glm/glm.hpp>

class TileMap;
class Character;
class Player;
class BaseEnemy;
class Item;
//class PortalObject;

class Colisions {
public: 
	Colisions(); 
	~Colisions() {}

	//colisiones entre pjs
	bool characters(Character* myself, const Character* other) const;
	
	//colisiones entre pjs y objetos
	bool object(Character* myself, const Item* obj)const;

private:

	//Check if two quads intersect, pos is the left-top vertex
	bool quadsCollision(glm::vec2 q1Pos, glm::vec2 q1Size, glm::vec2 q2Pos, glm::vec2 q2Size) const;

	//Returns the euclidean distance between two positions
	int distanceBetweenPositions(const glm::ivec2 pos1, const glm::ivec2 pos2) const;
};

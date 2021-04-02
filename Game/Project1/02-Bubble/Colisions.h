#pragma once
#include <glm/glm.hpp>

class TileMap;
class Character;
class Player;
class BaseEnemy;
class Object;
//class PortalObject;

class Colisions {
public: 
	Colisions(); 
	~Colisions() {}

	//colisiones con el mapa
	bool mapRight(const TileMap* map, Character* character) const;
	bool mapLeft(const TileMap* map, Character* character) const;
	bool mapDown(const TileMap* map, Character* character) const;
	bool mapUp(const TileMap* map, Character* character) const;

	//si un enemigo va a caer por una plataforma
	bool mapFalls(const TileMap* map, BaseEnemy* character) const;

	//colisiones entre pjs
	bool characters(Character* myself, const Character* other) const;
	
	//colisiones entre pjs y objetos
	bool object(Character* myself, const Object* obj)const;

	//void characterDoesTeleport(Character* character, PortalObject* portalAct,PortalObject* portalDest) const;
	//bool mapCollidesObject(const TileMap* tMap, BaseObject* object)const;

private:

	//Check if two quads intersect, pos is the left-top vertex
	bool quadsCollision(glm::vec2 q1Pos, glm::vec2 q1Size, glm::vec2 q2Pos, glm::vec2 q2Size) const;

	//Returns the euclidean distance between two positions
	int distanceBetweenPositions(const glm::ivec2 pos1, const glm::ivec2 pos2) const;

	//Returns if two caharacters are (near to be) at same horizontal line, used for kirby swalling
	bool sameHorizontal(const glm::ivec2 pos1, const glm::ivec2 pos2) const;
};

#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int* posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;

	bool collisionLiana(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionLianaUp(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionLianaDown(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionTP(const glm::ivec2& pos, const glm::ivec2& size, glm::ivec2* posPlayer) const;

	int getTileSize() const { return tileSize; }

	int getMapWidth() const { return tileSize * mapSize.x; }
	int getMapHeight() const { return tileSize * mapSize.y; }

	int getMapTilesWidth() const { return mapSize.x; }
	int getMapTilesHeight() const { return mapSize.y; }

	int* getMap() const { return map; };
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	vector<int> downCollision, rightCollision, leftCollision, upCollision, lianaUp, lianaDown, tp;

};


#endif // _TILE_MAP_INCLUDE



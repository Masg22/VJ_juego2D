#ifndef _STATUS_INCLUDE
#define _STATUS_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class Status
{

public:
	// Tile maps can only be created inside an OpenGL context
	static Status* createStatus(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	Status(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	~Status();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }
	int getMapSizeX() const { return mapSize.x * tileSize; }
	int getMapSizeY() const { return mapSize.y * tileSize; }


private:
	bool loadStatus();
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* status;

};


#endif // _TILE_MAP_INCLUDE



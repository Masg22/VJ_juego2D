#include <iostream>
#include <fstream>
#include <sstream>
#include "Status.h"


using namespace std;


Status* Status::createStatus(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	Status* status = new Status(levelFile, minCoords, program);

	return status;
}


Status::Status(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	//loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

Status::~Status()
{
	if (status != NULL)
		delete status;
}


void Status::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void Status::free()
{
	glDeleteBuffers(1, &vbo);
}

bool Status::loadStatus()
{
	mapSize.x = 32;
	mapSize.y = 24;

	tileSize = 16;
	blockSize = 16;

	tilesheet.loadFromFile("images/statusGoonies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	tilesheetSize.x = 32;
	tilesheetSize.y = 8;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	status = new int[mapSize.x * mapSize.y];

	status[0 * mapSize.x + 1] = 41;
	status[0 * mapSize.x + 2] = 49;

	status[0 * mapSize.x + 3] = 14;
	status[0 * mapSize.x + 4] = 22;
	status[0 * mapSize.x + 5] = 22;
	status[0 * mapSize.x + 6] = 22;
	status[0 * mapSize.x + 7] = 22;
	status[0 * mapSize.x + 8] = 22;
	status[0 * mapSize.x + 9] = 22;
	status[0 * mapSize.x + 10] = 22;
	status[0 * mapSize.x + 11] = 22;
	status[0 * mapSize.x + 12] = 22;
	status[0 * mapSize.x + 13] = 22;
	status[0 * mapSize.x + 14] = 32;

	status[1 * mapSize.x + 0] = 38;
	status[1 * mapSize.x + 1] = 57;
	status[1 * mapSize.x + 2] = 49;

	status[1 * mapSize.x + 3] = 14;
	status[1 * mapSize.x + 4] = 31;
	status[1 * mapSize.x + 5] = 31;
	status[1 * mapSize.x + 6] = 31;
	status[1 * mapSize.x + 7] = 31;
	status[1 * mapSize.x + 8] = 31;
	status[1 * mapSize.x + 9] = 31;
	status[1 * mapSize.x + 10] = 31;
	status[1 * mapSize.x + 11] = 31;
	status[1 * mapSize.x + 12] = 31;
	status[1 * mapSize.x + 13] = 31;
	status[1 * mapSize.x + 14] = 32;

	status[0 * mapSize.x + 27] = 52;
	status[0 * mapSize.x + 28] = 36;
	status[0 * mapSize.x + 29] = 38;
	status[0 * mapSize.x + 30] = 47;
	status[0 * mapSize.x + 31] = 38;

	status[1 * mapSize.x + 27] = 1;
	status[1 * mapSize.x + 28] = 2;
	status[1 * mapSize.x + 29] = 33;
	status[1 * mapSize.x + 30] = 1;
	status[1 * mapSize.x + 31] = 2;

	for (int i = 0; i < 32; ++i) {
		status[22 * mapSize.x + i] = 64;
	}

	status[23 * mapSize.x + 27] = 12;
	status[23 * mapSize.x + 28] = 33;
	status[23 * mapSize.x + 29] = 11;
	status[23 * mapSize.x + 30] = 11;
	status[23 * mapSize.x + 31] = 11;

	return true;
}

void Status::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = status[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % 16) / tilesheetSize.x, float((tile - 1) / 16) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				//texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}



// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

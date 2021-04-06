#include <iostream>
#include <fstream>
#include <sstream>
#include "TileMap.h"


using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{

		for (int i = 0; i < mapSize.x; i++)
		{
			fin >> map[j * mapSize.x + i];
		}
#ifndef _WIN32
		fin.get(tile);
#endif
	}

	for (int i = 0; i < 7; ++i) {
		fin >> tile;

		while (tile >= 0)
		{
			if (i == 0) leftCollision.push_back(tile);
			else if (i == 1) rightCollision.push_back(tile);
			else if (i == 2) downCollision.push_back(tile);
			else if (i == 3) upCollision.push_back(tile);
			else if (i == 4) lianaUp.push_back(tile);
			else if (i == 5) lianaDown.push_back(tile);
			else if (i == 6) tp.push_back(tile);
			fin >> tile;
		}
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
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

bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + 8) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (find(leftCollision.begin(), leftCollision.end(), map[y * mapSize.x + x]) != leftCollision.end())
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 9) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (find(rightCollision.begin(), rightCollision.end(), map[y * mapSize.x + x]) != rightCollision.end())
			return true;
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (find(downCollision.begin(), downCollision.end(), map[y * mapSize.x + x]) != downCollision.end())
		{
			if (*posY - tileSize * y + size.y <= 12)
			{
				*posY = tileSize * y - size.y + 8;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (find(upCollision.begin(), upCollision.end(), map[y * mapSize.x + x]) != upCollision.end())
		{
			return true;
		}
	}

	return false;
}

bool TileMap::collisionLiana(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y;

	x = (pos.x + (size.x - 1) / 2) / tileSize;
	//y0 = pos.y / tileSize;
	y = (pos.y + size.y * 3 / 2 + 1) / tileSize;
	//for (int y = y0; y <= y1; y++)
	{
		if (find(lianaUp.begin(), lianaUp.end(), map[y * mapSize.x + x]) != lianaUp.end())
		{
			return true;
		}
		else if (find(lianaDown.begin(), lianaDown.end(), map[y * mapSize.x + x]) != lianaDown.end())
		{
			return true;
		}
	}

	return false;
}

bool TileMap::collisionLianaUp(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y;

	x = (pos.x + (size.x - 1) / 2) / tileSize;
	//y0 = pos.y / tileSize;
	y = (pos.y) / tileSize;
	//for (int y = y0; y <= y1; y++)
	{
		if (find(lianaUp.begin(), lianaUp.end(), map[y * mapSize.x + x]) != lianaUp.end())
		{
			return true;
		}
	}

	return false;
}

bool TileMap::collisionLianaDown(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y;

	x = (pos.x + (size.x - 1) / 2) / tileSize;
	//y0 = pos.y / tileSize;
	y = (pos.y + size.y * 3 / 2) / tileSize;
	//for (int y = y0; y <= y1; y++)
	{
		if (find(lianaDown.begin(), lianaDown.end(), map[y * mapSize.x + x]) != lianaDown.end())
		{
			return true;
		}
	}

	return false;
}

bool TileMap::collisionTP(const glm::ivec2& pos, const glm::ivec2& size, glm::ivec2* posPlayer) const
{
	int x, y0, y;

	x = (pos.x + (size.x - 1) / 2) / tileSize;
	//y0 = pos.y / tileSize;
	y = (pos.y + size.y / 2) / tileSize;
	//for (int y = y0; y <= y1; y++)
	{
		if (find(tp.begin(), tp.end(), map[y * mapSize.x + x]) != tp.end())
		{
			posPlayer->x = x * tileSize - 8;
			posPlayer->y = y * tileSize - 8;
			return true;
		}
	}

	return false;
}

























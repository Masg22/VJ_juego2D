#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "Skeleton.h"


#define SCREEN_X 0
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 17
#define INIT_PLAYER_Y_TILES 15

Scene::Scene()
{
	map = NULL;
	player = NULL;
	colisions = NULL;
}

Scene::~Scene()
{
	resetScene();
}

void Scene::resetScene() {
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (colisions != NULL)
		delete colisions;

	enemies.clear();
}

void Scene::init(Player* playerGen, int lvl){

	initShaders();


	map = TileMap::createTileMap("levels/level" + to_string(lvl) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	colisions = new Colisions();

	player = playerGen;
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, this);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	
	initEnemies("levels/level" + to_string(lvl) + "_enemies.txt");


	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (BaseEnemy* enemy : enemies) {
		enemy->update(deltaTime);
	}
	//limpiar enemigos muertos
	for (set<BaseEnemy*>::iterator it = enemies.begin(); it != enemies.end();) {
		if ((*it)->isCharacterDead()) {
			//Game::instance().addScore((*it)->getScore());
			enemies.erase(it++);
		}
		else ++it;
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	for (BaseEnemy* enemy : enemies) {
		enemy->render();
	}
}

bool Scene::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const {
	return map->collisionMoveRight(pos, size);
}

bool Scene::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const {
	return map->collisionMoveLeft(pos, size);
}

bool Scene::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const {
	return map->collisionMoveDown(pos, size, posY);
}

bool Scene::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const {
	return map->collisionMoveUp(pos, size, posY);
}

bool Scene::collisionCanFall(const glm::ivec2& pos, const glm::ivec2& size) const {
	return map->mapFalls(pos, size);
}

int Scene::getMapSizeX() const {
	return map->getMapWidth();
}

int Scene::getMapSizeY() const {
	return map->getMapHeight();
}

bool Scene::collisionLiana(const glm::ivec2& pos, const glm::ivec2& size) const{
	return map->collisionLiana(pos, size);
}

bool Scene::collisionLianaUp(const glm::ivec2& pos, const glm::ivec2& size) const {
	return map->collisionLianaUp(pos, size);
}

bool Scene::collisionLianaDown(const glm::ivec2& pos, const glm::ivec2& size) const {
	return map->collisionLianaDown(pos, size);
}

bool Scene::collisionTP(const glm::ivec2& pos, const glm::ivec2& size, glm::ivec2* posPlayer) const {
	return map->collisionTP(pos, size, posPlayer);
}

bool Scene::characterCollidesEnemies(Character* character) const {
	//REHACER

	bool collision = false; //Can be improved checking if true after each enemy and returning(with few enemies doesn't matter)
	bool collidedEnemy = false;

	int posEnemyX = character->getPosition().x;

	
	for (BaseEnemy* enemy : enemies) {

		collidedEnemy = colisions->characters(character, enemy);
		collision = collision || collidedEnemy;
		if (collidedEnemy) {
			posEnemyX = enemy->getPosition().x;
			break;
		}
	}

	Player* p = dynamic_cast<Player*>(character);
	if (p && collision) {
		//enemies can not get damaged between them!
		p->damage(10);
	}
	return collision;
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::initEnemies(std::string enemiesLocationPathFile) {
	ifstream fin;
	string line;
	stringstream sstream;

	fin.open(enemiesLocationPathFile);
	if (!fin.is_open()) {
		cout << "file already open!" << endl;
		return;
	}

	getline(fin, line);
	sstream.str(line);
	
	int numEnemies;
	sstream >> numEnemies;
	int enemyType, posX, posY;

	for (int i = 0; i < numEnemies; ++i) {
		getline(fin, line);
		stringstream(line) >> enemyType >> posX >> posY;
		switch (enemyType) {
		case 0: {
			Skeleton* skeleton = new Skeleton;
			skeleton->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, this);
			skeleton->setPosition(glm::vec2(posX * map->getTileSize(), posY * map->getTileSize()));
			enemies.insert(skeleton);
			break;
		}
		}
	}

	fin.close();

}

bool Scene::playerHits(BaseEnemy* enemy) const {
	return false;
}

void Scene::changePlayerGameMode() {
	player->changeGameMode();
}

void Scene::givePlayerPowerUp(int i) {
	player->givePowerUp(i);
}

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
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 17
#define INIT_PLAYER_Y_TILES 18

#define FALL_STEP 4

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

void Scene::init(std::string levelPathFile, std::string enemiesLocationPathFile, std::string itemsLocationPathFile){
	resetScene();
	bToReset = false;

	initShaders();
	colisions = new Colisions();

	//map = TileMap::createTileMap("levels/level00.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap(levelPathFile, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(texProgram, this);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	
	initEnemies(enemiesLocationPathFile);

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

bool Scene::collisionMoveRight(Character* character) const {
	bool mapCollision = colisions->mapRight(map, character);
	bool enemyCollision = characterCollidesEnemies(character);
	//bool tilesCollision = characterCollidesTiles(character);
	return mapCollision || enemyCollision;// || tilesCollision;
}

bool Scene::collisionMoveLeft(Character* character) const {
	bool mapCollision = colisions->mapLeft(map, character);
	bool enemyCollision = characterCollidesEnemies(character);
	//bool tilesCollision = characterCollidesTiles(character);
	return mapCollision || enemyCollision;// || tilesCollision;
}

bool Scene::collisionMoveDown(Character* character) const {
	bool mapCollision = colisions->mapDown(map, character);
	bool enemyCollision = characterCollidesEnemies(character);
	//bool tilesCollision = characterCollidesTiles(character);
	if (!mapCollision /*&& tilesCollision*/) {
		glm::ivec2 posChar = character->getPosition();
		character->setPosition(glm::ivec2(posChar.x, posChar.y - FALL_STEP));
	}
	return mapCollision || enemyCollision;// || tilesCollision;
}

bool Scene::collisionMoveUp(Character* character) const {
	bool mapCollision = colisions->mapUp(map, character);
	bool enemyCollision = characterCollidesEnemies(character);
	//bool tilesCollision = characterCollidesTiles(character);
	return mapCollision || enemyCollision;// || tilesCollision;
}

bool Scene::collisionCanFall(BaseEnemy* enemy) const {
	return colisions->mapFalls(map, enemy);
}

bool Scene::characterCollidesEnemies(Character* character) const {
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
			skeleton->init(texProgram, this);
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

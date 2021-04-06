#include "Item.h"

Item::Item(int id) {
	itemID = id;
	score = id * 10;
	activated = true;
}
Item::~Item() {}

void Item::init(glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Scene* scene) {
	this->scene = scene;
	tileMapDispl = tileMapPos;

	if (itemID == 8) {
		spritesheet.loadFromFile("images/friend.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	}
	else {
		spritesheet.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0, 0));
	}
	sprite->changeAnimation(0);
	//sprite->setPosition(glm::vec2(float(posItem.x), float(posItem.y)));
}

void Item::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Item::render() {
	sprite->render();
}

void Item::setPosition(const glm::ivec2& pos) {
	posItem = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

void Item::desactivate() {
	activated = false;
}
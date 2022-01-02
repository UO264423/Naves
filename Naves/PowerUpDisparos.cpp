#include "PowerUpDisparos.h"
PowerUpDisparos::PowerUpDisparos(float x, float y, Game* game) : Actor("res/icono_vidas.png", x, y, 32, 32, game) {
	aMoving = new Animation("res/recolectable.png", 32, 32,
		256, 32, 6, 8, game);
	animation = aMoving;
}

void PowerUpDisparos::draw() {
	animation->draw(x, y);
}

void PowerUpDisparos::update() {
	animation->update();
}
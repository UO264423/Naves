#include "EnemyBase.h"
#include "Animation.h"
EnemyBase::EnemyBase(float x, float y, Game* game, string drawName, string animationName)
	: Actor(drawName, x, y, 36, 40, game) {
	aMoving = new Animation(animationName, width, height,
		108, 40, 6, 3, game);
	animation = aMoving;
}

void EnemyBase::draw() {
	animation->draw(x, y);
}

void EnemyBase::update() {

}


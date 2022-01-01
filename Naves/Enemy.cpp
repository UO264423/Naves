#include "Enemy.h"
#include "EnemyBase.h"
Enemy::Enemy(float x, float y, Game* game)
	: EnemyBase(x,y,game, "res/enemigo.png", "res/enemigo_movimiento.png")
	{
		vx = -1;
	}
void Enemy::update() {
	// Actualizar la animación
	animation->update();
	x = x + vx;
}


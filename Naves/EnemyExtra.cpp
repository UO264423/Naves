#include "EnemyExtra.h"
#include "EnemyBase.h"
EnemyExtra::EnemyExtra(float x, float y, Game* game)
	: EnemyBase(x, y, game, "res/enemigoExtra.png", "res/enemigoExtra_movimiento.png")
{
	vx = -4;
}
void EnemyExtra::update() {
	// Actualizar la animaci�n
	animation->update();
	
	x = x + vx;
	
}


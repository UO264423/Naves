#include "EnemyExtra.h"
#include "EnemyBase.h"
EnemyExtra::EnemyExtra(float x, float y, Game* game)
	: EnemyBase(x, y, game, "res/enemigoExtra.png", "res/enemigoExtra_movimiento.png")
{
	vx = -4;
}
void EnemyExtra::update() {
	// Actualizar la animación
	animation->update();
	
	x = x + vx;
	
}


#pragma once
#include "Actor.h"
#include "Animation.h"
class PowerUpDisparos : public Actor
{
public:
	PowerUpDisparos(float x, float y, Game* game);
	void draw() override; //Va a sobreescribir
	void update();
	Animation* aMoving;
	Animation* animation; //Referencia a la animacion mostrada
};

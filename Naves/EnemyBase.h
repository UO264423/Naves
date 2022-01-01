#pragma once
#include "Actor.h"
#include "Animation.h"
class EnemyBase : public Actor
{
public:
	EnemyBase(float x, float y, Game* game, string drawName, string animationName);
	void draw() override; //Va a sobreescribir
	virtual void update();
	Animation* aMoving;
	Animation* animation; //Referencia a la animacion mostrada
};


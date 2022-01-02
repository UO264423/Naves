#pragma once
#include "Actor.h"
class Bomb : public Actor
{
public:
	Bomb(float x, float y, Game* game);
	int x;
	int y;
	Game* game; // referencia al juego
};


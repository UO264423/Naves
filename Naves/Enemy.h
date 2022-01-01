#pragma once
#include "Actor.h"
#include "EnemyBase.h"
class Enemy : public EnemyBase
{
public:
	Enemy(float x, float y, Game* game);
	void update() override;
};
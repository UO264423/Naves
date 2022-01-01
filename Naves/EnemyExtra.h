#pragma once
#include "Actor.h"
#include "EnemyBase.h"
class EnemyExtra : public EnemyBase
{
public:
	EnemyExtra(float x, float y, Game* game);
	void update() override;
};


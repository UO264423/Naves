#pragma once
#include "Layer.h"
#include "Player.h"
#include "BackGround.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h"

#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	Text* textPoints;
	Audio* audioBackground;
	int points;
	int newEnemyTime = 0;
	Actor* backgroundPoints;
	Player* player;
	Background* background;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	list<Enemy*> enemies;
	list<Projectile*> projectiles;
};


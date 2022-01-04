#pragma once
#include "Layer.h"
#include "Player.h"
#include "BackGround.h"
#include "Enemy.h"
#include "EnemyExtra.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h"
#include "Bomb.h"
#include "PowerUpDisparos.h"
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
	Text* textVida;
	Text* textDisparos;
	Audio* audioBackground;
	Audio* audioBomba;
	int points;
	int disparos;

	int newEnemyTime = 0;
	int newBombTime = 100;
	Actor* backgroundPoints;
	Actor* vidaPoints;
	Actor* disparosImg;
	Player* player;
	Background* background;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;
	int vida = 3;
	int newPowerUpTime = 200;
	int tiempoMonedas = 340;
	

	list<PowerUpDisparos*> listPowerUp;
	list<Actor*> monedas;
	list<EnemyBase*> enemies;
	list<Bomb*> bombas;
	list<Projectile*> projectiles;
};


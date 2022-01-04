#include "GameLayer.h"
#include "Text.h"

GameLayer::GameLayer(Game* game) :Layer(game) {
	//llama al constructor padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();
	audioBomba = new Audio("/res/efecto_explosion.wav",false);
	points = 0;
	vida = 3;
	disparos = 7;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);
	textDisparos = new Text("hola", WIDTH * 0.88, HEIGHT * 0.92, game);
	textDisparos->content = to_string(disparos);
	textVida = new Text("hola", WIDTH * 0.12, HEIGHT * 0.04, game);
	textVida->content = to_string(vida);
	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5,-1, game);
	backgroundPoints = new Actor("res/icono_puntos.png", WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);
	disparosImg = new Actor("res/disparo_jugador2.png", WIDTH * 0.79, HEIGHT * 0.92, 24, 24, game);
	vidaPoints = new Actor("res/corazon.png", WIDTH * 0.05, HEIGHT * 0.05, 44, 36, game);
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	bombas.clear();
	listPowerUp.clear();
	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new Enemy(300, 50, game));
	enemies.push_back(new EnemyExtra(300, 200, game));
}
void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	//procesar controles
	// Disparar
	if (controlShoot) {
			Projectile* newProjectile = player->shoot();
			if (newProjectile != NULL) {
				projectiles.push_back(newProjectile);
			}
	}
	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}
	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}

}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			if (!controlShoot) {
				if (disparos > 0 && player->shootTime==0) {
					disparos--;
					textDisparos->content = to_string(disparos);
					controlShoot = true;
				}
			}
			break;
		}
	}
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}
	}
}

void GameLayer::update() {
	background->update();
	// Generar enemigos
	newEnemyTime--;
	newBombTime--;
	newPowerUpTime--;
	tiempoMonedas--;
	cout << newPowerUpTime << endl;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		rX = (rand() % (600 - 500)) + 1 + 500;
		rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new EnemyExtra(rX, rY, game));
		newEnemyTime = 110;
	}
	if (newBombTime <= 0) {
		int rX = (rand() % (460));
		int rY = (rand() % (300 - 60));
		bombas.push_back(new Bomb(rX, rY, game));
		newBombTime = 600;
	}
	if (newPowerUpTime <= 0) {
		int rX = (rand() % (460));
		int rY = (rand() % (300 - 60));
		listPowerUp.push_back(new PowerUpDisparos(rX, rY, game));
		newPowerUpTime = 600;
	}

	if (tiempoMonedas <= 0) {
		tiempoMonedas = 340;
		int rX = (rand() % (460));
		int rY = (rand() % (300 - 60));
		monedas.push_back(new Actor("res/moneda.png", rX, rY, 40, 40, game));
		textPoints->content = to_string(points);
	}

	//-----
	player->update();
	for (auto const& enemy : enemies) {
		enemy->update();
	}
	for (auto const& pw : listPowerUp) {
		pw->update();
	}

	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			vida=vida-1;
			textVida->content = to_string(vida);
			if (vida < 0) {
				init();
			}
			enemies.remove(enemy);
			return; // Cortar el for
		}
	}

	
	//Colisiones, Enemy - Proyectile
	list<EnemyBase*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<Bomb*> deleteBombas;
	list<PowerUpDisparos*> deletePW;
	list<Actor*> deleteMonedas;
	//Colision bomba jugador
	for (auto const& bomba : bombas) {
		if (player->isOverlap(bomba)) {
			//Se choca con la bomba
			audioBomba->play();
			enemies.clear();
			bombas.clear();
			return; // Cortar el for
		}
	}

	for (auto const& moneda : monedas) {
		if (player->isOverlap(moneda)) {
			points += 10;
			deleteMonedas.push_back(moneda);
		}
	}

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender() == false) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();
			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}
	for (auto const& pw : listPowerUp) {
		if (player->isOverlap(pw)) {
			deletePW.push_back(pw);
			disparos += 10;
			textDisparos->content = to_string(disparos);
		}
	}


	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();
				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end();
				if (!eInList) {
					deleteEnemies.push_back(enemy);
				}
				points++;
				textPoints->content = to_string(points);
			}
		}
	}
	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}

	for (auto const& delPW : deletePW) {
		listPowerUp.remove(delPW);
	} 

	deleteEnemies.clear();
	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		delete delProjectile;
	}

	for (auto const& delMoneda : deleteMonedas) {
		monedas.remove(delMoneda);
		delete delMoneda;
	}

	deleteProjectiles.clear();
	deletePW.clear();
	deleteMonedas.clear();

}
void GameLayer::draw() {
	background->draw();
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}
	
	player->draw();
	for (auto const& enemy : enemies) {
		enemy->draw();
	}
	for (auto const& bomb : bombas) {
		bomb->draw();
	}

	for (auto const& pw : listPowerUp) {
		pw->draw();
	}

	for (auto const& moneda : monedas) {
		moneda->draw();
	}

	textPoints->draw();
	textVida->draw();
	textDisparos->draw();
	backgroundPoints->draw();
	backgroundPoints->draw();
	vidaPoints->draw();
	disparosImg->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}

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
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);
	textVida = new Text("hola", WIDTH * 0.12, HEIGHT * 0.04, game);
	textVida->content = to_string(vida);
	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5,-1, game);
	backgroundPoints = new Actor("res/icono_puntos.png", WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);
	vidaPoints = new Actor("res/corazon.png", WIDTH * 0.05, HEIGHT * 0.05, 44, 36, game);
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	bombas.clear();
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
			controlShoot = true;
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
		bombas.push_back(new Bomb(150, 200, game));
		newBombTime = 600;
	}
	//-----
	player->update();
	for (auto const& enemy : enemies) {
		enemy->update();
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
	deleteEnemies.clear();
	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

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
	textPoints->draw();
	textVida->draw();
	backgroundPoints->draw();
	vidaPoints->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}

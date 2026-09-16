#pragma once

#include <string>

#include "ActorDatabase.h"
#include "ItemDatabase.h"
#include "RoomDatabase.h"
#include "PlayerClassDatabase.h"
#include "GameState.h"

class Game {
private:
	ActorDatabase _actorDatabase;
	ItemDatabase _itemDatabase;
	RoomDatabase _roomDatabase;
	PlayerClassDatabase _playerClassDatabase;

	GameState _gameState;

	bool _running = true;
	bool _inGame = false;

	std::string _previousRoom;

	void mainMenu();
	void gameLoop();

	void handleGameInput(char c);
	void displayCurrentRoom() const;
	void displayInventory();
	void levelUpPlayer();
	void useItem(const std::string& itemId);
	void encounterEnemy(const std::string& enemyId, int enemyLevel);
	void battle(const std::string& enemyId, int enemyLevel);
	void playerDeath();
	void playerVictory(const std::string& enemyId, int enemyLevel);
public:
	void newGame();
	void run();

	bool loadGame(const std::string& filename);
	bool saveGame(const std::string& filename);

	void loadDatabases();
};
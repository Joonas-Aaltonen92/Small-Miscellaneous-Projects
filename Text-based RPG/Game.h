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

	void mainMenu();
	void gameLoop();

	void handleGameInput(char c);
	void displayCurrentRoom() const;
	void displayInventory();
	void levelUpPlayer();
	void useItem(const std::string& itemId);
public:
	void newGame();
	void run();

	bool loadGame(const std::string& filename);
	bool saveGame(const std::string& filename);

	void loadDatabases();
};
#pragma once
#include <string>
#include "ActorDatabase.h"
#include "ItemDatabase.h"
#include "RoomDatabase.h"
#include "GameState.h"

class Game {
private:
	ActorDatabase _actorDatabase;
	ItemDatabase _itemDatabase;
	RoomDatabase _roomDatabase;

	GameState _gameState;
public:
	void newGame() {};
	void run() {}

	bool loadGame(const std::string& filename);
	bool saveGame(const std::string& filename);

	void mainMenuSelect(char c) {
		if (c == '1') {
			newGame();
		}
		if (c == '2') {
			loadGame("save.json");
		}
	}

	void loadDatabases();
};


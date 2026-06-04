#pragma once
#include <string>
#include "ActorDatabase.h"
#include "ItemDatabase.h"
#include "Player.h"

class Game {
private:
	//Needs a reference to the player character, and the databases
	Player _player;
	ActorDatabase _actorDatabase;
	ItemDatabase _itemDatabase;
public:
	Game(const std::string playerName);
	void Run();
};


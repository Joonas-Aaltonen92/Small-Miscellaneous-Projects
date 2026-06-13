#pragma once
#include <string>
#include "ActorDatabase.h"
#include "ItemDatabase.h"
#include "RoomDatabase.h"
#include "Player.h"

class Game {
private:
	Player _player;
	ActorDatabase _actorDatabase;
	ItemDatabase _itemDatabase;
	RoomDatabase _roomDatabase;
public:
	Game(const std::string playerName);
	void Run();
};


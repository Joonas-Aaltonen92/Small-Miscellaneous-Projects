#include "Game.h"

void Game::loadDatabases() {
	_itemDatabase.loadFromJson("items.json");
	_actorDatabase.loadActors();
	_roomDatabase.loadFromJson("rooms.json");
}
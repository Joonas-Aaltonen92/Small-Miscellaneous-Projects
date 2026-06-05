#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "Actor.h"	//Rooms have enemies, NPCs, merchants and containers
#include "Item.h"	//Rooms have containers that can hold items, and merchants that can sell items, or just items randomly lying around/loot
#include "Inventory.h"

class Room {
private:
	std::string _id;
	std::string _description;
	bool _dungeon;//This is a check for enemies that roam around dungeons to not go into towns/safe areas.
	std::vector<std::string> _loot;//This is for items that are just lying around in the room, not in containers (because Actors have their own inventories)
	std::unique_ptr<Actor> _occupant;//This is for an enemy, NPC, merchant or container
public:
	Room(std::string id, std::string description, bool isDungeon) : _id(std::move(id)), _description(std::move(description)), _dungeon(isDungeon) {}
	std::string getID() const { return _id; }
	std::string getDescription() const { return _description; }
	bool IsDungeon() const { return _dungeon; }
};
#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Actor.h"	//Rooms have enemies, NPCs, merchants and containers
#include "Item.h"	//Rooms have containers that can hold items, and merchants that can sell items, or just items randomly lying around/loot
#include "Inventory.h"

enum class Exits {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	UP,
	DOWN,
	NONE,//For jail or something
	COUNT
};

class Room {
private:
	std::string name;
	std::string _description;
	std::string _minorDescription;//For depicting what's in the next room (eg. "To the north, you see [simple description/landmark]...").
	bool _safeZone;//This is a check for enemies that roam around dungeons to not go into towns/safe areas.
	std::unordered_map<Exits, std::string> _exits;//Key is direction, value is Room ID of the connected room.
	std::vector<std::string> _loot;//This is for items that are just lying around in the room, not in containers (because Actors have their own inventories)
	std::unique_ptr<Actor> _occupant;//This is for an enemy, NPC, merchant or container
public:
	const std::string ID;
	Room(std::string id, const std::string name, const std::string description, const std::string minorDescription, bool isSafe) : ID(id), name(name), _description(std::move(description)), _minorDescription(std::move(minorDescription)), _safeZone(isSafe) {}
	~Room() = default;
	Room(const Room& other) = default;
	Room(Room&&) = default;
	Room& operator=(const Room& other) = default;
	Room& operator=(Room&&) = default;
	bool equals(const Room& other) const {
		const auto& r = static_cast<const Room&>(other);
		return name == r.name && _description == r._description && _minorDescription == r._minorDescription && _safeZone == r._safeZone && _exits == r._exits && _loot == r._loot && ((_occupant && other._occupant && *_occupant == *other._occupant) || (!_occupant && !other._occupant));
	}
	bool operator==(const Room& other) const {
		return typeid(*this) == typeid(other) && equals(other);
	}

	std::string getName() const { return name; }
	std::string getDescription() const { return _description; }
	std::string getMinorDescription() const { return _minorDescription; }
	bool IsSafe() const { return _safeZone; }
	std::unordered_map<Exits, std::string> getExits() const { return _exits; }
	std::vector<std::string> getLoot() const { return _loot; }
	Actor* getOccupant() const { return _occupant.get(); }

	std::unique_ptr<Room> clone() const {
		return std::make_unique<Room>(*this);
	}
};
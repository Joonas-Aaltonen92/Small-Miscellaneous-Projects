#pragma once
#include <string>
#include <unordered_map>
#include <vector>

enum class Exits {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	UP,
	DOWN,
	NONE,//For jail or something
	UNKNOWN,
	COUNT
};

struct RoomDefinition {
	std::string id;
	std::string name;
	std::string description;
	std::string minorDescription;//For depicting what's in the next room (eg. "To the north, you see [simple description/landmark]...").
	bool safeZone;//This is a check for enemies that roam around dungeons to not go into towns/safe areas.
	
	std::unordered_map<Exits, std::string> exits;//Key is direction, value is Room ID of the connected room.
	
	std::vector<std::string> npcs;
	std::vector<std::string> containers;
	std::vector<std::string> doors;
	std::vector<std::string> merchants;
	std::vector<std::string> enemies;


	std::unordered_map<std::string, int> loot;//This is for items that are just lying around in the room, not in containers (because Actors have their own inventories)
	[[nodiscard]] bool hasExits() const { return !exits.empty(); }
};
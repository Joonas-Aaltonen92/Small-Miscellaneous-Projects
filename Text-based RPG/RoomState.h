#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>

struct RoomState {
	bool visited = false;

	//Actors/Items removed from a room
	std::unordered_set<std::string> deadEnemies;
	std::unordered_set<std::string> emptiedContainers;
	std::unordered_set<std::string> openedDoors;
	std::unordered_set<std::string> departedNPCs;
	std::unordered_set<std::string> departedMerchants;
	std::unordered_map<std::string, int> itemsPickedUp;

	//Actors/Items added to the room
	std::unordered_set<std::string> spawnedEnemies;
	std::unordered_set<std::string> spawnedContainers;
	std::unordered_set<std::string> spawnedDoors;
	std::unordered_set<std::string> spawnedNPCs;
	std::unordered_set<std::string> spawnedMerchants;
	std::unordered_map<std::string, int> spawnedItems;
};
#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <EnemyState.h>

struct RoomState {
	bool visited = false;

	//Actors/Items removed from a room
	std::vector<EnemyState> deadEnemies;
	std::unordered_set<std::string> emptiedContainers;
	std::unordered_set<std::string> openedDoors;
	std::unordered_set<std::string> departedNPCs;
	std::unordered_set<std::string> departedMerchants;
	std::unordered_map<std::string, int> itemsPickedUp;

	//Actors/Items added to the room
	std::vector<EnemyState> spawnedEnemies;
	std::unordered_set<std::string> spawnedContainers;
	std::unordered_set<std::string> spawnedDoors;
	std::unordered_set<std::string> spawnedNPCs;
	std::unordered_set<std::string> spawnedMerchants;
	std::unordered_map<std::string, int> spawnedItems;
};
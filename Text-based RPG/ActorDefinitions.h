#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Stats.h"


struct NPCDefinition {
	std::string id;
	std::string name;
	std::string description;
	std::vector<std::string> dialogue;
};

struct ContainerDefinition {
	std::string id;
	std::string name;
	std::string description;
	std::vector<std::string> items; // List of item IDs contained in the container
	bool isLocked = false;
	bool isOpened = false;//Opened containers are removed from the game (state needs to be saved in the rooms.json)
	std::vector<std::string> keyIds; //IDs for all keys that can open a the container (iron key, lock pick, master key, etc.)
};

struct DoorDefinition {
	std::string id;
	std::string name;
	std::string description;
	std::vector<std::string> keyIds; //IDs for all keys that can open the door (iron key, lock pick, master key, etc.)
};

struct MerchantDefinition {
	std::string id;
	std::string name;
	std::string description;
	std::vector<std::string> itemsForSale; // List of item IDs that the merchant sells
};

struct EnemyDefinition {
	std::string id;
	std::string name;
	std::string description;

	ActorStats stats;

	std::unordered_map<std::string, int> lootTable; // List of item IDs that the enemy can drop
};
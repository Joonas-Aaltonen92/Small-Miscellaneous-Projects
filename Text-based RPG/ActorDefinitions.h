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
	std::unordered_map<std::string, int> items; // List of item IDs contained in the container
	std::vector<std::string> keys; //IDs for all keys that can open a the container (iron key, lock pick, master key, etc.)
	[[nodiscard]] bool isOpen() const { return !items.empty(); } //If container requires no key to open, it's considered open
};

struct DoorDefinition {
	std::string id;
	std::string name;
	std::string description;
	std::vector<std::string> keys; //IDs for all keys that can open the door (iron key, lock pick, master key, etc.)
};

struct MerchantDefinition {
	std::string id;
	std::string name;
	std::string description;
	std::vector<std::string> stock; // List of item IDs that the merchant sells
};

struct EnemyDefinition {
	std::string id;
	std::string name;
	std::string description;

	int baseExpYield = 0;

	ActorStats stats;

	std::unordered_map<std::string, int> lootTable; // List of item IDs that the enemy can drop
};
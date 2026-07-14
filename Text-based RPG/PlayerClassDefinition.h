#pragma once
#include <string>
#include <unordered_map>
#include "Stats.h"

struct PlayerClassDefinition{
	std::string classId;
	std::string className;
	std::string classDescription;

	std::unordered_map<std::string, int> startingInventory;

	ActorStats baseStats;
};
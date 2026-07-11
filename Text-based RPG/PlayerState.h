#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Stats.h"
#include "ItemDefinition.h"

struct PlayerState {
	std::string name;

	int level = 1;
	int experience = 0;
	int gold = 0;
	int walletSize = 99;

	ActorStats stats;

	std::unordered_map<std::string, int> inventory;
	std::unordered_map<EquipmentSlot, std::vector<std::string>> equipped;
};
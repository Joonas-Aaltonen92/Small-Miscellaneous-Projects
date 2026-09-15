#pragma once
#include <string>
#include <unordered_map>
#include <array>
#include <vector>

#include "Stats.h"
#include "ItemDefinition.h"

struct PlayerState {
	std::string name;
	std::string classId;
	std::string currentRoom;

	int level = 1;
	int experience = 0;
	int gold = 0;
	int walletSize = 99;

	ActorStats stats;
	std::array<float, GROWTH_STAT_COUNT> growthRates{};

	std::unordered_map<std::string, int> inventory;
	std::unordered_map<EquipmentSlot, std::vector<std::string>> equipped;
};
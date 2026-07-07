#include <fstream>
#include <iostream>
#include "ActorDatabase.h"
#include "json.hpp"

namespace {
	CombatStat getCombatStatFromString(const std::string& stat) {
		static const std::unordered_map<std::string, CombatStat> stringToCombatStatMap = {
			{"hp",CombatStat::HP},
			{"maxhp", CombatStat::MAXHP},
			{"mp", CombatStat::MP},
			{"maxmp", CombatStat::MAXMP},
			{"power", CombatStat::POWER},
			{"fortitude", CombatStat::FORTITUDE},
			{"sorcery", CombatStat::SORCERY},
			{"willpower", CombatStat::WILLPOWER},
			{"speed", CombatStat::SPEED},
			{"luck", CombatStat::LUCK},
			{"unknown", CombatStat::UNKNOWN}
		};
		auto it = stringToCombatStatMap.find(stat);
		return it != stringToCombatStatMap.end() ? it->second : CombatStat::UNKNOWN;
	}

	GrowthStat getGrowthStatFromString(const std::string& stat) {
		static const std::unordered_map<std::string, GrowthStat> stringToGrowthStatMap = {
			{"vitality", GrowthStat::VITALITY},
			{"wisdom", GrowthStat::WISDOM},
			{"strength", GrowthStat::STRENGTH},
			{"endurance", GrowthStat::ENDURANCE},
			{"intelligence", GrowthStat::INTELLIGENCE},
			{"resolve", GrowthStat::RESOLVE},
			{"agility", GrowthStat::AGILITY},
			{"fate", GrowthStat::FATE},
			{"unknown", GrowthStat::UNKNOWN}
		};
		auto it = stringToGrowthStatMap.find(stat);
		return it != stringToGrowthStatMap.end() ? it->second : GrowthStat::UNKNOWN;
	}
	ActorStats parseActorStats(const nlohmann::json& jsonStats) {
		ActorStats stats;
		//Setting defaults although all stats should be set up in the json file.
		stats.baseStats.fill(5);
		stats.growthRates.fill(1.1f);
		if (!jsonStats.is_object())
			return stats;
		//Parse base stats
		if (jsonStats.contains("base") && jsonStats["base"].is_object()) {
			auto& base = jsonStats["base"];
			for (auto& [key, value] : base.items()) {
				CombatStat stat = getCombatStatFromString(key);
				if (stat != CombatStat::UNKNOWN && stat != CombatStat::COUNT) {
					stats.baseStats[std::to_underlying(stat)] = value.get<int>();
				}
			}
		}
		//Parse multipliers
		if (jsonStats.contains("growth") && jsonStats["growth"].is_object()) {
			auto& growth = jsonStats["growth"];
			for (auto& [key, value] : growth.items()) {
				GrowthStat stat = getGrowthStatFromString(key);
				if (stat != GrowthStat::UNKNOWN && stat != GrowthStat::COUNT) {
					stats.growthRates[std::to_underlying(stat)] = value.get<float>();
				}
			}
		}
		return stats;
	}
}

bool ActorDatabase::loadFromJson(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "Could not open file: " << filename << std::endl;
		return false;
	}
	nlohmann::json jsonData;
	try {
		file >> jsonData;
	}
	catch(const std::exception& e) {
		std::cerr << "Error reading JSON file: " << e.what() << std::endl;
		return false;
	}


}

const NPCDefinition* ActorDatabase::findNPC(const std::string& id) const {
	auto it = _NPCs.find(id);
	return it != _NPCs.end() ? &it->second : nullptr;
}

const ContainerDefinition* ActorDatabase::findContainer(const std::string& id) const {
	auto it = _containers.find(id);
	return it != _containers.end() ? &it->second : nullptr;
}

const DoorDefinition* ActorDatabase::findDoor(const std::string& id) const {
	auto it = _doors.find(id);
	return it != _doors.end() ? &it->second : nullptr;
}

const MerchantDefinition* ActorDatabase::findMerchant(const std::string& id) const {
	auto it = _merchants.find(id);
	return it != _merchants.end() ? &it->second : nullptr;
}

const EnemyDefinition* ActorDatabase::findEnemy(const std::string& id) const {
	auto it = _enemies.find(id);
	return it != _enemies.end() ? &it->second : nullptr;
}




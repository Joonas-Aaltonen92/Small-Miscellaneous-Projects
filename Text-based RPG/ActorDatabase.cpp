//#include "Stats.h"
//#include "ActorDatabase.h"
//#include "Actor.h"
//#include <utility>
//#include <fstream>
//#include <iostream>
//
//CombatStat test = CombatStat::HP;
//
//namespace {
//	const std::unordered_map<std::string, CombatStat> stringToCombatStatMap = {
//	{"hp",CombatStat::HP},
//	{"maxhp", CombatStat::MAXHP},
//	{"mp", CombatStat::MP},
//	{"maxmp", CombatStat::MAXMP},
//	{"power", CombatStat::POWER},
//	{"fortitude", CombatStat::FORTITUDE},
//	{"sorcery", CombatStat::SORCERY},
//	{"willpower", CombatStat::WILLPOWER},
//	{"speed", CombatStat::SPEED},
//	{"luck", CombatStat::LUCK},
//	};
//
//	const std::unordered_map<std::string, GrowthStat> stringToGrowthStatMap = {
//	 {"vitality", GrowthStat::VITALITY},
//	 {"wisdom", GrowthStat::WISDOM},
//	 {"strength", GrowthStat::STRENGTH},
//	 {"endurance", GrowthStat::ENDURANCE},
//	 {"intelligence", GrowthStat::INTELLIGENCE},
//	 {"resolve", GrowthStat::RESOLVE},
//	 {"agility", GrowthStat::AGILITY},
//	 {"fate", GrowthStat::FATE}
//	};
//}
//
//std::unique_ptr<Actor> ActorDatabase::createActorFromJson(const std::string& id, const nlohmann::json& data) {
//	std::string type = data.value("type", "unknown");
//	std::string name = data.value("name", "unknown");
//	std::string description = data.value("description", "");
//
//
//	if (type == "npc") {
//		std::vector<std::string> dialogue = data["dialogue"];
//		return std::make_unique<NPC>(id, name, description, dialogue);
//	}
//	if (type == "merchant") {
//		auto merchant = std::make_unique<Merchant>(id, name, description, database);
//		const auto inventory = data["inventory"];
//		for (const auto& [ID, quantity] : inventory.items()) {
//			merchant->getInventory().addItem(ID, quantity);
//		}
//		return merchant;
//	}
//	if (type == "container") {
//		auto container = std::make_unique<Container>(id, name, description, database);
//		const auto inventory = data["inventory"];
//		for (const auto& [ID, quantity] : inventory.items()) {
//			container->getInventory().addItem(ID, quantity);
//		}
//		return container;
//	}
//	return nullptr;
//}
//
//bool ActorDatabase::loadFromJson(const std::string& filename) {
//	std::ifstream file(filename);
//	if (!file) {
//		std::cerr << "Could not open file: " << filename << std::endl;
//		return false;
//	}
//
//	nlohmann::json jsonData;
//	try {
//		file >> jsonData;
//	}
//	catch (const std::exception& e) {
//		std::cerr << "Error occured while parsing JSON file: " << e.what() << std::endl;
//		return false;
//	}
//
//	_prototypes.reserve(jsonData.size());
//	for (auto& [id, data] : jsonData.items()) {
//		try {
//			auto actor = createActorFromJson(id, data);
//			if (actor) {
//				_prototypes[id] = std::move(actor);
//				std::println("Loaded actor: {}, ({}).", id, _prototypes[id]->getName());
//			}
//		}
//		catch (const std::exception& e) {
//			std::cerr << "Error loading actor '" << id << "': " << e.what() << std::endl;
//		}
//	}
//	std::println("Loaded {} actors.", _prototypes.size());
//	return true;
//}
//
//std::unique_ptr<Actor> ActorDatabase::createActor(const std::string& actorID) const {
//	auto it = _prototypes.find(actorID);
//	if (it != _prototypes.end()) {
//		return it->second->clone();
//	}
//	return nullptr;
//}
//
//const Actor* ActorDatabase::getPrototype(const std::string& actorID) const {
//	auto it = _prototypes.find(actorID);
//	return it != _prototypes.end() ? it->second.get() : nullptr;
//}
//
//ActorStats ActorDatabase::parseActorStats(const nlohmann::json& jsonStats) {
//	ActorStats stats;
//
//	//Setting defaults although all stats should be set up in the json file.
//	stats.baseStats.fill(5);
//	stats.growthRates.fill(1.1f);
//
//	if (!jsonStats.is_object())
//		return stats;
//
//	//Parse base stats
//	if (jsonStats.contains("base") && jsonStats["base"].is_object()) {
//		auto& base = jsonStats["base"];
//		for (auto& [key, value] : base.items()){
//			CombatStat stat = getCombatStatFromString(key);
//			if (stat != CombatStat::UNKNOWN && stat != CombatStat::COUNT) {
//				stats.baseStats[std::to_underlying(stat)] = value.get<int>();
//			}
//		}
//	}
//	//Parse multipliers
//	if (jsonStats.contains("growth") && jsonStats["growth"].is_object()) {
//		auto& growth = jsonStats["growth"];
//		for (auto& [key, value] : growth.items()) {
//			GrowthStat stat = getGrowthStatFromString(key);
//			if (stat != GrowthStat::UNKNOWN && stat != GrowthStat::COUNT) {
//				stats.growthRates[std::to_underlying(stat)] = value.get<float>();
//			}
//		}
//	}
//	return stats;
//}
//
//CombatStat ActorDatabase::getCombatStatFromString(const std::string& combatStat) const {
//	auto it = stringToCombatStatMap.find(combatStat);
//	return it != stringToCombatStatMap.end() ? it->second : CombatStat::UNKNOWN;
//}
//
//GrowthStat ActorDatabase::getGrowthStatFromString(const std::string& growthStat) const {
//	auto it = stringToGrowthStatMap.find(growthStat);
//	return it != stringToGrowthStatMap.end() ? it->second : GrowthStat::COUNT;
//}
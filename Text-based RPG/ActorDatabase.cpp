#include <fstream>
#include <iostream>
#include "ActorDatabase.h"
#include "Actor.h"

namespace {
	const std::unordered_map<std::string, CombatStat> stringToCombatStatMap = {
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
	};

	const std::unordered_map<std::string, GrowthStat> stringToGrowthStatMap = {
	 {"vitality", GrowthStat::VITALITY},
	 {"wisdom", GrowthStat::WISDOM},
	 {"strength", GrowthStat::STRENGTH},
	 {"endurance", GrowthStat::ENDURANCE},
	 {"intelligence", GrowthStat::INTELLIGENCE},
	 {"resolve", GrowthStat::RESOLVE},
	 {"agility", GrowthStat::AGILITY},
	 {"fate", GrowthStat::FATE}
	};
}

std::unique_ptr<Actor> ActorDatabase::createActorFromJson(const std::string& id, const nlohmann::json& data) {
	std::string type = data.value("type", "unknown");
	std::string name = data.value("name", "unknown");
	std::string description = data.value("description", "");


	if (type == "npc") {
		std::vector<std::string> dialogue = data["dialogue"];
		return std::make_unique<NPC>(id, name, description, dialogue);
	}
	if (type == "merchant") {
		
	}
	return nullptr;
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
	catch(const std::exception& e){
		std::cerr << "Error occured while parsing JSON file: " << e.what() << std::endl;
		return false;
	}

	_prototypes.reserve(jsonData.size());
	for (auto& [id, data] : jsonData.items()) {
		try {
			auto actor = createActorFromJson(id, data);
			if (actor) {
				_prototypes[id] = std::move(actor);
				std::println("Loaded actor: {}, ({}).",id, _prototypes[id]->getName());
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Error loading actor '" << id << "': " << e.what() << std::endl;
		}
	}
	std::println("Loaded {} actors.", _prototypes.size());
	return true;
}

std::unique_ptr<Actor> ActorDatabase::createActor(const std::string& actorID) const {
	auto it = _prototypes.find(actorID);
	if (it != _prototypes.end()) {
		return it->second->clone();
	}
	return nullptr;
}

const Actor* ActorDatabase::getPrototype(const std::string& actorID) const {
	auto it = _prototypes.find(actorID);
	return it != _prototypes.end() ? it->second.get() : nullptr;
}
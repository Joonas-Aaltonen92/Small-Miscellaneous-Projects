
#include <fstream>
#include <iostream>
#include "RoomDatabase.h"
#include "json.hpp"

namespace {
	Exits getExitFromString(const std::string& exit) {
		static const std::unordered_map<std::string, Exits> stringToExitMap = {
			{"north", Exits::NORTH},
			{"east",Exits::EAST},
			{"south", Exits::SOUTH},
			{"west", Exits::WEST},
			{"up", Exits::UP},
			{"down", Exits::DOWN},
			{"none", Exits::NONE},
			{"unknown",Exits::UNKNOWN}
		};
		const auto it = stringToExitMap.find(exit);
		return it != stringToExitMap.end() ? it->second : Exits::UNKNOWN;
	}
}

bool RoomDatabase::loadFromJson(const std::string& filename) {
	std::fstream file(filename);
	if (!file) {
		std::cerr << "Could not open Room JSON file: " << filename << std::endl;
		return false;
	}
	nlohmann::json jsonData;
	try {
		file >> jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error occured while parsing Room JSON file: " << e.what() << std::endl;
		return false;
	}

	_rooms.reserve(jsonData.size());
	for (const auto& [id, definition] : jsonData.items()) {
		RoomDefinition room;
		room.id = id;
		room.name = definition.value("name", "");
		room.description = definition.value("description","");
		room.minorDescription = definition.value("minorDescription", "");
		room.safeZone = definition.value("safe", true);
		
		if (definition.contains("exits") && definition["exits"].is_object()) {
			for (const auto& [exitString, connectedRoom] : definition["exits"].items()) {
				Exits exit = getExitFromString(exitString);
				room.exits[exit] = connectedRoom.get<std::string>();
			}
		}
		else {//No exits found, but could be intended. Warning just in case
			std::cout << "WARNING: No exits found for Room (" << id << "), check JSON just in case.\n";
		}

		if (definition.contains("npcs") && definition["actors"].is_array()) {
			for (const auto& actorId : definition["actors"]) {
				room.npcs.push_back(actorId.get<std::string>());
			}
		}
		if (definition.contains("containers") && definition["actors"].is_array()) {
			for (const auto& actorId : definition["actors"]) {
				room.containers.push_back(actorId.get<std::string>());
			}
		}
		if (definition.contains("doors") && definition["actors"].is_array()) {
			for (const auto& actorId : definition["actors"]) {
				room.doors.push_back(actorId.get<std::string>());
			}
		}
		if (definition.contains("merchants") && definition["actors"].is_array()) {
			for (const auto& actorId : definition["actors"]) {
				room.merchants.push_back(actorId.get<std::string>());
			}
		}
		if (definition.contains("enemies") && definition["actors"].is_array()) {
			for (const auto& actorId : definition["actors"]) {
				room.enemies.push_back(actorId.get<std::string>());
			}
		}

		if (definition.contains("loot") && definition["loot"].is_object()) {
			for (const auto& [id, quantity] : definition["loot"].items()) {
				room.loot[id] = quantity.get<int>();
			}
		}
		_rooms[id] = room;
	}
	std::cout << "Loaded " << _rooms.size() << " Rooms from file " << filename << std::endl;
	return true;
}

const RoomDefinition* RoomDatabase::find(const std::string& id) const {
	auto it = _rooms.find(id);
	return it != _rooms.end() ? &it->second : nullptr;
}
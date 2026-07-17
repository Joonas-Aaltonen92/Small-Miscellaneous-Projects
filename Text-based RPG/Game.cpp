#include "Game.h"
#include "json.hpp"

namespace {
	std::string equipmentSlotToString(const EquipmentSlot& slot) {
		switch (slot) {
			case EquipmentSlot::HEAD: return "head";
			case EquipmentSlot::CHEST: return "chest";
			case EquipmentSlot::WAIST: return "waist";
			case EquipmentSlot::LEGS: return "legs";
			case EquipmentSlot::FEET: return "feet";
			case EquipmentSlot::ARMS: return "arms";
			case EquipmentSlot::HAND: return "hand";
			case EquipmentSlot::RING: return "ring";
			case EquipmentSlot::AMULET: return "amulet";
			default: return "unknown";
		}
	}

	nlohmann::json serializeEquipment(const std::unordered_map<EquipmentSlot, std::vector<std::string>>& equipment) {
		nlohmann::json jsonData;

		for (const auto& [slot, items] : equipment) {
			jsonData[equipmentSlotToString(slot)] = items;
		}

		return jsonData;
	}
	
	nlohmann::json serializeActorStats(const ActorStats& stats) {
		nlohmann::json jsonData;

		jsonData["baseStats"] = stats.baseStats;
		jsonData["growthRates"] = stats.growthRates;

		return jsonData;
	}
	
	nlohmann::json serializePlayer(const PlayerState& player) {
		nlohmann::json jsonData;
		jsonData["name"] = player.name;
		jsonData["classId"] = player.classId;
		jsonData["level"] = player.level;
		jsonData["skillPoints"] = player.skillPoints;
		jsonData["experience"] = player.experience;
		jsonData["gold"] = player.gold;
		jsonData["walletSize"] = player.walletSize;
		jsonData["inventory"] = player.inventory;
		jsonData["stats"] = serializeActorStats(player.stats);
		jsonData["equipment"] = serializeEquipment(player.equipped);
	}

	EquipmentSlot stringToEquipmentSlot(const std::string& str) {
		static const std::unordered_map<std::string, EquipmentSlot> stringToEquipmentSlotMap = {
			{"head", EquipmentSlot::HEAD},
			{"chest", EquipmentSlot::CHEST},
			{"waist", EquipmentSlot::WAIST},
			{"legs", EquipmentSlot::LEGS},
			{"feet", EquipmentSlot::FEET},
			{"arms", EquipmentSlot::ARMS},
			{"hand", EquipmentSlot::HAND},
			{"ring", EquipmentSlot::RING},
			{"amulet", EquipmentSlot::AMULET},
			{"unknown", EquipmentSlot::UNKNOWN}
		};
		auto it = stringToEquipmentSlotMap.find(str);
		return it != stringToEquipmentSlotMap.end() ? it->second : EquipmentSlot::UNKNOWN;
	}
	std::unordered_map<EquipmentSlot, std::vector<std::string>> deserializeEquipment(const nlohmann::json& jsonData) {
		
		
		if (jsonData.contains("equipment")) {
			for (const auto& [slotName, items] : jsonData.items()) {
				EquipmentSlot slot = stringToEquipmentSlot(slotName);
				
			}
		}
	}

	ActorStats deserializeActorStats(const nlohmann::json& jsonData) {
		ActorStats stats;

		if (jsonData.contains("baseStats")) {
			stats.baseStats = jsonData["baseStats"].get<decltype(stats.baseStats)>();
		}
		if (jsonData.contains("growthRates")) {
			stats.growthRates = jsonData["growthRates"].get<decltype(stats.growthRates)>();
		}

		return stats;
	}
	PlayerState deserializePlayerState(const nlohmann::json& jsonData) {
		PlayerState player;

		player.name = jsonData.value("name", "unknown");
		player.classId = jsonData.value("classId", "unknown class");
		player.level = jsonData.value("level", 0);
		player.skillPoints = jsonData.value("skillPoints", 0);
		player.experience = jsonData.value("experience", 0);
		player.gold = jsonData.value("gold", 0);
		player.walletSize = jsonData.value("walletSize", 0);
		if (jsonData.contains("stats"))
			player.stats = deserializeActorStats(jsonData["stats"]);
		if (jsonData.contains("inventory"))
			player.inventory = jsonData.value("inventory", std::unordered_map<std::string, int>{});
		if (jsonData.contains("equipped")) {
			for (const auto& [slotName, items] : jsonData["equipped"].items()) {
				EquipmentSlot slot = stringToEquipmentSlot(slotName);
				player.equipped[slot] = items.get<std::vector<std::string>>();
			}
		}
		return player;
	}

	nlohmann::json serializeRoomState(const RoomState& room) {

	}
	RoomState deserializeRoomState(const nlohmann::json& jsonData) {

	}

}

void Game::loadDatabases() {
	_itemDatabase.loadFromJson("items.json");
	_actorDatabase.loadActors();
	_roomDatabase.loadFromJson("rooms.json");
}

bool Game::saveGame(const std::string& filename) {

}

bool Game::loadGame(const std::string& filename) {

}
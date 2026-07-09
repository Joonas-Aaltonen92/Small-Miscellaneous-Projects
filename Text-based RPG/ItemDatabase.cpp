#include <fstream>
#include <iostream>
#include "ItemDatabase.h"
#include "json.hpp"

namespace {

	ItemType getItemTypeFromString(const std::string& type) {
		static const std::unordered_map<std::string, ItemType> stringToItemTypeMap = {
			{"equipment", ItemType::EQUIPMENT},
			{"consumable", ItemType::CONSUMABLE},
			{"trinket", ItemType::TRINKET},
			{"key", ItemType::KEY},
			{"keyitem", ItemType::KEYITEM},
			{"unknown", ItemType::UNKNOWN}
		};
		auto it = stringToItemTypeMap.find(type);
		return it != stringToItemTypeMap.end() ? it->second : ItemType::UNKNOWN;
	}

	EquipmentSlot getEquipmentSlotFromString(const std::string& slot) {
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
		auto it = stringToEquipmentSlotMap.find(slot);
		return it != stringToEquipmentSlotMap.end() ? it->second : EquipmentSlot::UNKNOWN;
	}

	CombatStat getCombatStatFromString(const std::string& stat) {
		static const std::unordered_map<std::string, CombatStat> stringToCombatStatMap = {
			{"hp", CombatStat::HP},
			{"maxhp", CombatStat::MAXHP},
			{"mp", CombatStat::MP},
			{"maxmp", CombatStat::MAXMP},
			{"power", CombatStat::POWER},
			{"fortitude", CombatStat::FORTITUDE},
			{"sorcery", CombatStat::SORCERY},
			{"willpower", CombatStat::WILLPOWER},
			{"speed", CombatStat::SPEED},
			{"luck", CombatStat::LUCK}
		};
		auto it = stringToCombatStatMap.find(stat);
		return it != stringToCombatStatMap.end() ? it->second : CombatStat::UNKNOWN;
	}

	StatModifiers parseStatModifiers(const nlohmann::json& jsonMods) {
		StatModifiers mods;
		//Setting defaults so that there battle logic has valid data to work with and no garbage.
		mods.flatModifiers.fill(0);
		mods.multipliers.fill(1.0f);
		if (!jsonMods.is_object())
			return mods;
		//Parse flat modifiers
		if (jsonMods.contains("flat") && jsonMods["flat"].is_object()) {
			auto& flat = jsonMods["flat"];
			for (auto& [key, value] : flat.items()) {
				CombatStat stat = getCombatStatFromString(key);
				if (stat != CombatStat::UNKNOWN && stat != CombatStat::COUNT) {
					mods.flatModifiers[std::to_underlying(stat)] = value.get<int>();
				}
			}
		}
		//Parse multipliers
		if (jsonMods.contains("multipliers") && jsonMods["multipliers"].is_object()) {
			auto& multipliers = jsonMods["multipliers"];
			for (auto& [key, value] : multipliers.items()) {
				CombatStat stat = getCombatStatFromString(key);
				if (stat != CombatStat::UNKNOWN && stat != CombatStat::COUNT) {
					mods.multipliers[std::to_underlying(stat)] = value.get<float>();
				}
			}
		}
		return mods;
	}

}

bool ItemDatabase::loadFromJson(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "Could not open file: " << filename << std::endl;
		return false;
	}

	nlohmann::json jsonData;
	try {
		file >> jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error occured while parsing JSON file: " << e.what() << std::endl;
		return false;
	}

	_items.reserve(jsonData.size());
	for(const auto& [id, definition]: jsonData.items())
	{
		ItemDefinition item;
		item.id = id;
		item.name = definition.value("name", "Unknown Item");
		item.description = definition.value("description", "No description available");
		item.type = getItemTypeFromString(definition.value("type","unknown"));
		item.value = definition.value("value", 0);
		item.modifiers = parseStatModifiers(definition.value("modifiers", nlohmann::json()));
		item.stackable = definition.value("stackable", false);
		item.maxStackSize = definition.value("maxStackSize", 1);

		if (definition.contains("slotCosts") && definition["slotCosts"].is_object()) {
			for (const auto& [slotName, cost] : definition["slotCosts"].items()) {
				EquipmentSlot slot = getEquipmentSlotFromString(slotName);
				item.slotCosts[slot] = cost;
			}
		}

		_items[id] = item;
	}
	std::cout << "Loaded " << _items.size() << " items from " << filename << std::endl;
	return true;
}

const ItemDefinition* ItemDatabase::find(const std::string& id) const {
	auto it = _items.find(id);
	return it != _items.end() ? &it->second : nullptr;
}

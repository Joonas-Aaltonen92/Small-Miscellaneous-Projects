#include "ItemDatabase.h"

namespace {
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
					mods.multipliers[(size_t)stat] = value.get<float>();
				}
			}
		}
		return mods;
	}

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
			{"amulet", EquipmentSlot::AMULET}
		};
		auto it = stringToEquipmentSlotMap.find(slot);
		return it != stringToEquipmentSlotMap.end() ? it->second : EquipmentSlot::HEAD;//Just a default for now
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
		return it != stringToCombatStatMap.end() ? it->second : CombatStat::HP; //Default to HP for now
	}
}

//#include "Stats.h"
//#include "ItemDatabase.h"
//#include <utility>
//#include <fstream>
//#include <iostream>
//
//namespace {
//	const std::unordered_map<std::string, ItemType> stringToItemTypeMap = {
//	{"equipment", ItemType::EQUIPMENT},
//	{"consumable", ItemType::CONSUMABLE},
//	{"trinket", ItemType::TRINKET},
//	{"key", ItemType::KEY},
//	{"keyitem", ItemType::KEYITEM},
//	{"unknown", ItemType::UNKNOWN}
//	};
//
//	const std::unordered_map<std::string, EquipmentSlot> stringToEquipmentSlotMap = {
//		{"head", EquipmentSlot::HEAD},
//		{"chest", EquipmentSlot::CHEST},
//		{"waist", EquipmentSlot::WAIST},
//		{"legs", EquipmentSlot::LEGS},
//		{"body", EquipmentSlot::BODY},
//		{"feet", EquipmentSlot::FEET},
//		{"arms", EquipmentSlot::ARMS},
//		{"hand", EquipmentSlot::HAND},
//		{"ring", EquipmentSlot::RING},
//		{"amulet", EquipmentSlot::AMULET},
//		{"none", EquipmentSlot::NONE}
//	};
//
//	const std::unordered_map<std::string, CombatStat> stringToCombatStatMap = {
//		{"hp", CombatStat::HP},
//		{"maxhp", CombatStat::MAXHP},
//		{"mp", CombatStat::MP},
//		{"maxmp", CombatStat::MAXMP},
//		{"power", CombatStat::POWER},
//		{"fortitude", CombatStat::FORTITUDE},
//		{"sorcery", CombatStat::SORCERY},
//		{"willpower", CombatStat::WILLPOWER},
//		{"speed", CombatStat::SPEED},
//		{"luck", CombatStat::LUCK}
//	};
//}
//
//std::unique_ptr<Item> ItemDatabase::createItemFromJson(const std::string& id, const nlohmann::json& data) {
//	std::string type = data.value("type", "unknown");
//	std::string name = data.value("name", "unknown");
//	std::string description = data.value("description", "");
//	int value = data.value("value", 0);
//	
//	ItemType itemType = getItemTypeFromString(type);
//
//	switch (itemType) {
//	case ItemType::EQUIPMENT:
//		EquipmentSlot slot = getEquipmentSlotFromString(data.value("equipSlot", "none"));
//		StatModifiers modifiers = parseStatModifiers(data.value("statModifiers", nlohmann::json::object()));
//		return std::make_unique<Equipment>(id, name, description, value, itemType, slot, modifiers);
//	case ItemType::CONSUMABLE:
//		StatModifiers modifiers = parseStatModifiers(data["statModifiers"]);
//		return std::make_unique<Consumable>(id, name, description, value, itemType, modifiers);
//	case ItemType::TRINKET:
//		return std::make_unique<Trinket>(id, name, description, value, itemType);
//	case ItemType::KEY:
//		return std::make_unique<Key>(id, name, description, value, itemType);
//	case ItemType::KEYITEM:
//		return std::make_unique<KeyItem>(id, name, description, value, itemType);
//	default:
//		std::println("Unknown item type: {}", type);
//		return nullptr;
//	}
//}
//
//
//bool ItemDatabase::loadFromJson(const std::string& filename) {
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
//			auto item = createItemFromJson(id, data);
//			if (item) {
//				_prototypes[id] = std::move(item);
//				std::cout << "Loaded item: " << id << " (" << _prototypes[id]->getName() << ")\n";
//			}
//		}
//		catch (const std::exception& e) {
//			std::cerr << "Error loading item '" << id << "': " << e.what() << std::endl;
//		}
//	}
//	std::cout << "Loaded " << _prototypes.size() << " items.\n";
//	return true;
//}
//
//std::unique_ptr<Item> ItemDatabase::createItem(const std::string& itemID) const {
//	auto it = _prototypes.find(itemID);
//	if (it != _prototypes.end()) {
//		return it->second->clone();
//	}
//	return nullptr;
//}
//
//StatModifiers ItemDatabase::parseStatModifiers(const nlohmann::json& jsonMods) {
//	StatModifiers mods;
//
//	//Setting defaults so that there battle logic has valid data to work with and no gartbage.
//	mods.flatModifiers.fill(0);
//	mods.multipliers.fill(1.0f);
//
//	if (!jsonMods.is_object())
//		return mods;
//
//	//Parse flat modifiers
//	if (jsonMods.contains("flat") && jsonMods["flat"].is_object()) {
//		auto& flat = jsonMods["flat"];
//		for (auto& [key, value] : flat.items()) {
//			CombatStat stat = getCombatStatFromString(key);
//			if (stat != CombatStat::UNKNOWN && stat != CombatStat::COUNT) {
//				mods.flatModifiers[std::to_underlying(stat)] = value.get<int>();
//			}
//		}
//	}
//	//Parse multipliers
//	if (jsonMods.contains("multipliers") && jsonMods["multipliers"].is_object()) {
//		auto& multipliers = jsonMods["multipliers"];
//		for (auto& [key, value] : multipliers.items()) {
//			CombatStat stat = getCombatStatFromString(key);
//			if (stat != CombatStat::UNKNOWN && stat != CombatStat::COUNT) {
//				mods.multipliers[(size_t)stat] = value.get<float>();
//			}
//		}
//	}
//	return mods;
//}
//
//const Item* ItemDatabase::getPrototype(const std::string& itemID) const {
//	auto it = _prototypes.find(itemID);
//	return it != _prototypes.end() ? it->second.get() : nullptr;
//}
//
//ItemType ItemDatabase::getItemTypeFromString(const std::string& type) const {
//	auto it = stringToItemTypeMap.find(type);
//	return it != stringToItemTypeMap.end() ? it->second : ItemType::UNKNOWN;
//}
//
//EquipmentSlot ItemDatabase::getEquipmentSlotFromString(const std::string& slot) const {
//	auto it = stringToEquipmentSlotMap.find(slot);
//	return it != stringToEquipmentSlotMap.end() ? it->second : EquipmentSlot::NONE;
//}
//
//CombatStat ItemDatabase::getCombatStatFromString(const std::string& stat) const {
//	auto it = stringToCombatStatMap.find(stat);
//	return it != stringToCombatStatMap.end() ? it->second : CombatStat::UNKNOWN;
//}
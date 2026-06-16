#include "ItemDatabase.h"

std::shared_ptr<Item> ItemDatabase::createItemFromJson(const std::string& id, const nlohmann::json& data) {
	std::string type = data.value("type", "unknown");
	std::string name = data.value("name", "unknown");
	std::string description = data.value("description", "");
	int value = data.value("value", 0);
	
	ItemType itemType = getItemTypeFromString(type);

	if (type == "equipment") {
		EquipmentSlot slot = getEquipmentSlotFromString(data.value("equipSlot", "none"));
		StatModifiers modifiers = parseStatModifiers(data["statModifiers"]);
		return std::make_shared<Equipment>(id, name, description, value, itemType, slot, modifiers);
	}
	else if (type == "consumable") {
		StatModifiers modifiers = parseStatModifiers(data["statModifiers"]);
		return std::make_shared<Consumable>(id, name, description, value, itemType, modifiers);
	}
	else if (type == "trinket") {
		return std::make_shared<Trinket>(id, name, description, value, itemType);
	}
	else if (type == "currency") {
		return std::make_shared<Currency>(id, name, description, value, itemType);
	}
	else if (type == "key") {
		return std::make_shared<Key>(id, name, description, value, itemType);
	}
	else if (type == "keyItem") {
		return std::make_shared<KeyItem>(id, name, description, value, itemType);
	}
	//Default fallback (unknown item)
	std::println("Unknown item type: {}", type);
	return nullptr;
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

	for (auto& [id, data] : jsonData.items()) {
		try {
			auto item = createItemFromJson(id, data);
			if (item) {
				_prototypes[id] = std::move(item);
				std::cout << "Loaded item: " << id << " (" << _prototypes[id]->getName() << ")\n";
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Error loading item '" << id << "': " << e.what() << std::endl;
		}
		std::cout << "Loaded " << _prototypes.size() << " items.\n";
		return true;
	}
}

std::shared_ptr<Item> ItemDatabase::createItem(const std::string& itemID) const {
	auto it = _prototypes.find(itemID);
	if (it != _prototypes.end()) {
		return it->second->clone();
	}
	return nullptr;
}

StatModifiers ItemDatabase::parseStatModifiers(const nlohmann::json& jsonMods) {
	StatModifiers mods;

	//Setting defaults so that there battle logic has valid data to work with and no gartbage.
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
				mods.flatModifiers[(size_t)stat] = value.get<int>();
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

const Item* ItemDatabase::getPrototype(const std::string& itemID) const {
	auto it = _prototypes.find(itemID);
	return it != _prototypes.end() ? it->second.get() : nullptr;
}

ItemType ItemDatabase::getItemTypeFromString(const std::string& type) const {
	auto it = stringToItemTypeMap.find(type);
	return it != stringToItemTypeMap.end() ? it->second : ItemType::UNKNOWN;
}

EquipmentSlot ItemDatabase::getEquipmentSlotFromString(const std::string& slot) const {
	auto it = stringToEquipmentSlotMap.find(slot);
	return it != stringToEquipmentSlotMap.end() ? it->second : EquipmentSlot::NONE;
}

CombatStat ItemDatabase::getCombatStatFromString(const std::string& stat) const {
	auto it = stringToCombatStatMap.find(stat);
	return it != stringToCombatStatMap.end() ? it->second : CombatStat::UNKNOWN;
}
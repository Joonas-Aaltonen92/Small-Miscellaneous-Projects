#pragma once
#include <string>
#include <map>
#include <algorithm>
#include <memory>
#include <vector>
#include <utility>

class ItemDatabase;
class Item;

enum class InventorySortMode {
	NAME,
	VALUE,
	TYPE
};

using InventoryEntry = std::pair<std::string, int>;
//Change to
//using InventoryEntry = std::pair<std::string, int>::value_type;
//If getting constructor/conversion errors

class Inventory {
private:
	std::map<std::string, int> stacks;
	const ItemDatabase& database;

public:
	Inventory(ItemDatabase& database) : database(database) {}
	~Inventory() = default;
	Inventory(const Inventory&) = default;
	Inventory(Inventory&&) = default;
	Inventory& operator=(const Inventory&) = delete;
	Inventory& operator=(Inventory&&) = default;

	bool operator==(const Inventory& other) const {
		return stacks == other.stacks;
	}
	bool operator!=(const Inventory& other) const {
		return !(*this == other);
	}

	bool addItem(std::string& itemID, int quantity = 1);
	bool removeItem(const std::string& itemID, int quantity = 1);
	int getQuantity(const std::string& itemID) const;

	const std::map<std::string, int> getStacks() { return stacks; }
	const std::string getItemName(const std::string& itemID);
	const ItemType getItemType(const std::string& itemID);//Maybe not needed
	const int getItemValue(const std::string& itemID);

	std::vector<InventoryEntry> getSortedItems(InventorySortMode mode, bool ascending = true) const;
	std::shared_ptr<Item> getItem(const std::string& itemID);
};
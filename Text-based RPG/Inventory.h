#pragma once
#include <string>
#include <map>
#include <vector>

class ItemDatabase;

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
	explicit Inventory(const ItemDatabase& db) : database(db) {};
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

	bool addItem(const std::string& itemID, int quantity = 1);
	bool removeItem(const std::string& itemID, int quantity = 1);
	int getQuantity(const std::string& itemID) const;

	const std::map<std::string, int>& getStacks() const { return stacks; }
	std::vector<InventoryEntry> getSortedItems(InventorySortMode mode, bool ascending = true) const;
};
#pragma once
#include <string>
#include <map>
#include <algorithm>

class ItemDatabase;

class Inventory {
private:
	std::map<std::string, int> stacks;

	bool equals(const Inventory& other) const {
		return stacks == other.stacks;
	}

	const ItemDatabase& database;

public:
	Inventory(ItemDatabase& database) : database(database) {}
	~Inventory() = default;
	Inventory(const Inventory&) = default;
	Inventory(Inventory&&) = default;
	Inventory& operator=(const Inventory&) = default;
	Inventory& operator=(Inventory&&) = default;

	bool operator==(const Inventory& other) const {
		return typeid(*this) == typeid(other) && equals(other);
	}
	bool operator!=(const Inventory& other) const {
		return !(*this == other);
	}

	bool addItem(std::string itemID, int quantity = 1);
	bool removeItem(const std::string& itemID, int quantity = 1);
	int getQuantity(const std::string& itemID) const;

	void sortByName(bool ascending = true);
	void sortByValue(bool ascending = true);

	void sortByType(bool ascending = true) {
		std::sort(stacks.begin(), stacks.end(), [ascending](const ItemStack& a, const ItemStack& b) {
			if (!a._item || !b._item) return false;
			ItemType typeA = a._item->getType();
			ItemType typeB = b._item->getType();
			if (ascending)
				return typeA < typeB;
			else
				return typeB < typeA;
			});
	}
};
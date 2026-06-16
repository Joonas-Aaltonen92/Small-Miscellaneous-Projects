#pragma once
#include <map>
#include <algorithm>
#include "ItemStack.h"

class Inventory {
private:
	std::map<std::string, ItemStack> stacks;

	bool equals(const Inventory& other) const {
		return stacks == other.stacks;
	}

public:
	Inventory() = default;
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

	bool addItem(std::shared_ptr<Item> item, int quantity = 1) {
		if (item->isStackable()) {
			auto it = stacks.find(item->ID);
			if (it != stacks.end())
				it->second.add(quantity);
			else
				stacks.emplace(item->ID, ItemStack(std::move(item), quantity));
			return true;
		}
		return false;
	}
	bool removeItem(const std::string& itemID, int quantity = 1) {
		auto it = stacks.find(itemID);
		if (it != stacks.end()) {
			it->second.remove(quantity);
			if (it->second.isEmpty())
				stacks.erase(it);
			return true;
		}
		return false;
	}
	int getQuantity(const std::string& itemID) const {
		auto it = stacks.find(itemID);
		if (it != stacks.end())
			return it->second._quantity;
		return 0;
	}

	void sortByName(bool ascending = true) {
		std::sort(stacks.begin(), stacks.end(), [ascending](const ItemStack& a, const ItemStack& b) {
			if (!a._item || !b._item) return false;

			const std::string& nameA = a._item->getName();
			const std::string& nameB = b._item->getName();

			if (ascending)
				return nameA < nameB;
			else
				return nameB < nameA;
			});
	}
	void sortByValue(bool ascending = true) {
		std::sort(stacks.begin(), stacks.end(), [ascending](const ItemStack& a, const ItemStack& b){
			if (!a._item || !b._item) return false;

			int valueA = a._item->getValue();
			int valueB = b._item->getValue();

			if (ascending)
				return valueA < valueB;
			else
				return valueB < valueA;
		});
	}

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

	const std::map<std::string, ItemStack>& getStacks() const { return stacks; }

};
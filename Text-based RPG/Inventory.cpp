#include <algorithm>
#include "Inventory.h"
#include "Item.h"
#include "ItemDatabase.h"

bool Inventory::addItem(std::string itemID, int quantity = 1) {
	const auto& item = database.getPrototype(itemID);
	if (item->isStackable()) {
		stacks[itemID] += quantity;
		return true;
	}
	return false;
}

bool Inventory::removeItem(const std::string& itemID, int quantity = 1) {
	const auto& item = database.getPrototype(itemID);
	auto it = stacks.find(itemID);
	if(it != stacks.end()) {
		it->second -= quantity;
		if(it->second <= 0) {
			stacks.erase(it);
		}
		return true;
	}
	return false;
}

int Inventory::getQuantity(const std::string& itemID) const {
	auto it = stacks.find(itemID);
	if (it != stacks.end())
		return it->second;
	return 0;
}

void Inventory::sortByName(bool ascending) {
	std::sort(stacks.begin(), stacks.end(), [ascending](const auto& a, const auto& b)) {
		const auto& itemA = database.getPrototype(a.first);
		const auto& itemB = database.getPrototype(b.first);
		if(!itemA || !itemB) return false;
		const std::string& nameA = itemA->getName();
		const std::string& nameB = itemB->getName();
		if (ascending)
			return nameA < nameB;
		else
			return nameB < nameA;
	}
}

void Inventory::sortByValue(bool ascending) {
	std::sort(stacks.begin(), stacks.end(), [ascending](const auto& a, const auto& b)) {
		const auto& itemA = database.getPrototype(a.first);
		const auto& itemB = database.getPrototype(b.first);
		if (!itemA || !itemB) return false;
		int valueA = itemA->getValue();
		int valueB = itemB->getValue();
		if (ascending)
			return valueA < valueB;
		else
			return valueB < valueA;
	}
}

void Inventory::sortByType(bool ascending) {
	std::sort(stacks.begin(), stacks.end(), [ascending](const auto& a, const auto& b)) {
		const auto& itemA = database.getPrototype(a.first);
		const auto& itemB = database.getPrototype(b.first);
		if (!itemA || !itemB) return false;
		ItemType typeA = itemA->getType();
		ItemType typeB = itemB->getType();
		if (ascending)
			return typeA < typeB;
		else
			return typeB < typeA;
	}
}
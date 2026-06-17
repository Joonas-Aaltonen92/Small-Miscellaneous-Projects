#include <algorithm>
#include "Inventory.h"
#include "Item.h"
#include "ItemDatabase.h"

bool Inventory::addItem(std::string& itemID, int quantity) {
	const auto& item = database.getPrototype(itemID);
	if (!item) {
		std::println("Item not found: {}", itemID);
		return false;
	}
	if (item->isStackable()) {
		stacks[itemID] += quantity;
		return true;
	}
	return false;
}

bool Inventory::removeItem(const std::string& itemID, int quantity) {
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

std::string Inventory::getItemName(std::string& itemID) {
	const auto& item = database.getPrototype(itemID);
	if (!item) {
		std::println("Item not found: {}",itemID);
		return "Unkown";
	}
	return item->getName();
}

ItemType Inventory::getItemType(std::string& itemID) {
	const auto& item = database.getPrototype(itemID);
	if (!item) {
		std::println("Item not found: {}", itemID);
		return ItemType::UNKNOWN;
	}
	return item->getType();
}

int Inventory::getItemValue(std::string& itemID) {
	const auto& item = database.getPrototype(itemID);
	if (!item) {
		std::println("Item not found: {}", itemID);
		return 0;
	}
	return item->getValue();
}

std::vector<InventoryEntry> Inventory::getSortedItems(InventorySortMode mode, bool ascending) const {
	std::vector<InventoryEntry> items(
		stacks.begin(),
		stacks.end()
	);

	std::sort(items.begin(), items.end(), [this, mode, ascending](const auto& a, const auto& b) {
		const auto& itemA = database.getPrototype(a.first);
		const auto& itemB = database.getPrototype(b.first);

		if (!itemA || !itemB)
			return false;

		switch (mode) {
		case(InventorySortMode::NAME):
			return ascending
				? itemA->getName() < itemB->getName()
				: itemA->getName() > itemB->getName();

		case (InventorySortMode::VALUE):
			return ascending
				? itemA->getValue() < itemB->getValue()
				: itemA->getValue() > itemB->getValue();

		case(InventorySortMode::TYPE):
			return ascending
				? itemA->getType() < itemB->getType()
				: itemA->getType() > itemB->getType();

		default:
			return false;
		}
		});
	return items;
}

std::shared_ptr<Item> Inventory::getItem(const std::string& itemID){
	const auto& item = database.getPrototype(itemID);
	if(item)
		return item->clone();

	std::println("Item not found: {}", itemID);
	return nullptr;
}
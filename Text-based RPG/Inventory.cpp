//#include <algorithm>
//#include "Inventory.h"
//#include "Item.h"
//#include "ItemDatabase.h"
//
//bool Inventory::addItem(const std::string& itemID, int quantity) {
//	const auto& item = database.getPrototype(itemID);
//	if (!item) {
//		std::println("Item not found: {}", itemID);
//		return false;
//	}
//	
//	stacks[itemID] += quantity;
//	return true;
//}
//
//bool Inventory::removeItem(const std::string& itemID, int quantity) {
//	auto it = stacks.find(itemID);
//	if(it != stacks.end()) {
//		it->second -= quantity;
//		if(it->second <= 0) {
//			stacks.erase(it);
//		}
//		return true;
//	}
//	return false;
//}
//
//int Inventory::getQuantity(const std::string& itemID) const {
//	auto it = stacks.find(itemID);
//	if (it != stacks.end())
//		return it->second;
//	return 0;
//}
//
//std::vector<InventoryEntry> Inventory::getSortedItems(InventorySortMode mode, bool ascending) const {
//	struct SortedItemView {
//		std::string id;
//		int quantity;
//		const Item* item;
//	};
//
//	std::vector<SortedItemView> items;
//	items.reserve(stacks.size());
//
//	for (const auto& [id, qty] : stacks) {
//		items.push_back({ id,qty,database.getPrototype(id) });
//	}
//
//	std::sort(items.begin(), items.end(), [mode, ascending](const auto& a, const auto& b) {
//
//		if (!a.item || !b.item)
//			return false;
//
//		switch (mode) {
//		case(InventorySortMode::NAME):
//			return ascending
//				? a.item->getName() < b.item->getName()
//				: a.item->getName() > b.item->getName();
//
//		case (InventorySortMode::VALUE):
//			return ascending
//				? a.item->getValue() < b.item->getValue()
//				: a.item->getValue() > b.item->getValue();
//
//		case(InventorySortMode::TYPE):
//			return ascending
//				? a.item->getType() < b.item->getType()
//				: a.item->getType() > b.item->getType();
//		}
//		return false;
//		});
//
//	std::vector<InventoryEntry> result;
//	result.reserve(items.size());
//	for (const auto& i : items) {
//		result.emplace_back(i.id, i.quantity);
//	}
//
//	return result;
//}
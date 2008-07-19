#include "ItemTracker.h"

/* constructors */
ItemTracker::ItemTracker(Saiph *saiph) : saiph(saiph) {
}

/* methods */
void ItemTracker::addItemToInventory(const Item &item) {
}

void ItemTracker::addItemToStash(const Item &item) {
	Coordinate coordinate(saiph->current_branch, saiph->current_level, saiph->world->player.row, saiph->world->player.col);
}

void ItemTracker::moveItemFromInventoryToStash(const Item &item) {
	Coordinate coordinate(saiph->current_branch, saiph->current_level, saiph->world->player.row, saiph->world->player.col);
}

void ItemTracker::moveItemFromStashToInventory(const Item &item) {
	Coordinate coordinate(saiph->current_branch, saiph->current_level, saiph->world->player.row, saiph->world->player.col);
}

void ItemTracker::removeItemFromInventory(const Item &item) {
}

void ItemTracker::removeItemFromStash(const Item &item) {
	Coordinate coordinate(saiph->current_branch, saiph->current_level, saiph->world->player.row, saiph->world->player.col);
}

/* private methods */
void ItemTracker::addItemToStash(const Coordinate &coordinate, const Item &item) {
	for (list<Stash>::iterator s = stashes.begin(); s != stashes.end(); ++s) {
		if (s->branch != coordinate.branch || s->level != coordinate.level || s->row != coordinate.row || s->col != coordinate.col)
			continue;
		s->addItem(item);
		return;
	}
}

void ItemTracker::removeItemFromStash(const Coordinate &coordinate, const Item &item) {
	for (list<Stash>::iterator s = stashes.begin(); s != stashes.end(); ++s) {
		if (s->branch != coordinate.branch || s->level != coordinate.level || s->row != coordinate.row || s->col != coordinate.col)
			continue;
		s->removeItem(item);
		if (s->items.size() <= 0)
			stashes.erase(s);
		return;
	}
}

#include "ItemTracker.h"

/* constructors */
ItemTracker::ItemTracker(Saiph *saiph) : saiph(saiph) {
}

/* methods */
void ItemTracker::addItemToInventory(const Item &item) {
}

void ItemTracker::addItemToStash(const Item &item) {
	Coordinate coordinate(saiph->position.branch, saiph->position.level, saiph->world->player.row, saiph->world->player.col);
}

void ItemTracker::moveItemFromInventoryToStash(const Item &item) {
	Coordinate coordinate(saiph->position.branch, saiph->position.level, saiph->world->player.row, saiph->world->player.col);
}

void ItemTracker::moveItemFromStashToInventory(const Item &item) {
	Coordinate coordinate(saiph->position.branch, saiph->position.level, saiph->world->player.row, saiph->world->player.col);
}

void ItemTracker::parseMessages(const string &messages) {
	string::size_type pos;
	if ((pos == messages.find(MESSAGE_YOU_SEE_HERE, 0)) != string::npos) {
		/* single item on ground */
		pos += sizeof (MESSAGE_YOU_SEE_HERE) - 1;
		string::size_type length = messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			Item item = parseItemText(messages.substr(pos, length));
			addItemToStash(item);
		}
	} else if ((pos == messages.find(MESSAGE_THINGS_THAT_ARE_HERE, 0)) != string::npos) {
		/* multiple items on ground */
	} else if ((pos == messages.find(MESSAGE_YOU_SEE_NO_OBJECTS, 0)) != string::npos) {
		/* no items on ground */
		clearStash(Coordinate(saiph->position.branch, saiph->position.level, saiph->world->player.row, saiph->world->player.col));
	} else if ((pos == messages.find(MESSAGE_PICK_UP_WHAT, 0)) != string::npos) {
		/* should we handle this here? */
	}
}

void ItemTracker::removeItemFromInventory(const Item &item) {
}

void ItemTracker::removeItemFromStash(const Item &item) {
	Coordinate coordinate(saiph->position.branch, saiph->position.level, saiph->world->player.row, saiph->world->player.col);
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

void ItemTracker::clearStash(const Coordinate &coordinate) {
	for (list<Stash>::iterator s = stashes.begin(); s != stashes.end(); ++s) {
		if (s->branch != coordinate.branch || s->level != coordinate.level || s->row != coordinate.row || s->col != coordinate.col)
			continue;
		stashes.erase(s);
		return;
	}
}

Item ItemTracker::parseItemText(const string &text) {
	Item item("", -1);
	char amount[8];
	char name[128];
	int matched = sscanf(text.c_str(), GET_SINGLE_ITEM, amount, name);
	if (matched != 2)
		return item; // unable to parse text as item
	/* figure out amount of items */
	if ((amount[0] == 'a' && (amount[1] == '\0' || (amount[1] == 'n' && amount[2] == '\0'))) || (amount[0] == 't' && amount[1] == 'h' && amount[2] == 'e' && amount[3] == '\0'))     
		item.count = 1; // "a", "an" or "the" <item>
	else if (amount[0] >= '0' || amount[0] <= '9')
		item.count = atoi(amount); // n <items>
	else    
		return item; // unable to parse text as item
	item.name = name;
	return item;
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

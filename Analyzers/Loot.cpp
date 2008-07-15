#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) {
	this->saiph = saiph;
}

/* methods */
void Loot::command(string *command) {
	*command = action;
}

int Loot::finish() {
	return 0;
}

void Loot::inspect(const Point &point) {
	/*
	symbols.push_back(WEAPON);
	symbols.push_back(ARMOR);
	symbols.push_back(RING);
	symbols.push_back(AMULET);
	symbols.push_back(TOOL);
	symbols.push_back(FOOD);
	symbols.push_back(POTION);
	symbols.push_back(SCROLL);
	symbols.push_back(SPELLBOOK);
	symbols.push_back(WAND);
	symbols.push_back(GOLD);
	symbols.push_back(GEM);
	symbols.push_back(STATUE);
	// skipping boulder as that's a special item
	symbols.push_back(IRON_BALL);
	symbols.push_back(CHAINS);
	symbols.push_back(VENOM);
	*/
	return;
}

int Loot::parseMessages(string *messages) {
	string::size_type pos = messages->find(MESSAGE_YOU_SEE_HERE, 0);
	if (pos != string::npos) {
		/* one item on the floor */
		pos += sizeof (MESSAGE_YOU_SEE_HERE) - 1;
		string::size_type length = messages->find(".  ", pos);                                                                           
		if (length != string::npos) {                                                                                                          
			length = length - pos;
			int b = saiph->current_branch;
			int l = saiph->current_level;
			int r = saiph->world->player.row;
			int c = saiph->world->player.col;
			vector<Item> *stash = &stashes[b][l][r][c].items;
			if (stash->size() > 0) {
				/* we know about this stash already.
				 * ditch the previous content of this stash */
				stash->clear();
			} else {
				/* new stash, add it to stash_locations */
				stash_locations.push_back(Coordinate(b, l, r, c));
			}
			parseMessageItem(messages->substr(pos, length), stash);
		}
	}
	pos = messages->find(MESSAGE_THINGS_THAT_ARE_HERE, 0);
	if (pos != string::npos) {
		/* multiple items on the floor */
		int b = saiph->current_branch;
		int l = saiph->current_level;
		int r = saiph->world->player.row;
		int c = saiph->world->player.col;
		vector<Item> *stash = &stashes[b][l][r][c].items;
		if (stash->size() > 0) {
			/* we know about this stash already.
			 * ditch the previous content of this stash */
			stash->clear();
		} else {
			/* new stash, add it to stash_locations */
			stash_locations.push_back(Coordinate(b, l, r, c));
		}
		pos = messages->find("  ", pos);
		while (pos != string::npos) {
			pos += 2;
			string::size_type length = messages->find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			parseMessageItem(messages->substr(pos, length), stash);
			pos += length;
		}
	}
	return 0;
}

/* private methods */
void Loot::parseMessageItem(const string &message, vector<Item> *stash) {
	char amount[16];
	char name[512];
	int matched = sscanf(message.c_str(), GET_SINGLE_ITEM, amount, name);
	if (matched != 2) {
		cerr << "ERROR: matched " << matched << ", expected 2" << endl;
		exit(1);
	}
	/* figure out amount of items */
	int count;
	if (amount[0] < '0' || amount[0] > '9')
		count = 1; // "a", "an" or "the" <item>
	else    
		count = atoi(amount);
	/* add item to stash */
	stash->push_back(Item(name, count));
}

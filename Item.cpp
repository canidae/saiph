#include <stdlib.h>
#include "Globals.h"
#include "Item.h"

using namespace std;

/* constructors/destructor */
Item::Item(const string &text) : name(""), count(0), beatitude(BEATITUDE_UNKNOWN), greased(false), fixed(false), damage(0), unknown_enchantment(true), enchantment(0), additional("") {
	/* parse text */
	char amount[8];
	char name_long[128];
	int matched = sscanf(text.c_str(), ITEM_PARSE_TEXT, amount, name_long);
	if (matched != 2)
		return; // unable to parse text as item
	/* figure out amount of items */
	if ((amount[0] == 'a' && (amount[1] == '\0' || (amount[1] == 'n' && amount[2] == '\0'))) || ((amount[0] == 't' || amount[0] == 'T') && amount[1] == 'h' && amount[2] == 'e' && amount[3] == '\0'))     
		count = 1; // "a", "an" or "the" <item>
	else if (amount[0] >= '0' || amount[0] <= '9')
		count = atoi(amount); // n <items>
	else    
		return; // unable to parse text as item
	string::size_type pos = 0;
	name = name_long;
	/* buc */
	if (name.find(ITEM_PARSE_BLESSED, pos) == pos) {
		beatitude = BLESSED;
		pos += sizeof (ITEM_PARSE_BLESSED) - 1;
	} else if (name.find(ITEM_PARSE_UNCURSED, pos) == pos) {
		beatitude = UNCURSED;
		pos += sizeof (ITEM_PARSE_UNCURSED) - 1;
	} else if (name.find(ITEM_PARSE_CURSED, pos) == pos) {
		beatitude = CURSED;
		pos += sizeof (ITEM_PARSE_CURSED) - 1;
	} else {
		beatitude = BEATITUDE_UNKNOWN;
	}
	/* greased */
	if (name.find(ITEM_PARSE_GREASED, pos) == pos) {
		greased = true;
		pos += sizeof (ITEM_PARSE_GREASED) - 1;
	} else {
		greased = false;
	}
	/* fixed */
	if (name.find(ITEM_PARSE_FIREPROOF, pos) == pos) {
		fixed = true;
		pos += sizeof (ITEM_PARSE_FIREPROOF) - 1;
	} else if (name.find(ITEM_PARSE_RUSTPROOF, pos) == pos) {
		fixed = true;
		pos += sizeof (ITEM_PARSE_RUSTPROOF) - 1;
	} else {
		fixed = false;
	}
	/* damage */
	if (name.find(ITEM_PARSE_THOROUGHLY_RUSTY, pos) == pos) {
		if (damage < 3)
			damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_RUSTY) - 1;
	} else if (name.find(ITEM_PARSE_VERY_RUSTY, pos) == pos) {
		if (damage < 2)
			damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_RUSTY) - 1;
	} else if (name.find(ITEM_PARSE_RUSTY, pos) == pos) {
		if (damage < 1)
			damage = 1;
		pos += sizeof (ITEM_PARSE_RUSTY) - 1;
	}
	if (name.find(ITEM_PARSE_THOROUGHLY_BURNT, pos) == pos) {
		if (damage < 3)
			damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_BURNT) - 1;
	} else if (name.find(ITEM_PARSE_VERY_BURNT, pos) == pos) {
		if (damage < 2)
			damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_BURNT) - 1;
	} else if (name.find(ITEM_PARSE_BURNT, pos) == pos) {
		if (damage < 1)
			damage = 1;
		pos += sizeof (ITEM_PARSE_BURNT) - 1;
	}
	if (name.find(ITEM_PARSE_THOROUGHLY_CORRODED, pos) == pos) {
		if (damage < 3)
			damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_CORRODED) - 1;
	} else if (name.find(ITEM_PARSE_VERY_CORRODED, pos) == pos) {
		if (damage < 2)
			damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_CORRODED) - 1;
	} else if (name.find(ITEM_PARSE_CORRODED, pos) == pos) {
		if (damage < 1)
			damage = 1;
		pos += sizeof (ITEM_PARSE_CORRODED) - 1;
	}
	if (name.find(ITEM_PARSE_THOROUGHLY_ROTTED, pos) == pos) {
		if (damage < 3)
			damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_ROTTED) - 1;
	} else if (name.find(ITEM_PARSE_VERY_ROTTED, pos) == pos) {
		if (damage < 2)
			damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_ROTTED) - 1;
	} else if (name.find(ITEM_PARSE_ROTTED, pos) == pos) {
		if (damage < 1)
			damage = 1;
		pos += sizeof (ITEM_PARSE_ROTTED) - 1;
	}
	/* enchantment */
	if (name[pos] == '+') {
		unknown_enchantment = false;
		enchantment = name[pos + 1] - '0'; // assuming no item is enchanted beyond +9
		pos += 3;
		/* if we know enchantment, we (probably) know that it's uncursed unless it says otherwise */
		/* trying to comment out this
		 * i believe some stuff don't say "uncursed" after dropping them on an altar,
		 * so this is probably going to fail miserably
		if (beatitude == BEATITUDE_UNKNOWN)
			beatitude = UNCURSED;
		 */
	} else if (name[pos] == '-') {
		unknown_enchantment = false;
		enchantment = 0 - (name[pos + 1] - '0'); // assuming no item is enchanted beyond -9
		pos += 3;
		/* if we know enchantment, we (probably) know that it's uncursed unless it says otherwise */
		/* trying to comment out this
		 * i believe some stuff don't say "uncursed" after dropping them on an altar,
		 * so this is probably going to fail miserably
		if (beatitude == BEATITUDE_UNKNOWN)
			beatitude = UNCURSED;
		 */
	}
	/* erase up to pos as we've extracted all the info up to item name */
	name.erase(0, pos);
	/* set "(being worn)" and so on in additional */
	if (name[name.size() - 1] == ')') {
		pos = name.rfind("(", name.size() - 1);
		if (pos != string::npos) {
			additional = name.substr(pos + 1, name.size() - pos - 2); // ditch "(" and ")"
			name.erase(pos - 1); // no need for this in name, and remove last space before "(" too
		}
	}
	/* if items are named something (else than "blessed", "uncursed" or "cursed"), replace name with that */
	pos = name.rfind(ITEM_NAMED, name.size() - 1);
	if (pos != string::npos) {
		string named = name.substr(pos + sizeof (ITEM_NAMED) - 1);
		int tmpbeatitude = BEATITUDE_UNKNOWN;
		if (named == "blessed")
			tmpbeatitude = BLESSED;
		else if (named == "uncursed")
			tmpbeatitude = UNCURSED;
		else if (named == "cursed")
			tmpbeatitude = CURSED;
		else if (named.find("The ", 0) == 0)
			named = named.substr(4);
		if (tmpbeatitude != BEATITUDE_UNKNOWN) {
			/* item was named "blessed", "uncursed" or "cursed".
			 * unless we already know beatitude we should set beatitude
			 * and clear "named" */
			if (beatitude == BEATITUDE_UNKNOWN)
				beatitude = tmpbeatitude;
			named.clear();
		}
		/* if items are named something (else than "blessed", "uncursed" or "cursed"), replace name with that */
		if (!named.empty())
			name = named;
		else
			name.erase(pos);
	}
	/* if items are called something, replace name with that */
	pos = name.rfind(ITEM_CALLED, name.size() - 1);
	if (pos != string::npos)
		name = name.substr(pos + sizeof (ITEM_CALLED) - 1);
	/* singularize name.
	 * we'll only singularize stuff we care about for now */
	if (name.find("pair of ") == 0)
		return; // it's "2 pair of boots", not "2 pairs of boot"
	string::size_type stop = string::npos;
	if ((stop = name.find(" of ", 0)) != string::npos || (stop = name.find(" labeled ", 0)) != string::npos || (stop = name.find(" called ", 0)) != string::npos || (stop = name.find(" named ", 0)) != string::npos || (stop = name.find(" from ", 0)) != string::npos) {
		/* no need to do anything here.
		 * after this check "stop" should be placed just after the word that may be pluralized */
	}
	if (stop == string::npos)
		stop = name.size();
	string::size_type start = name.find_last_of(' ', stop - 1);
	if (start == string::npos)
		start = 0;
	else
		++start; // or we'll get the space before the word
	string word = name.substr(start, stop - start);
	if (word == "leaves")
		word = "leaf";
	else if (word.size() >= 2 && word[word.size() - 1] == 's' && word[word.size() - 2] != 's')
		word.erase(word.size() - 1);
	name.replace(start, stop, word);
}

Item::Item() : name(""), count(0), beatitude(BEATITUDE_UNKNOWN), greased(false), fixed(false), damage(0), enchantment(0), additional("") {
}

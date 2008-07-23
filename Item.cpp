#include "Item.h"

/* constructors */
Item::Item(const string &text) : name(""), count(0), buc_status(BUC_UNKNOWN), greased(false), fixed(false), damage(0), enchantment(0), additional("") {
	/* parse text */
	char amount[8];
	char name_long[128];
	int matched = sscanf(text.c_str(), ITEM_PARSE_TEXT, amount, name_long);
	if (matched != 2)
		return; // unable to parse text as item
	/* figure out amount of items */
	if ((amount[0] == 'a' && (amount[1] == '\0' || (amount[1] == 'n' && amount[2] == '\0'))) || (amount[0] == 't' && amount[1] == 'h' && amount[2] == 'e' && amount[3] == '\0'))     
		count = 1; // "a", "an" or "the" <item>
	else if (amount[0] >= '0' || amount[0] <= '9')
		count = atoi(amount); // n <items>
	else    
		return; // unable to parse text as item
	/* blessed greased fireproof +3 cloak of magic resistance
	 *  <buc>    <g>    <fixed>  <>   <item> */
	string::size_type pos = 0;
	name = name_long;
	/* buc */
	if (name.find(ITEM_PARSE_BLESSED, pos) == 0) {
		buc_status = BLESSED;
		pos += sizeof (ITEM_PARSE_BLESSED) - 1;
	} else if (name.find(ITEM_PARSE_UNCURSED, pos) == 0) {
		buc_status = UNCURSED;
		pos += sizeof (ITEM_PARSE_UNCURSED) - 1;
	} else if (name.find(ITEM_PARSE_CURSED, pos) == 0) {
		buc_status = CURSED;
		pos += sizeof (ITEM_PARSE_CURSED) - 1;
	} else {
		buc_status = BUC_UNKNOWN;
	}
	/* greased */
	if (name.find(ITEM_PARSE_GREASED, pos) == 0) {
		greased = true;
		pos += sizeof (ITEM_PARSE_GREASED) - 1;
	} else {
		greased = false;
	}
	/* fixed */
	if (name.find(ITEM_PARSE_FIREPROOF, pos) == 0) {
		fixed = true;
		pos += sizeof (ITEM_PARSE_FIREPROOF) - 1;
	} else if (name.find(ITEM_PARSE_RUSTPROOF, pos) == 0) {
		fixed = true;
		pos += sizeof (ITEM_PARSE_RUSTPROOF) - 1;
	} else {
		fixed = false;
	}
	/* damage */
	if (name.find(ITEM_PARSE_THOROUGHLY_BURNT, pos) == 0) {
		damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_BURNT) - 1;
	} else if (name.find(ITEM_PARSE_THOROUGHLY_CORRODED, pos) == 0) {
		damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_CORRODED) - 1;
	} else if (name.find(ITEM_PARSE_THOROUGHLY_ROTTED, pos) == 0) {
		damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_ROTTED) - 1;
	} else if (name.find(ITEM_PARSE_THOROUGHLY_RUSTY, pos) == 0) {
		damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_RUSTY) - 1;
	} else if (name.find(ITEM_PARSE_VERY_BURNT, pos) == 0) {
		damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_BURNT) - 1;
	} else if (name.find(ITEM_PARSE_VERY_CORRODED, pos) == 0) {
		damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_CORRODED) - 1;
	} else if (name.find(ITEM_PARSE_VERY_ROTTED, pos) == 0) {
		damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_ROTTED) - 1;
	} else if (name.find(ITEM_PARSE_VERY_RUSTY, pos) == 0) {
		damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_RUSTY) - 1;
	} else if (name.find(ITEM_PARSE_BURNT, pos) == 0) {
		damage = 1;
		pos += sizeof (ITEM_PARSE_BURNT) - 1;
	} else if (name.find(ITEM_PARSE_CORRODED, pos) == 0) {
		damage = 1;
		pos += sizeof (ITEM_PARSE_CORRODED) - 1;
	} else if (name.find(ITEM_PARSE_ROTTED, pos) == 0) {
		damage = 1;
		pos += sizeof (ITEM_PARSE_ROTTED) - 1;
	} else if (name.find(ITEM_PARSE_RUSTY, pos) == 0) {
		damage = 1;
		pos += sizeof (ITEM_PARSE_RUSTY) - 1;
	} else {
		damage = 0;
	}
	/* enchantment */
	if (name[pos] == '+') {
		enchantment = name[pos + 1] - '0'; // assuming no item is enchanted beyond +9
		pos += 3;
	} else if (name[pos] == '-') {
		enchantment = 0 - (name[pos + 1] - '0'); // assuming no item is enchanted beyond -9
		pos += 3;
	}
	name.erase(0, pos);
	/* TODO: "named" & "called" */
	/* set "(being worn)" and so on in additional */
	if (name[name.size() - 1] == ')') {
		pos = name.rfind("(", name.size() - 1);
		if (pos != string::npos) {
			additional = name.substr(pos + 1, name.size() - pos - 2); // ditch "(" and ")"
			name.erase(pos - 1); // no need for this in name, and remove last space before "(" too
		}
	}
}

Item::Item() : name(""), count(0), buc_status(BUC_UNKNOWN), greased(false), fixed(false), damage(0), enchantment(0), additional("") {
}

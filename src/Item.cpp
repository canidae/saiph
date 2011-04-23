#include "Item.h"

#include <cstdio>
#include <stdlib.h>
#include <ostream>
#include "Globals.h"

/* parse text */
#define ITEM_CALLED " called "
#define ITEM_NAMED " named "
#define ITEM_PARSE_TEXT "%7s %127[^\t\n]"
#define ITEM_PARSE_BLESSED "blessed "
#define ITEM_PARSE_UNCURSED "uncursed "
#define ITEM_PARSE_CURSED "cursed "
#define ITEM_PARSE_GREASED "greased "
#define ITEM_PARSE_FIREPROOF "fireproof "
#define ITEM_PARSE_RUSTPROOF "rustproof "
#define ITEM_PARSE_BURNT "burnt "
#define ITEM_PARSE_VERY_BURNT "very burnt "
#define ITEM_PARSE_THOROUGHLY_BURNT "thoroughly burnt "
#define ITEM_PARSE_CORRODED "corroded "
#define ITEM_PARSE_VERY_CORRODED "very corroded "
#define ITEM_PARSE_THOROUGHLY_CORRODED "thoroughly corroded "
#define ITEM_PARSE_ROTTED "rotted "
#define ITEM_PARSE_VERY_ROTTED "very rotted "
#define ITEM_PARSE_THOROUGHLY_ROTTED "thoroughly rotted "
#define ITEM_PARSE_RUSTY "rusty "
#define ITEM_PARSE_VERY_RUSTY "very rusty "
#define ITEM_PARSE_THOROUGHLY_RUSTY "thoroughly rusty "

using namespace std;

/* constructors/destructor */
Item::Item(const string& text, int want) : _name(""), _count(0), _beatitude(BEATITUDE_UNKNOWN), _greased(false), _fixed(false), _damage(0), _unknown_enchantment(true), _enchantment(0), _additional(""), _want(want), _is_item(true) {
	/* parse text */
	char amount[8];
	char name_long[128];
	int matched = sscanf(text.c_str(), ITEM_PARSE_TEXT, amount, name_long);
	if (matched != 2) {
		_is_item = false;
		return; // unable to parse text as item
	}
	/* figure out amount of items */
	if ((amount[0] == 'a' && (amount[1] == '\0' || (amount[1] == 'n' && amount[2] == '\0'))) || ((amount[0] == 't' || amount[0] == 'T') && amount[1] == 'h' && amount[2] == 'e' && amount[3] == '\0')) {
		_count = 1; // "a", "an" or "the" <item>
	} else if (amount[0] >= '0' || amount[0] <= '9') {
		_count = atoi(amount); // n <items>
	} else {
		_is_item = false;
		return; // unable to parse text as item
	}
	string::size_type pos = 0;
	_name = name_long;
	/* buc */
	if (_name.find(ITEM_PARSE_BLESSED, pos) == pos) {
		_beatitude = BLESSED;
		pos += sizeof (ITEM_PARSE_BLESSED) - 1;
	} else if (_name.find(ITEM_PARSE_UNCURSED, pos) == pos) {
		_beatitude = UNCURSED;
		pos += sizeof (ITEM_PARSE_UNCURSED) - 1;
	} else if (_name.find(ITEM_PARSE_CURSED, pos) == pos) {
		_beatitude = CURSED;
		pos += sizeof (ITEM_PARSE_CURSED) - 1;
	} else {
		_beatitude = BEATITUDE_UNKNOWN;
	}
	/* greased */
	if (_name.find(ITEM_PARSE_GREASED, pos) == pos) {
		_greased = true;
		pos += sizeof (ITEM_PARSE_GREASED) - 1;
	} else {
		_greased = false;
	}
	/* fixed */
	if (_name.find(ITEM_PARSE_FIREPROOF, pos) == pos) {
		_fixed = true;
		pos += sizeof (ITEM_PARSE_FIREPROOF) - 1;
	} else if (_name.find(ITEM_PARSE_RUSTPROOF, pos) == pos) {
		_fixed = true;
		pos += sizeof (ITEM_PARSE_RUSTPROOF) - 1;
	} else {
		_fixed = false;
	}
	/* damage */
	if (_name.find(ITEM_PARSE_THOROUGHLY_RUSTY, pos) == pos) {
		if (_damage < 3)
			_damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_RUSTY) - 1;
	} else if (_name.find(ITEM_PARSE_VERY_RUSTY, pos) == pos) {
		if (_damage < 2)
			_damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_RUSTY) - 1;
	} else if (_name.find(ITEM_PARSE_RUSTY, pos) == pos) {
		if (_damage < 1)
			_damage = 1;
		pos += sizeof (ITEM_PARSE_RUSTY) - 1;
	}
	if (_name.find(ITEM_PARSE_THOROUGHLY_BURNT, pos) == pos) {
		if (_damage < 3)
			_damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_BURNT) - 1;
	} else if (_name.find(ITEM_PARSE_VERY_BURNT, pos) == pos) {
		if (_damage < 2)
			_damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_BURNT) - 1;
	} else if (_name.find(ITEM_PARSE_BURNT, pos) == pos) {
		if (_damage < 1)
			_damage = 1;
		pos += sizeof (ITEM_PARSE_BURNT) - 1;
	}
	if (_name.find(ITEM_PARSE_THOROUGHLY_CORRODED, pos) == pos) {
		if (_damage < 3)
			_damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_CORRODED) - 1;
	} else if (_name.find(ITEM_PARSE_VERY_CORRODED, pos) == pos) {
		if (_damage < 2)
			_damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_CORRODED) - 1;
	} else if (_name.find(ITEM_PARSE_CORRODED, pos) == pos) {
		if (_damage < 1)
			_damage = 1;
		pos += sizeof (ITEM_PARSE_CORRODED) - 1;
	}
	if (_name.find(ITEM_PARSE_THOROUGHLY_ROTTED, pos) == pos) {
		if (_damage < 3)
			_damage = 3;
		pos += sizeof (ITEM_PARSE_THOROUGHLY_ROTTED) - 1;
	} else if (_name.find(ITEM_PARSE_VERY_ROTTED, pos) == pos) {
		if (_damage < 2)
			_damage = 2;
		pos += sizeof (ITEM_PARSE_VERY_ROTTED) - 1;
	} else if (_name.find(ITEM_PARSE_ROTTED, pos) == pos) {
		if (_damage < 1)
			_damage = 1;
		pos += sizeof (ITEM_PARSE_ROTTED) - 1;
	}
	/* enchantment */
	if (_name[pos] == '+') {
		_unknown_enchantment = false;
		_enchantment = _name[pos + 1] - '0'; // assuming no item is enchanted beyond +9
		pos += 3;
		/* if we know enchantment, we (probably) know that it's uncursed unless it says otherwise */
		/* sadly certain items won't say "uncursed" after dropping them on an altar
		 * for example, the dagger valks start with is such an item.
		 * to prevent her from dropping it over & over again,
		 * we'll say items we know enchantment of, we also "know" is uncursed */
		if (_beatitude == BEATITUDE_UNKNOWN)
			_beatitude = UNCURSED;
	} else if (_name[pos] == '-') {
		_unknown_enchantment = false;
		_enchantment = 0 - (_name[pos + 1] - '0'); // assuming no item is enchanted beyond -9
		pos += 3;
		/* if we know enchantment, we (probably) know that it's uncursed unless it says otherwise */
		/* sadly certain items won't say "uncursed" after dropping them on an altar
		 * for example, the dagger valks start with is such an item.
		 * to prevent her from dropping it over & over again,
		 * we'll say items we know enchantment of, we also "know" is uncursed */
		if (_beatitude == BEATITUDE_UNKNOWN)
			_beatitude = UNCURSED;
	}
	/* erase up to pos as we've extracted all the info up to item name */
	_name.erase(0, pos);
	/* set "(being worn)" and so on in additional */
	if (_name[_name.size() - 1] == ')') {
		pos = _name.rfind("(", _name.size() - 1);
		if (pos != string::npos) {
			_additional = _name.substr(pos + 1, _name.size() - pos - 2); // ditch "(" and ")"
			_name.erase(pos - 1); // no need for this in name, and remove last space before "(" too
		}
	}
	/* if items are named something (else than "blessed", "uncursed" or "cursed"), replace name with that */
	pos = _name.rfind(ITEM_NAMED, _name.size() - 1);
	if (pos != string::npos) {
		string named = _name.substr(pos + sizeof (ITEM_NAMED) - 1);
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
			if (_beatitude == BEATITUDE_UNKNOWN)
				_beatitude = tmpbeatitude;
			named.clear();
		}
		/* if items are named something (else than "blessed", "uncursed" or "cursed"), replace name with that */
		if (!named.empty())
			_name = named;
		else
			_name.erase(pos);
	}
	/* if items are called something, replace name with that */
	pos = _name.rfind(ITEM_CALLED, _name.size() - 1);
	if (pos != string::npos)
		_name = _name.substr(pos + sizeof (ITEM_CALLED) - 1);
	/* singularize name.
	 * we'll only singularize stuff we care about for now */
	if (_name.find("pair of ") == 0) {
		if (_name.find("pair of lenses") == string::npos) {
			// boots or gloves; remove "pair of"
			_name = _name.erase(_name.find("pair of "), string("pair of ").length());
		}
		return; // don't de-pluralize "boots", "gloves" or "lenses"
	}
	string::size_type stop = string::npos;
	if ((stop = _name.find(" of ", 0)) != string::npos || (stop = _name.find(" labeled ", 0)) != string::npos || (stop = _name.find(" called ", 0)) != string::npos || (stop = _name.find(" named ", 0)) != string::npos || (stop = _name.find(" from ", 0)) != string::npos) {
		/* no need to do anything here.
		 * after this check "stop" should be placed just after the word that may be pluralized */
	}
	if (stop == string::npos)
		stop = _name.size();
	string::size_type start = _name.find_last_of(' ', stop - 1);
	if (start == string::npos)
		start = 0;
	else
		++start; // or we'll get the space before the word
	string word = _name.substr(start, stop - start);
	if (word == "leaves")
		word = "leaf";
	else if (word.size() >= 2 && word[word.size() - 1] == 's' && word[word.size() - 2] != 's')
		word.erase(word.size() - 1);
	_name.replace(start, stop, word);
}

Item::Item() : _name(""), _count(0), _beatitude(BEATITUDE_UNKNOWN), _greased(false), _fixed(false), _damage(0), _unknown_enchantment(true), _enchantment(0), _additional(""), _want(0), _is_item(false) {
}

/* methods */
const string& Item::name() const {
	return _name;
}

const string& Item::name(const string& name) {
	_name = name;
	return this->name();
}

int Item::count() const {
	return _count;
}

int Item::count(int count) {
	_count = count;
	return this->count();
}

int Item::beatitude() const {
	return _beatitude;
}

int Item::beatitude(int beatitude) {
	_beatitude = beatitude;
	return this->beatitude();
}

bool Item::greased() const {
	return _greased;
}

bool Item::greased(bool greased) {
	_greased = greased;
	return this->greased();
}

bool Item::fixed() const {
	return _fixed;
}

bool Item::fixed(bool fixed) {
	_fixed = fixed;
	return this->fixed();
}

int Item::damage() const {
	return _damage;
}

int Item::damage(int damage) {
	_damage = damage;
	return this->damage();
}

bool Item::unknownEnchantment() const {
	return _unknown_enchantment;
}

bool Item::unknownEnchantment(bool unknown_enchantment) {
	_unknown_enchantment = unknown_enchantment;
	return this->unknownEnchantment();
}

int Item::enchantment() const {
	return _enchantment;
}

int Item::enchantment(int enchantment) {
	_enchantment = enchantment;
	return this->enchantment();
}

const std::string& Item::additional() const {
	return _additional;
}

const std::string& Item::additional(const std::string& additional) {
	_additional = additional;
	return this->additional();
}

int Item::want() const {
	return _want;
}

int Item::want(int want) {
	_want = want;
	return this->want();
}

/* operator overloading */
bool Item::operator==(const Item& i) const {
	return _is_item == i._is_item && _count == i._count && _beatitude == i._beatitude && _greased == i._greased && _fixed == i._fixed && _damage == i._damage && _unknown_enchantment == i._unknown_enchantment && _enchantment == i._enchantment && _name == i._name && _additional == i._additional;
}

bool Item::operator!=(const Item& i) const {
	return !(*this == i);
}

ostream& operator<<(ostream& out, const Item& item) {
	if (item.name() == "") {
		out << "(no item)";
	} else {
		out << item.count() << " ";
		if (item.beatitude() == BLESSED)
			out << "blessed ";
		else if (item.beatitude() == UNCURSED)
			out << "uncursed ";
		else if (item.beatitude() == CURSED)
			out << "cursed ";
		if (item.greased())
			out << "greased ";
		if (item.fixed())
			out << "fixed ";
		if (item.damage() > 0) {
			if (item.damage() == 2)
				out << "very ";
			else if (item.damage() == 3)
				out << "thoroughly ";
			out << "damaged ";
		}
		if (!item.unknownEnchantment())
			out << ((item.enchantment() >= 0) ? "+" : "") << item.enchantment() << " ";
		out << item.name();
		if (item.additional() != "")
			out << " (" << item.additional() << ")";
	}
	return out;
}

#ifndef ITEM_H
#define ITEM_H
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

#include <string>

class Item {
	public:
		std::string name;
		int count;
		int beatitude;
		bool greased;
		bool fixed;
		int damage;
		bool unknown_enchantment;
		int enchantment;
		std::string additional;

		Item(const std::string &text);
		Item();
		bool operator==(const Item &other);
};
#endif

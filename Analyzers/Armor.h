#ifndef ARMOR_H
#define ARMOR_H

#define ARMOR_UNKNOWN_ENCHANTMENT_BONUS 3
/* various polymorph messages */
#define ARMOR_HAVE_NO_FEET "  You have no feet...  "
#define ARMOR_WONT_FIT_HORN " won't fit over your horn"
#define ARMOR_TOO_MANY_HOOVES "  You have too many hooves to wear "

#include <string>
#include <vector>
#include <map>
#include "../Item.h"
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Debug.h"

/* struct for wearing armor */
struct OldArmorData {
	int beatitude; // beatitudes we'll accept
	int priority; // wear armor with highest priority, priority += enchantment
	bool keep; // if we should keep this armor even if we find better armor
	std::string name;
};

/* struct for ranking a set of armor; much nicer than using a vector */
struct ArmorSet {
	Item shirt, suit, cloak, boots, gloves, helmet, shield;
	//allow looping over the armor types
	inline const Item & operator[](int index) const {
		switch(index) {
			case ARMOR_SHIRT:
				return shirt;
			case ARMOR_SUIT:
				return suit;
			case ARMOR_CLOAK:
				return cloak;
			case ARMOR_BOOTS:
				return boots;
			case ARMOR_GLOVES:
				return gloves;
			case ARMOR_HELMET:
				return helmet;
			case ARMOR_SHIELD:
				return shield;
			default:
				Debug::error() << "Armor] Using invalid ArmorSet index " << index << std::endl;
		}
	}
	inline Item & operator[](int index) {
		switch(index) {
			case ARMOR_SHIRT:
				return shirt;
			case ARMOR_SUIT:
				return suit;
			case ARMOR_CLOAK:
				return cloak;
			case ARMOR_BOOTS:
				return boots;
			case ARMOR_GLOVES:
				return gloves;
			case ARMOR_HELMET:
				return helmet;
			case ARMOR_SHIELD:
				return shield;
			default:
				Debug::error() << "Armor] Using invalid ArmorSet index " << index << std::endl;
		}
	}
	friend std::ostream & operator<<(std::ostream &out, const ArmorSet &as);
};

inline std::ostream & operator<<(std::ostream &out, const ArmorSet &as) {
	return out << "[" << as.shirt << ", " << as.suit << ", " << as.cloak << ", "
			<< as.boots << ", " << as.gloves << ", " << as.helmet << ", "
			<< as.shield << "]";
}

class Saiph;

class Armor : public Analyzer {
	public:
		Armor(Saiph *saiph);

		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		bool wear_armor;
		int carry_amount[ARMOR_SLOTS];
		bool can_wear[ARMOR_SLOTS];
		std::vector<OldArmorData> armor[ARMOR_SLOTS];
		std::string command2;
		Request req;
		int last_armor_type;
		bool last_polymorphed;
		static std::map<unsigned long long int, int> scores;

		bool isCursed(int armor_slot);
		void wearArmor();
		void resetCanWear();
		int rank(const ArmorSet &r);
};
#endif

#ifndef ANALYZER_ARMOR_H
#define ANALYZER_ARMOR_H

#include <string>
#include <vector>
#include <map>
#include "Analyzer.h"
#include "../Item.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Debug.h"

#define ARMOR_UNKNOWN_ENCHANTMENT_BONUS 3
/* various polymorph messages */
#define ARMOR_HAVE_NO_FEET "  You have no feet...  "
#define ARMOR_WONT_FIT_HORN " won't fit over your horn"
#define ARMOR_TOO_MANY_HOOVES "  You have too many hooves to wear "

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
			case SLOT_SHIRT:
				return shirt;
			case SLOT_SUIT:
				return suit;
			case SLOT_CLOAK:
				return cloak;
			case SLOT_BOOTS:
				return boots;
			case SLOT_GLOVES:
				return gloves;
			case SLOT_HELMET:
				return helmet;
			case SLOT_SHIELD:
				return shield;
			default:
				Debug::error() << "Armor] Using invalid ArmorSet index " << index << std::endl;
		}
	}
	inline Item & operator[](int index) {
		switch(index) {
			case SLOT_SHIRT:
				return shirt;
			case SLOT_SUIT:
				return suit;
			case SLOT_CLOAK:
				return cloak;
			case SLOT_BOOTS:
				return boots;
			case SLOT_GLOVES:
				return gloves;
			case SLOT_HELMET:
				return helmet;
			case SLOT_SHIELD:
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

namespace analyzer {
	class Armor : public Analyzer {
	public:
		Armor();

		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		bool wear_armor;
		int carry_amount[SLOTS];
		bool can_wear[SLOTS];
		std::vector<OldArmorData> armor[SLOTS];
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
}
#endif

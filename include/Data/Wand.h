#ifndef DATA_WAND_H
#define DATA_WAND_H

#include <map>
#include <string>
#include "Data/Item.h"

// non-directional doesn't ask for a direction
// beam asks for a direction but doesn't animate or reflect
// ray asks for a direction, animates, and can bounce or reflect
// unknown is used for random appearances
#define WAND_ZAP_TYPE_UNKNOWN 0
#define WAND_ZAP_TYPE_NONDIRECTIONAL 1
#define WAND_ZAP_TYPE_BEAM 2
#define WAND_ZAP_TYPE_RAY 3

#define WAND_COLD_MESSAGE "  A few ice cubes drop from the wand.  "
#define WAND_CREATE_MONSTER_MESSAGE " with a wand of create monster.  " // You write in the <dust/frost/etc> with a wand of create monster.
#define WAND_DIGGING_MESSAGE " is a wand of digging!  "
#define WAND_ENLIGHTENMENT_MESSAGE "  You feel self-knowledgeable...  "
#define WAND_FIRE_MESSAGE " is a wand of fire!  "
#define WAND_LIGHT_MESSAGE "  A lit field surrounds you!  "
#define WAND_LIGHTNING_MESSAGE " is a wand of lightning!  "
#define WAND_MAGIC_MISSILE_MESSAGE "  The floor is riddled by bullet holes!  "
#define WAND_POLYMORPH_MESSAGE "  The engraving now reads: " // followed by random rumor
#define WAND_SLEEP_DEATH_MESSAGE " stop moving!  " // The bugs on the <floor/ground/etc> stop moving!
#define WAND_SLOW_MONSTER_MESSAGE " slow down!  " // The bugs on the <floor/ground/etc> slow down!
#define WAND_SPEED_MONSTER_MESSAGE " speed up!  " // The bugs on the <floor/ground/etc> speed up!
#define WAND_STRIKING_MESSAGE "  The wand unsuccessfully fights your attempt to write!  "
#define WAND_VANISHER_MESSAGE " vanishes!  " // The engraving on the <floor/ground/etc> vanishes!
#define WAND_WISHING_MESSAGE "  You may wish for an object.  "

namespace data {

	class Wand : public Item {
	public:
		virtual ~Wand();

		static void init();
		static const std::map<const std::string, const Wand*>& wands();
		int maximumCharges() const;
		int zapType() const;
		const std::string& engraveMessage() const; // returns "" if no message

	protected:
		Wand(const std::string& name, int cost, int material, int maximum_charges, int zap_type, const std::string& engrave_message, unsigned long long properties);

		static void addToMap(const std::string& name, const Wand* wand);

	private:
		static std::map<const std::string, const Wand*> _wands;

		static void create(const std::string& name, int cost, int material, int maximum_charges, int zap_type, const std::string& engrave_message, unsigned long long properties);

		const int _maximum_charges;
		const int _zap_type;
		const std::string _engrave_message;
	};
}
#endif

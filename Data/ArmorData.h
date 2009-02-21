#ifndef _ARMORDATA_H
#define	_ARMORDATA_H

#include "ItemData.h"
#include "../Globals.h"

/* armor properties */
#define PROPERTY_MAGICRES
#define PROPERTY_REFLECTION
#define PROPERTY_FIRERES
#define PROPERTY_COLDRES
#define PROPERTY_SLEEPRES
#define PROPERTY_SHOCKRES
#define PROPERTY_POISONRES
#define PROPERTY_ACIDRES
#define PROPERTY_DISINTRES
#define PROPERTY_DISPLACEMENT
#define PROPERTY_INVISIBLE
#define PROPERTY_VISIBLE /* mummy wrapping */
#define PROPERTY_BRILLIANCE /* int+wis boost */
#define PROPERTY_ESP
#define PROPERTY_STUPIDITY /* dunce cap; might be useful vs. flayers */
#define PROPERTY_STRENGTH /* GoP */
#define PROPERTY_DEXTERITY /* GoD */
#define PROPERTY_FUMBLING
#define PROPERTY_JUMPING
#define PROPERTY_KICKING /* handy to know not to kick-test gray stones with this */
#define PROPERTY_LEVITATION
#define PROPERTY_STEALTH
#define PROPERTY_VERYFAST
#define PROPERTY_WATERWALKING
#define PROPERTY_SLIPPERY /* oilskin cloak */
//we've used 25 bits so far, 7 remain

class ArmorData : public ItemData {
public:
	ArmorData(const std::string& name, int cost, int weight, bool magic, int slot, int ac, int mc, unsigned int properties);
	/* TODO: after materials get implemented, hindersCasting() and erodes() */
	const int slot;
	const int ac;
	const int mc;
	const unsigned int properties;
};

#endif	/* _ARMORDATA_H */


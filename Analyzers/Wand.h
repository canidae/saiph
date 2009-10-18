#ifndef ANALYZER_WAND_H
#define ANALYZER_WAND_H

#include <string>
#include <vector>
#include "Analyzer.h"
#include "../Request.h"
#include "../Item.h"

#define WAND_DEBUG_NAME "Wand] "

/* these don't auto-id; we must actually engrave */
#define WAND_VANISHER_MESSAGE "  The engraving on the floor vanishes!  "
#define WAND_COLD_MESSAGE "  A few ice cubes drop from the wand.  "
#define WAND_SLEEP_DEATH_MESSAGE "  The bugs on the floor stop moving!  "
#define WAND_MAGIC_MISSILE_MESSAGE "  The floor is riddled by bullet holes!  "
#define WAND_POLYMORPH_MESSAGE "  The engraving now reads: " //followed by random rumor
#define WAND_SLOW_MONSTER_MESSAGE "  The bugs on the floor slow down!  "
#define WAND_SPEED_MONSTER_MESSAGE "  The bugs on the floor speed up!  "
#define WAND_STRIKING_MESSAGE "  The wand unsuccessfully fights your attempt to write!  "
/* these auto-id before the engrave prompt, so cancel the engraving */
#define WAND_DIGGING_MESSAGE " is a wand of digging!  "
#define WAND_LIGHTNING_MESSAGE " is a wand of lightning!  "
#define WAND_FIRE_MESSAGE " is a wand of fire!  "

#define WAND_WORN_OUT_MESSAGE "  You wrest one last charge from the worn-out wand.  "

#define WAND_VANISHER_NAME "wand of vanishing"
#define WAND_SLEEP_DEATH_NAME "wand of sleep/death"
#define WAND_NO_EFFECT_NAME "wand of no engrave effect"

#define WAND_STATE_INIT 0
#define WAND_STATE_DUST_X 1
#define WAND_STATE_WANTS_LOOK 2
#define WAND_STATE_CONFIRM_LOOK 6
#define WAND_STATE_ENGRAVING 3
#define WAND_STATE_READY_TO_NAME 4
#define WAND_STATE_WANT_DIRTY_INVENTORY 5

class Saiph;

namespace analyzer {
	class Wand : public Analyzer {
	public:
		Wand(Saiph* saiph);

		void analyze();
		void parseMessages(const std::string& messages);
	private:
		Saiph* saiph;
		Request req;
		unsigned char wand_key;
		int state;
		std::vector<std::string> wand_appearances;
		std::vector<std::pair<std::string, std::string> > wand_engrave_messages;
		void findUnidentifiedWands();
		bool isUnidentifiedWand(const Item& i);
		bool isUnidentifiedWand(const unsigned char& c);
	};
}
#endif

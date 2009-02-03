#ifndef WAND_H
#define WAND_H

#define WAND_VANISHER_MESSAGE "  The engraving on the floor vanishes!  "
#define WAND_COLD_MESSAGE "  A few ice cubes drop frm the wand.  "
#define WAND_SLEEP_DEATH_MESSAGE "  The bugs on the floor stop moving!  "
#define WAND_MAGIC_MISSILE_MESSAGE "  The floor is riddled by bullet holes!  "
#define WAND_POLYMORPH_MESSAGE "  The engraving now reads: " //followed by random rumor
#define WAND_SLOW_MONSTER_MESSAGE "  The bugs on the floor slow down!  "
#define WAND_SPEED_MONSTER_MESSAGE "  The bugs on the floor speed up!  "
#define WAND_STRIKING_MESSAGE "  The wand unsuccessfully fights your attempt to write.  "

#define WAND_VANISHER_NAME "wand of vanishing"
#define WAND_NO_EFFECT_NAME "wand of no engrave effect"

#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Request.h"
#include "../Item.h"

class Saiph;

class Wand : public Analyzer {
	public:
		Wand(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);
		void complete();
		void fail();
	private:
		Saiph *saiph;
		Request req;
		unsigned char wand_key;
		bool processing;
		std::vector<std::string> wand_appearances;
		void findUnidentifiedWands();
		bool isUnidentifiedWand(const Item& i);
};
#endif

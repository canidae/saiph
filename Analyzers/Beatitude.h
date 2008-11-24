#ifndef BEATITUDE_H
#define BEATITUDE_H
/* BEATITUDE_DROP_ALTAR_MIN says how many items we need at least before we
 * bother dropping them on an altar.
 * BEATITUDE_DROP_ALTAR_ADD_PER_1000_MOVE says how many additional items per
 * move we need before we bother heading towards an altar. we do this so that
 * she's a bit more reluctant to go a long way back to an altar */
#define BEATITUDE_DROP_ALTAR_MIN 1
#define BEATITUDE_DROP_ALTAR_ADD_PER_1000_MOVE 32
/* priorities */
#define BEATITUDE_DROP_ALTAR_PRIORITY 230

#include <string>
#include "../Analyzer.h"

class Item;
class Saiph;

class Beatitude : public Analyzer {
	public:
		Beatitude(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		bool check_beatitude;

		bool beatify(const Item &item);
};
#endif

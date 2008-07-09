#ifndef LOOTANALYZER_H
/* defines */
#define LOOTANALYZER_H
/* messages */
#define LO_THINGS_HERE "Things that are here:"
#define LO_YOU_SEE "You see here"
#define LO_PICK_UP "Pick up what?"
#define LO_LITTLE_LIFTING "You have a little trouble lifting"
#define LO_EXTREME_LIFTING "You have extreme difficulty lifting"
#define LO_MUCH_LIFTING "You have much trouble lifting"
/* actions */
#define LO_NOTHING 0
#define LO_LOOT 1
#define LO_SELECT_ALL 2
#define LO_NO_LOOT 3

/* forward declare */
class LootAnalyzer;

/* includes */
#include <string>
#include "../Saiph.h"

/* namespace */
using namespace std;

/* analyzes the map and finds somewhere to explore */
class LootAnalyzer : public Analyzer {
	public:
		/* constructors */
		LootAnalyzer(Saiph *saiph);

		/* methods */
		void analyze(int row, int col, unsigned char symbol);
		void command(string *command);
		int finish();
		int parseMessages(string *messages);

	private:
		/* variables */
		Saiph *saiph;
		int action;
};
#endif

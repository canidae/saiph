#ifndef LOOTANALYZER_H
/* defines */
#define LOOTANALYZER_H

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
		int parseMessages(string *messages);
		int analyze(int row, int col, char symbol);
		int finish();
		void command();

	private:
		/* variables */
		Saiph *saiph;
};
#endif

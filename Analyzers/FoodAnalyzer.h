#ifndef FOODANALYZER_H
/* defines */
#define FOODANALYZER_H
/* general */
#define FO_ROT_TIME 30
#define FO_ROTTEN_MONSTER "MZ" // monsters always leaving rotten corpses
/* actions */
#define FO_DO_NOTHING 0
#define FO_EAT_CORPSE 1

/* forward declare */
class FoodAnalyzer;

/* includes */
#include <list>
#include "../Saiph.h"

/* namespace */
using namespace std;

struct FO_Food {
	int row;
	int col;
	int rots_turn;
};

/* monitors health */
class FoodAnalyzer : public Analyzer {
	public:
		/* constructors */
		FoodAnalyzer(Saiph *saiph);

		/* methods */
		int analyze(int row, int col, unsigned char symbol);
		void command(string *command);
		int finish();
		int parseMessages(string *messages);

	private:
		/* variables */
		Saiph *saiph;
		list<FO_Food> food;
		list<string> inedible;
		int action;
		unsigned char action_move;
};
#endif

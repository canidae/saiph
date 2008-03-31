#ifndef FIGHTANALYZER_H
/* defines */
#define FIGHTANALYZER_H
/* various */
#define FI_MAX_MONSTERS 64

/* forward declare */
class FightAnalyzer;

/* includes */
#include "../Saiph.h"

/* namespace */
using namespace std;

/* keeping track of visible monsters */
struct FI_Monster {
	int row;
	int col;
	int threat;
	bool no_melee;
};

/* analyzer for fighting monsters */
class FightAnalyzer : public Analyzer {
	public:
		/* constructors */
		FightAnalyzer(Saiph *saiph);

		/* methods */
		virtual void analyze(int row, int col, char symbol);
		virtual void finish();

	private:
		/* variables */
		FI_Monster monsters[FI_MAX_MONSTERS];
		int monster_count;
		Saiph *saiph;
};
#endif

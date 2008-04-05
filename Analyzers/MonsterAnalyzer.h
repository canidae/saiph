#ifndef MONSTERANALYZER_H
/* defines */
#define MONSTERANALYZER_H
/* various */
#define MO_MAX_MONSTERS 64

/* forward declare */
class MonsterAnalyzer;

/* includes */
#include "../Saiph.h"
#include "HealthAnalyzer.h" // needed for HA_ENGRAVE_ELBERETH

/* namespace */
using namespace std;

/* keeping track of visible monsters */
struct MO_Monster {
	int row;
	int col;
	int threat;
	bool no_melee;
};

/* analyzer for fighting monsters */
class MonsterAnalyzer : public Analyzer {
	public:
		/* constructors */
		MonsterAnalyzer(Saiph *saiph);

		/* methods */
		virtual void analyze(int row, int col, char symbol);
		virtual void finish();

	private:
		/* variables */
		MO_Monster monsters[MO_MAX_MONSTERS];
		int monster_count;
		Saiph *saiph;
};
#endif

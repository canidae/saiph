#ifndef MONSTERANALYZER_H
/* defines */
#define MONSTERANALYZER_H
/* various */
#define MO_MAX_MONSTERS 64

/* forward declare */
class MonsterAnalyzer;

/* includes */
#include "../Saiph.h"

/* namespace */
using namespace std;

/* keeping track of visible monsters */
struct MO_Monster {
	int row;
	int col;
	int last_seen;
	char symbol;
};

/* analyzer for fighting monsters */
class MonsterAnalyzer : public Analyzer {
	public:
		/* constructors */
		MonsterAnalyzer(Saiph *saiph);

		/* methods */
		int analyze(int row, int col, char symbol);
		int finish();
		void command();

	private:
		/* variables */
		MO_Monster monsters[MO_MAX_MONSTERS];
		Saiph *saiph;
};
#endif

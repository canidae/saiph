#ifndef FIGHTANALYZER_H
#define FIGHTANALYZER_H

/* forward declare */
class FightAnalyzer;

/* includes */
#include "../Analyzer.h"
#include "../Saiph.h"

/* defines */
#define FA_MAX_MONSTERS 64

using namespace std;

struct Monster {
	int row;
	int col;
	int threat;
	bool no_melee;
};

class FightAnalyzer : public Analyzer {
	public:
		/* constructors */
		FightAnalyzer(Saiph *saiph);

		/* destructors */
		~FightAnalyzer();

		/* methods */
		virtual void analyze(int row, int col, char symbol) const;
		virtual void finish() const;

	private:
		/* variables */
		Monster monsters[FA_MAX_MONSTERS];
		int monster_count;
		Saiph *saiph;
};

#endif

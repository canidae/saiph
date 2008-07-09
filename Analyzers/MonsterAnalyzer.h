#ifndef MONSTERANALYZER_H
/* defines */
#define MONSTERANALYZER_H
/* various */
#define MO_MAX_MONSTERS 64
/* messages */
#define MO_REALLY_ATTACK "Really attack"

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
	unsigned char symbol;
};

/* analyzer for fighting monsters */
class MonsterAnalyzer : public Analyzer {
	public:
		/* constructors */
		MonsterAnalyzer(Saiph *saiph);

		/* methods */
		void analyze(int row, int col, unsigned char symbol);
		void command(string *command);
		int finish();
		int parseMessages(string *messages);

	private:
		/* variables */
		Saiph *saiph;
		MO_Monster monsters[MO_MAX_MONSTERS];
		unsigned char action;
};
#endif

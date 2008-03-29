#ifndef DOORANALYZER_H
#define DOORANALYZER_H

/* forward declare */
class DoorAnalyzer;

/* includes */
#include "../Analyzer.h"
#include "../Saiph.h"

/* defines */
#define DA_NO_DOOR 0
#define DA_DOOR_OPEN 1
#define DA_DOOR_CLOSED 2

using namespace std;

class DoorAnalyzer : public Analyzer {
	public:
		/* constructors */
		DoorAnalyzer(Saiph *saiph);

		/* destructors */
		~DoorAnalyzer();

		/* methods */
		virtual void analyze(int row, int col, char symbol);
		virtual void finish();

	private:
		/* variables */
		Saiph *saiph;
};

#endif

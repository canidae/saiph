#ifndef LEVEL_H
/* defines */
#define LEVEL_H
/* various */
#define LEVEL_DESCEND_PRIORITY 40

/* forward declare */
class Level;

/* includes */
#include <string>
#include "../Coordinate.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* class for ascending/descending */
class Level : public Analyzer {
	public:
		/* constructors */
		Level(Saiph *saiph);

		/* methods */
		void complete();
		void finish();
		void inspect(const Point &point);

	private:
		/* variables */
		Saiph *saiph;
		vector<Coordinate> stairs; // TODO: this must be more advanced in the future 
};
#endif

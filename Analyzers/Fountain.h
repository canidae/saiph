#ifndef FOUNTAIN_H
/* defines */
#define FOUNTAIN_H

/* forward declare */
class Fountain;

/* includes */
#include <map>
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Point.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* locates and store fountains */
class Fountain : public Analyzer {
	public:
		/* constructors */
		Fountain(Saiph *saiph);

		/* methods */
		void command(string *command);
		void finish();
		void inspect(const Point &point);
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		map<int, map<int, map<Point, bool> > > fountains;
		Coordinate target;
		int target_priority;
		string action;
		Request req;
};
#endif

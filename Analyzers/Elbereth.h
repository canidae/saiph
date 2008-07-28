#ifndef ELBERETH_H
/* defines */
#define ELBERETH_H

/* forward declare */
class Elbereth;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Elbereth : public Analyzer {
	public:
		/* constructors */
		Elbereth(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		int elbereth_count;
		bool burned;
		bool digged;
		bool dusted;
		bool did_look;
		bool append;
};
#endif

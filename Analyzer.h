#ifndef ANALYZER_H
/* defines */
#define ANALYZER_H
/* various ambigous messages */
#define CHOOSE_DIRECTION "In what direction?"

/* forward declare */
class Analyzer;

/* includes */
#include <string>
#include "Point.h"

/* namespace */
using namespace std;

/* analyze every tile of type and/or do whatever you want in finish() */
class Analyzer {
	public:
		/* methods */
		virtual void command(string *command);
		virtual int finish();
		virtual void inspect(const Point &point, unsigned char symbol);
		virtual int parseMessages(string *messages);
		virtual bool requestAction(int request);
		virtual int start();
};
#endif

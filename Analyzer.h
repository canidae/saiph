#ifndef ANALYZER_H
/* defines */
#define ANALYZER_H

/* forward declare */
class Analyzer;

/* includes */
#include <string>
#include "Point.h"
#include "Request.h"

/* namespace */
using namespace std;

/* analyze every tile of type and/or do whatever you want in finish() */
class Analyzer {
	public:
		/* destructor */
		virtual ~Analyzer();

		/* methods */
		virtual void command(string *command);
		virtual int finish();
		virtual void inspect(const Point &point);
		virtual int parseMessages(string *messages);
		virtual bool requestAction(const Request &request);
		virtual int start();
};
#endif

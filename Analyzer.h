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
		/* variables */
		string name;
		int priority;
	
		/* constructor */
		Analyzer(string name);

		/* destructor */
		virtual ~Analyzer();

		/* methods */
		virtual void command(string *command);
		virtual void finish();
		virtual void inspect(const Point &point);
		virtual void parseMessages(const string &messages);
		virtual bool request(const Request &request);
		virtual void start();
};
#endif

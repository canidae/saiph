#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>

class Point;
class Request;

class Analyzer {
	public:
		/* variables */
		std::string name;
		int priority;
		std::string command;
	
		/* constructor */
		Analyzer(std::string name);

		/* destructor */
		virtual ~Analyzer();

		/* methods */
		virtual void analyze();
		virtual void complete();
		virtual void fail();
		virtual void init();
		virtual void inspect(const Point &point);
		virtual void parseMessages(const std::string &messages);
		virtual bool request(const Request &request);
};
#endif

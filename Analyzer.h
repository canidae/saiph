#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>

class Point;
class Request;

class Analyzer {
	public:
		std::string name;
		int priority;
		std::string command;
		bool expired;
	
		Analyzer(std::string name);
		virtual ~Analyzer();

		virtual void analyze(const std::string &messages);
		virtual void complete();
		virtual void fail();
		virtual void init();
		virtual bool request(const Request &request);
};
#endif

#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>

#define ILLEGAL_ANALYZER_ID -1

namespace action {
	class Action;
}
namespace event {
	class Event;
}
class Command;

namespace analyzer {
	class Analyzer {
	public:
		std::string name;
		action::Action *action;

		Analyzer(std::string name) : name(name), action(NULL) {}
		virtual ~Analyzer();

		virtual void analyze(const Command &) {}
		virtual int getID() {return ILLEGAL_ANALYZER_ID;}
		virtual void handle(event::Event *const) {}
		virtual void init() {}
		virtual void parseMessages(const std::string &, const Command &) {}
	};
}
#endif

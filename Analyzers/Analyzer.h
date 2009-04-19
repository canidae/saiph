#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>

namespace action {
	class Action;
}
namespace event {
	class Event;
}

namespace analyzer {
	class Analyzer {
	public:
		std::string name;
		action::Action *action;

		Analyzer(std::string name) : name(name), action(NULL) {}
		virtual ~Analyzer() {}

		virtual void init() {}
		virtual void parseMessages(const std::string &) {}
		virtual void analyze() {}
		virtual void onEvent(event::Event *const) {}
	};
}
#endif

#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include "../Actions/Action.h"

namespace event {
	class Event;
}

namespace analyzer {
	class Analyzer {
	public:
		std::string name;

		Analyzer(std::string name) : name(name), action(NULL) {}
		virtual ~Analyzer() {delete action;}

		virtual void init() {}
		virtual void parseMessages(const std::string &) {}
		virtual void analyze() {}
		virtual void onEvent(event::Event *const) {}
		action::Action *getAction() {return action;}
		void setAction(action::Action *action);

	private:
		action::Action *action;
	};
}

/* inline methods */
inline void analyzer::Analyzer::setAction(action::Action *action) {
	delete this->action;
	this->action = action;
}
#endif

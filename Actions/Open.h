#ifndef ACTION_OPEN_H
#define ACTION_OPEN_H

#include "Action.h"
#include "../World.h"

namespace action {
	class Open : public Action {
	public:
		static int id;

		Open(analyzer::Analyzer *analyzer, unsigned char direction, int priority) : Action(analyzer), open("o", priority), open_direction(std::string(1, direction), PRIORITY_CONTINUE_ACTION) {}
		virtual ~Open() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command open;
		const Command open_direction;
	};
}

/* methods */
inline const Command &action::Open::getCommand() {
	switch (sequence) {
	case 0:
		return open;

	case 1:
		return open_direction;

	default:
		return Action::noop;
	}
}

inline void action::Open::updateAction(const std::string &messages) {
	if (World::question && messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos)
		sequence = 1;
	else if (sequence == 1)
		sequence = 2;
}
#endif

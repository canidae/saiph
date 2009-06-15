#ifndef KICK_H
#define KICK_H

#include "Action.h"
#include "../World.h"

namespace action {
	class Kick : public Action {
	public:
		static int id;

		Kick(analyzer::Analyzer *analyzer, unsigned char direction, int priority) : Action(analyzer), kick("", priority), kick_direction(std::string(1, direction), PRIORITY_CONTINUE_ACTION) {}
		virtual ~Kick() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command kick;
		const Command kick_direction;
	};
}

/* methods */
inline const Command &action::Kick::getCommand() {
	switch (sequence) {
	case 0: 
		return kick;

	case 1: 
		return kick_direction;

	default:
		return Action::noop;
	}
}

inline void action::Kick::updateAction(const std::string &messages) {
	if (World::question && messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos)
		sequence = 1;
	else if (sequence == 1)
		sequence = 2;
}
#endif

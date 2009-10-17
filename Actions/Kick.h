#ifndef ACTION_KICK_H
#define ACTION_KICK_H

#include "Action.h"

namespace action {

	class Kick : public Action {
	public:
		static int id;

		Kick(analyzer::Analyzer *analyzer, unsigned char direction, int priority) : Action(analyzer), kick("", priority), kick_direction(std::string(1, direction), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Kick() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command kick;
		const Command kick_direction;
	};

	inline const Command &action::Kick::command() {
		switch (sequence) {
		case 0:
			return kick;

		case 1:
			return kick_direction;

		default:
			return Action::NOOP;
		}
	}

	inline void action::Kick::update(const std::string &messages) {
		if (messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos)
			sequence = 1;
		else if (sequence == 1)
			sequence = 2;
	}
}
#endif

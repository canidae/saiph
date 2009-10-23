#ifndef ACTION_KICK_H
#define ACTION_KICK_H

#include "Action.h"

namespace action {

	class Kick : public Action {
	public:
		static const int ID;

		Kick(analyzer::Analyzer* analyzer, const unsigned char& direction, const int& priority) : Action(analyzer), _kick("", priority), _kick_direction(std::string(1, direction), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Kick() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _kick;

			case 1:
				return _kick_direction;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos)
				_sequence = 1;
			else if (_sequence == 1)
				_sequence = 2;
		}

	private:
		const Command _kick;
		const Command _kick_direction;
	};
}
#endif

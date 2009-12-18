#ifndef ACTION_ENGRAVE_H
#define ACTION_ENGRAVE_H

#include "Action.h"

// TODO: (can't engrave)
#define MESSAGE_YOU_CANT_HOLD "  You can't even hold anything!  "

namespace action {

	class Engrave : public Action {
	public:
		static const int ID;

		Engrave(analyzer::Analyzer* analyzer, const std::string& engrave_msg, unsigned char engrave_with, int priority, bool should_append = true) : Action(analyzer), _engrave("E", priority), _message(engrave_msg, PRIORITY_CONTINUE_ACTION), _item(engrave_with, PRIORITY_CONTINUE_ACTION), _append(should_append ? YES : NO, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Engrave() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {

			case 0:
				return _engrave;

			case 1:
				return _item;

			case 2:
				return _append;

			case 3:
				return _message;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_ENGRAVE_WITH) != std::string::npos)
				_sequence = 1;
			else if (messages.find(MESSAGE_ENGRAVE_ADD) != std::string::npos)
				_sequence = 2;
			else if (_sequence < 3)
				_sequence = 3;
			else if (_sequence == 3)
				_sequence = 4;
		}

	private:
		const Command _engrave;
		const Command _message;
		const Command _item;
		const Command _append;
	};
}
#endif

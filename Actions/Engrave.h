#ifndef ACTION_ENGRAVE_H
#define ACTION_ENGRAVE_H

#include "Action.h"

namespace action {

	class Engrave : public Action {
	public:
		static const int ID;

		Engrave(analyzer::Analyzer* analyzer, const std::string& engrave_msg, const unsigned char& engrave_with, const int& priority, const bool& should_append = true) : Action(analyzer), _engrave(std::string(1, 'E'), priority), _message(engrave_msg, PRIORITY_CONTINUE_ACTION), _item(std::string(1, engrave_with), PRIORITY_CONTINUE_ACTION), _append(std::string(1, should_append ? 'y' : 'n'), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Engrave() {
		}

		virtual const int& id() {
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
			else if (_sequence == 3 || _sequence == -1)
				_sequence = -1;
			else
				_sequence = 3;
		};

	private:
		const Command _engrave;
		const Command _message;
		const Command _item;
		const Command _append;
	};
}
#endif

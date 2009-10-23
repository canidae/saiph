#ifndef ACTION_OPEN_H
#define ACTION_OPEN_H

#include "Action.h"

namespace action {

	class Open : public Action {
	public:
		static const int ID;

		Open(analyzer::Analyzer* analyzer, const unsigned char& direction, const int& priority) : Action(analyzer), _open("o", priority), _open_direction(std::string(1, direction), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Open() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _open;

			case 1:
				return _open_direction;

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
		const Command _open;
		const Command _open_direction;
	};
}
#endif

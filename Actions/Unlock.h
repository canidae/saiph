#ifndef ACTION_UNLOCK_H
#define ACTION_UNLOCK_H

#include "Action.h"

namespace action {

	class Unlock : public Action {
	public:
		static const int ID;

		Unlock(analyzer::Analyzer* analyzer, unsigned char key, unsigned char direction, int priority) : Action(analyzer), _do_apply("a", priority), _apply_item(std::string(1, key), PRIORITY_CONTINUE_ACTION), _apply_direction(std::string(1, direction), PRIORITY_CONTINUE_ACTION), _do_unlock("y", PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Unlock() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_apply;

			case 1:
				return _apply_item;

			case 2:
				return _apply_direction;

			case 3:
				return _do_unlock;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_WHAT_TO_APPLY) != std::string::npos) {
				_sequence = 1;
			} else if (messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos) {
				_sequence = 2;
			} else if (messages.find(MESSAGE_UNLOCK_IT) != std::string::npos) {
				_sequence = 3;
			} else if (_sequence == 3) {
				_sequence = 4;
			}
		}

	private:
		const Command _do_apply;
		const Command _apply_item;
		const Command _apply_direction;
		const Command _do_unlock;
	};
}
#endif

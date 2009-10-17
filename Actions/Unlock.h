#ifndef ACTION_UNLOCK_H
#define ACTION_UNLOCK_H

#include "Action.h"

namespace action {

	class Unlock : public Action {
	public:
		static int id;

		Unlock(analyzer::Analyzer *analyzer, unsigned char key, unsigned char direction, int priority) : Action(analyzer), do_apply("a", priority), apply_item(std::string(1, key), PRIORITY_CONTINUE_ACTION), apply_direction(std::string(1, direction), PRIORITY_CONTINUE_ACTION), do_unlock("y", PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Unlock() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command do_apply;
		const Command apply_item;
		const Command apply_direction;
		const Command do_unlock;
	};

	inline const Command &action::Unlock::getCommand() {
		switch (sequence) {
		case 0:
			return do_apply;

		case 1:
			return apply_item;

		case 2:
			return apply_direction;

		case 3:
			return do_unlock;

		default:
			return Action::noop;
		}
	}

	inline void action::Unlock::updateAction(const std::string &messages) {
		if (messages.find(MESSAGE_WHAT_TO_APPLY) != std::string::npos) {
			sequence = 1;
		} else if (messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos) {
			sequence = 2;
		} else if (messages.find(MESSAGE_UNLOCK_IT) != std::string::npos) {
			sequence = 3;
		} else if (sequence == 3) {
			sequence = 4;
		}
	}
}
#endif

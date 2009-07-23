#ifndef ACTION_CALL_H
#define ACTION_CALL_H

#include "Action.h"
#include "../Inventory.h"

namespace action {
	class Call : public Action {
	public:
		static int id;

		Call(analyzer::Analyzer *analyzer, unsigned char item, std::string call) : Action(analyzer), do_call("#call", PRIORITY_LOOK), call_individual("n", PRIORITY_CONTINUE_ACTION), call_item(std::string(1, item), PRIORITY_CONTINUE_ACTION), set_call(call, PRIORITY_CONTINUE_ACTION) {}
		virtual ~Call() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command do_call;
		const Command call_individual;
		const Command call_item;
		const Command set_call;
	};

	inline const Command &Call::getCommand() {
		switch (sequence) {
		case 0:
			return do_call;

		case 1:
			return call_individual;

		case 2:
			return call_item;

		case 3:
			return set_call;

		default:
			return Action::noop;
		}
	}

	inline void Call::updateAction(const std::string &messages) {
		if (messages.find(MESSAGE_NAME_INDIVIDUAL_OBECT) != std::string::npos) {
			sequence = 1;
		} else if (messages.find(MESSAGE_ITEM_TO_CALL) != std::string::npos) {
			sequence = 2;
		} else if (messages.find(MESSAGE_WHAT_TO_CALL_ITEM) != std::string::npos) {
			sequence = 3;
		} else {
			/* we need to mark inventory as not updated */
			Inventory::updated = false;
			sequence = 4;
		}
	}
}
#endif

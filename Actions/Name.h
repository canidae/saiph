#ifndef ACTION_NAME_H
#define ACTION_NAME_H

#include "Action.h"
#include "../Inventory.h"

#define MESSAGE_ITEM_TO_NAME "  What do you want to name? "
#define MESSAGE_WHAT_TO_NAME_ITEM "  What do you want to name this "

namespace action {
	class Name : public Action {
	public:
		static int id;

		Name(analyzer::Analyzer *analyzer, unsigned char item, std::string name) : Action(analyzer), do_name("#name", PRIORITY_LOOK), name_individual("y", PRIORITY_CONTINUE_ACTION), name_item(std::string(1, item), PRIORITY_CONTINUE_ACTION), set_name(name, PRIORITY_CONTINUE_ACTION) {}
		virtual ~Name() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command do_name;
		const Command name_individual;
		const Command name_item;
		const Command set_name;
	};

	inline const Command &Name::getCommand() {
		switch (sequence) {
		case 0:
			return do_name;

		case 1:
			return name_individual;

		case 2:
			return name_item;

		case 3:
			return set_name;

		default:
			return Action::noop;
		}
	}

	inline void Name::updateAction(const std::string &messages) {
		if (messages.find(MESSAGE_NAME_INDIVIDUAL_OBECT) != std::string::npos) {
			sequence = 1;
		} else if (messages.find(MESSAGE_ITEM_TO_NAME) != std::string::npos) {
			sequence = 2;
		} else if (messages.find(MESSAGE_WHAT_TO_NAME_ITEM) != std::string::npos) {
			sequence = 3;
		} else {
			/* we need to mark inventory as not updated */
			Inventory::updated = false;
			sequence = 4;
		}
	}
}
#endif

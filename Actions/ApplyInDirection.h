#ifndef ACTION_APPLY_IN_DIRECTION_H
#define ACTION_APPLY_IN_DIRECTION_H

#include "Action.h"
#include "../Inventory.h"
#include "../World.h"

namespace action {
	class ApplyInDirection : public Action {
	public:
		static int id;

		ApplyInDirection(analyzer::Analyzer *analyzer, unsigned char key, unsigned char direction, int priority, bool update_inventory = true) : Action(analyzer), do_apply("a", priority), apply_item(std::string(1, key), PRIORITY_CONTINUE_ACTION), apply_direction(std::string(1, direction), PRIORITY_CONTINUE_ACTION), update_inventory(update_inventory) {}
		virtual ~ApplyInDirection() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command do_apply;
		const Command apply_item;
		const Command apply_direction;
		const bool update_inventory;
	};
}

/* methods */
inline const Command &action::ApplyInDirection::getCommand() {
	switch (sequence) {
	case 0: 
		return do_apply;

	case 1: 
		return apply_item;

	case 2: 
		return apply_direction;

	default:
		return Action::noop;
	}
}

inline void action::ApplyInDirection::updateAction(const std::string &messages) {
	if (World::question && messages.find(MESSAGE_WHAT_TO_APPLY) != std::string::npos) {
		sequence = 1;
	} else if (World::question && messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos) {
		sequence = 2;
	} else if (sequence == 2) {
		/* mark inventory dirty if update_inventory is true */
		if (update_inventory)
			Inventory::updated = false;
		sequence = 3;
	}
}
#endif

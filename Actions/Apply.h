#ifndef ACTION_APPLY_H
#define ACTION_APPLY_H

#include "Action.h"
#include "../Inventory.h"

namespace action {

	class Apply : public Action {
	public:
		static int id;

		Apply(analyzer::Analyzer *analyzer, unsigned char key, int priority, bool update_inventory = true) : Action(analyzer), do_apply("a", priority), apply_item(std::string(1, key), PRIORITY_CONTINUE_ACTION), update_inventory(update_inventory) {
		}

		virtual ~Apply() {
		}

		virtual int getID() {
			return id;
		}
		virtual const Command &command();
		virtual void update(const std::string &messages);

	private:
		const Command do_apply;
		const Command apply_item;
		const bool update_inventory;
	};

	inline const Command &action::Apply::command() {
		switch (sequence) {
		case 0:
			return do_apply;

		case 1:
			return apply_item;

		default:
			return Action::noop;
		}
	}

	inline void action::Apply::update(const std::string &messages) {
		if (messages.find(MESSAGE_WHAT_TO_APPLY) != std::string::npos) {
			sequence = 1;
		} else if (sequence == 1) {
			/* mark inventory dirty if update_inventory is true */
			if (update_inventory)
				Inventory::updated = false;
			sequence = 2;
		}
	}
}
#endif

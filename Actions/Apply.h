#ifndef ACTION_APPLY_H
#define ACTION_APPLY_H

#include "Action.h"
#include "../Inventory.h"

namespace action {

	class Apply : public Action {
	public:
		static const int ID;

		Apply(analyzer::Analyzer* analyzer, const unsigned char& key, const int& priority, const bool& update_inventory = true) : Action(analyzer), _do_apply("a", priority), _apply_item(key, PRIORITY_CONTINUE_ACTION), _update_inventory(update_inventory) {
		}

		virtual ~Apply() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_apply;

			case 1:
				return _apply_item;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_WHAT_TO_APPLY) != std::string::npos) {
				_sequence = 1;
			} else if (_sequence == 1) {
				/* mark inventory dirty if _update_inventory is true */
				if (_update_inventory)
					Inventory::updated(false);
				_sequence = 2;
			}
		}

	private:
		const Command _do_apply;
		const Command _apply_item;
		const bool _update_inventory;
	};
}
#endif

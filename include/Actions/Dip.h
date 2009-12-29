#ifndef ACTION_DIP_H
#define ACTION_DIP_H

#include "Inventory.h"
#include "Actions/Action.h"

namespace action {

	class Dip : public Action {
	public:
		static const int ID;

		Dip(analyzer::Analyzer* analyzer, unsigned char key, unsigned char dip_into, const int& priority, bool update_inventory = true) : Action(analyzer), _do_dip("#dip", priority), _dip_item(key, PRIORITY_CONTINUE_ACTION), _dip_in_water(dip_into == ILLEGAL_ITEM ? 'y' : 'n', PRIORITY_CONTINUE_ACTION), _dip_in_item(dip_into, PRIORITY_CONTINUE_ACTION), _update_inventory(update_inventory) {
		}

		virtual ~Dip() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_dip;

			case 1:
				return _dip_item;

			case 2:
				return _dip_in_water;

			case 3:
				return _dip_in_item;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_WHAT_TO_DIP) != std::string::npos) {
				_sequence = 1;
			} else if (messages.find(MESSAGE_DIP_IN_FOUNTAIN) != std::string::npos || messages.find(MESSAGE_DIP_IN_WATER) != std::string::npos) {
				_sequence = 2;
			} else if (messages.find(MESSAGE_WHAT_TO_DIP_INTO) != std::string::npos) {
				_sequence = 3;
			} else if (_sequence == 2 || _sequence == 3) {
				/* mark inventory dirty if _update_inventory is true */
				if (_update_inventory)
					Inventory::update();
				_sequence = 4;
			}
		}

	private:
		const Command _do_dip;
		const Command _dip_item;
		const Command _dip_in_water;
		const Command _dip_in_item;
		const bool _update_inventory;
	};
}
#endif

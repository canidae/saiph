#ifndef ACTION_THROW_H
#define ACTION_THROW_H

#include "Action.h"
#include "../Inventory.h"

namespace action {

	class Throw : public Action {
	public:
		static const int ID;

		Throw(analyzer::Analyzer* analyzer, const unsigned char& key, const unsigned char& direction, const int& priority) : Action(analyzer), _do_throw("t", priority), _throw_item(key, PRIORITY_CONTINUE_ACTION), _throw_direction(direction, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Throw() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _do_throw;

			case 1:
				return _throw_item;

			case 2:
				return _throw_direction;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_WHAT_TO_THROW) != std::string::npos) {
				_sequence = 1;
			} else if (messages.find(MESSAGE_IN_WHAT_DIRECTION) != std::string::npos) {
				_sequence = 2;
			} else if (_sequence == 2) {
				/* mark inventory dirty */
				Inventory::update();
				_sequence = 3;
			}
		}

	private:
		const Command _do_throw;
		const Command _throw_item;
		const Command _throw_direction;
	};
}
#endif

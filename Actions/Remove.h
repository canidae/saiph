#ifndef ACTION_REMOVE_H
#define ACTION_REMOVE_H

#include "Action.h"
#include "../Inventory.h"

namespace action {

	class Remove : public Action {
	public:
		static const int ID;

		Remove(analyzer::Analyzer *analyzer, unsigned char key, int priority) : Action(analyzer), _remove("R", priority), _remove_key(std::string(1, key), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Remove() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command &command() {
			switch (_sequence) {
			case 0:
				return _remove;

			case 1:
				return _remove_key;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string &messages) {
			if (World::question && messages.find(MESSAGE_WHAT_TO_REMOVE) != std::string::npos) {
				_sequence = 1;
			} else if (_sequence == 1) {
				/* also mark the inventory dirty when we do this */
				Inventory::updated = false;
				_sequence = 2;
			}
		}

	private:
		const Command _remove;
		const Command _remove_key;
	};
}
#endif

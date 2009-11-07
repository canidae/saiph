#ifndef ACTION_WEAR_H
#define	ACTION_WEAR_H

#include "Action.h"
#include "../Inventory.h"

namespace action {

	class Wear : public Action {
	public:
		static const int ID;

		Wear(analyzer::Analyzer* analyzer, const unsigned char& key, const int& priority) : Action(analyzer), _wear("W", priority), _wear_key(std::string(1, key), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Wear() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _wear;

			case 1:
				return _wear_key;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (World::question() && messages.find(MESSAGE_WHAT_TO_wear) != std::string::npos) {
				_sequence = 1;
			} else if (_sequence == 1) {
				/* also mark the inventory dirty when we do this */
				Inventory::updated(false);
				_sequence = 2;
			}
		}

	private:
		const Command _wear;
		const Command _wear_key;
	};
}
#endif

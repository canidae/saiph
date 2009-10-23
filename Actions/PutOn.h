#ifndef ACTION_PUT_ON_H
#define ACTION_PUT_ON_H

#include "Action.h"
#include "../Inventory.h"

namespace action {

	class PutOn : public Action {
	public:
		static const int ID;

		PutOn(analyzer::Analyzer* analyzer, unsigned char key, int priority) : Action(analyzer), _put_on("P", priority), _put_on_key(std::string(1, key), PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~PutOn() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _put_on;

			case 1:
				return _put_on_key;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (World::question() && messages.find(MESSAGE_WHAT_TO_PUT_ON) != std::string::npos) {
				_sequence = 1;
			} else if (_sequence == 1) {
				/* also mark the inventory dirty when we do this */
				Inventory::updated(false);
				_sequence = 2;
			}
		}

	private:
		const Command _put_on;
		const Command _put_on_key;
	};
}
#endif

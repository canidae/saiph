#ifndef ACTION_PAY_H
#define ACTION_PAY_H

#include "World.h"
#include "Inventory.h"
#include "Actions/Action.h"

namespace action {

	class Pay : public Action {
	public:
		static const int ID;

		Pay(analyzer::Analyzer* analyzer, int priority) : Action(analyzer), _pay("p", priority), _answer_no(NO, PRIORITY_CONTINUE_ACTION), _answer_yes(YES, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Pay() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {

			case 0:
				return _pay;

			case 1:
				return _answer_no;

			case 2:
				return _answer_yes;
                
			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (messages.find(MESSAGE_ITEMIZED_BILLING) != std::string::npos) {
				/* asking if we want to use itemized billing */
				_sequence = 1;
			} else if (messages.find(MESSAGE_PAY) != std::string::npos) {
				/* we do want to pay for this item */
				_sequence = 2;
			} else {
				/* we finished paying */
				if (_sequence != 2) failed();
				_sequence = 3;
				Inventory::update();
			}
		}

	private:
		const Command _pay;
		const Command _answer_no;
		const Command _answer_yes;
	};
}
#endif

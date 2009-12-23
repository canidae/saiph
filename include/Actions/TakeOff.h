#ifndef TAKEOFF_H
#define	TAKEOFF_H

#include "Inventory.h"
#include "Actions/Action.h"

#define TAKE_OFF_ARMOR_CURSED "  You can't. "

namespace action {

	class TakeOff : public Action {
	public:
		static const int ID;

		TakeOff(analyzer::Analyzer* analyzer, unsigned char key, int priority) : Action(analyzer), _take_off("T", priority), _take_off_key(key, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~TakeOff() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _take_off;

			case 1:
				return _take_off_key;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (World::question() && messages.find(MESSAGE_WHAT_TO_TAKE_OFF) != std::string::npos) {
				_sequence = 1;
			} else if (_sequence == 1 || messages.find(MESSAGE_YOU_FINISH_TAKING_OFF) != std::string::npos || messages.find(TAKE_OFF_ARMOR_CURSED) != std::string::npos) {
				/* also mark the inventory dirty when we do this */
				Inventory::update();
				_sequence = 2;
			}
		}

	private:
		const Command _take_off;
		const Command _take_off_key;
	};
}
#endif

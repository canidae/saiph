#ifndef ACTION_WIELD_H
#define	ACTION_WIELD_H

#include "Action.h"
#include "../Inventory.h"

/* messages */
#define WEAPON_WHAT_TO_WIELD "  What do you want to wield? "
#define WIELD_WEAPON_CURSED " is welded to your "

namespace action {

	class Wield : public Action {
	public:
		static const int ID;

		Wield(analyzer::Analyzer* analyzer, unsigned char key, int priority) : Action(analyzer), _wield("w", priority), _wield_key(key, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~Wield() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _wield;

			case 1:
				return _wield_key;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			if (World::question() && messages.find(MESSAGE_WHAT_TO_WIELD) != std::string::npos) {
				_sequence = 1;
			} else if (_sequence == 1 || messages.find(WIELD_WEAPON_CURSED) != std::string::npos) {
				/* also mark the inventory dirty when we do this */
				Inventory::update();
				_sequence = 2;
			}
		}

	private:
		const Command _wield;
		const Command _wield_key;
	};
}
#endif

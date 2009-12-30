#ifndef ACTION_LIST_PLAYER_ATTRIBUTES_H
#define	ACTION_LIST_PLAYER_ATTRIBUTES_H

#include "World.h"
#include "Actions/Action.h"

namespace action {

	class ListPlayerAttributes : public Action {
	public:
		static const int ID;

		/* this is done at the start of each game.
		 * XXX:
		 * since Weapon analyzer calculate score of weapons when we list inventory
		 * and because players role determines the weapon score we need to determine
		 * the role of the player before we list inventory, thus priority is set to
		 * PRIORITY_CONTINUE_ACTION instead of PRIORITY_LOOK as it should be */
		ListPlayerAttributes(analyzer::Analyzer* analyzer) : Action(analyzer, false), _list_attributes("", PRIORITY_CONTINUE_ACTION), _close_page(CLOSE_PAGE, PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~ListPlayerAttributes() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _list_attributes;

			case 1:
				return _close_page;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			if (World::menu())
				_sequence = 1;
			else if (_sequence == 1)
				_sequence = 2;
		}

	private:
		const Command _list_attributes;
		const Command _close_page;
	};
}
#endif

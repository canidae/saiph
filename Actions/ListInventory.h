#ifndef ACTION_LIST_INVENTORY_H
#define ACTION_LIST_INVENTORY_H

#include "Action.h"
#include "../World.h"

namespace action {

	class ListInventory : public Action {
	public:
		static int id;

		ListInventory(analyzer::Analyzer *analyzer) : Action(analyzer), list_inventory("i", PRIORITY_LOOK), close_page(" ", PRIORITY_CONTINUE_ACTION) {
		}

		virtual ~ListInventory() {
		};

		virtual int getID() {
			return id;
		}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command list_inventory;
		const Command close_page;
	};

	inline const Command &action::ListInventory::getCommand() {
		switch (sequence) {
		case 0:
			return list_inventory;

		case 1:
			return close_page;

		default:
			return Action::noop;
		}
	}

	inline void action::ListInventory::updateAction(const std::string &) {
		if (World::menu)
			sequence = 1;
		else
			sequence = 2;
	}
}
#endif

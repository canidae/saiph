#ifndef ACTION_LIST_INVENTORY_H
#define ACTION_LIST_INVENTORY_H

#include "Action.h"

namespace action {
	class ListInventory : public Action {
	public:
		static int id;

		ListInventory(analyzer::Analyzer *analyzer) : Action(analyzer), list_inventory("i", PRIORITY_LOOK) {}
		virtual ~ListInventory() {};

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command list_inventory;
	};
}

/* methods */
inline const Command &action::ListInventory::getCommand() {
	switch (sequence) {
	case 0: 
		return list_inventory;

	default:
		return Action::noop;
	}
}

inline void action::ListInventory::updateAction(const std::string &) {
	++sequence;
}
#endif

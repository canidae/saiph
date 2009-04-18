#ifndef _LIST_INVENTORY_H
#define _LIST_INVENTORY_H

#include "Action.h"

namespace action {
	class ListInventory : public Action {
	public:
		static const int id;

		ListInventory();
		virtual ~ListInventory() {};

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction();

	private:
		const Command list_inventory;
	};
}
#endif

#ifndef ACTION_LIST_INVENTORY_H
#define ACTION_LIST_INVENTORY_H

#include "Actions/Action.h"

namespace action {
	class ListInventory : public Action {
	public:
		static const int ID;

		ListInventory(analyzer::Analyzer* analyzer);
		virtual ~ListInventory();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		const Command _list_inventory;
		const Command _close_page;
	};
}
#endif

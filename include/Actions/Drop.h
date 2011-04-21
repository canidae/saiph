#ifndef ACTION_DROP_H
#define	ACTION_DROP_H

#include <map>
#include <queue>
#include <sstream>
#include <vector>
#include "EventBus.h"
#include "Globals.h"
#include "Inventory.h"
#include "Item.h"
#include "World.h"
#include "Actions/Action.h"
#include "Events/WantItems.h"

namespace action {
	class Drop : public Action {
	public:
		static const int ID;

		Drop(analyzer::Analyzer* analyzer, int priority, bool safe_stash);
		virtual ~Drop();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _drop;
		const Command _close_page;
		const Command _look;
		const bool _safe_stash;
		Command _drop_item;
		std::queue<std::string> _keys;
	};
}
#endif

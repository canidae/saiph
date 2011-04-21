#ifndef ACTION_LOOT_H
#define ACTION_LOOT_H

#include <map>
#include <queue>
#include <sstream>
#include <vector>
#include "EventBus.h"
#include "Inventory.h"
#include "Item.h"
#include "World.h"
#include "Actions/Action.h"
#include "Actions/Look.h"
#include "Events/WantItems.h"

namespace action {
	class Loot : public Action {
	public:
		static const int ID;

		Loot(analyzer::Analyzer* analyzer, int priority, bool safe_stash);
		virtual ~Loot();

		static bool canLoot();
		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _loot;
		const Command _close_page;
		const bool _safe_stash;
		Command _loot_item;
		std::queue<std::string> _keys;
	};
}
#endif

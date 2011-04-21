#ifndef ACTION_APPLY_IN_DIRECTION_H
#define ACTION_APPLY_IN_DIRECTION_H

#include "Inventory.h"
#include "Actions/Action.h"

namespace action {
	class ApplyInDirection : public Action {
	public:
		static const int ID;

		ApplyInDirection(analyzer::Analyzer* analyzer, unsigned char key, unsigned char direction, int priority, bool update_inventory = true);
		virtual ~ApplyInDirection();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _do_apply;
		const Command _apply_item;
		const Command _apply_direction;
		const bool _update_inventory;
	};
}
#endif

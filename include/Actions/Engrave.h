#ifndef ACTION_ENGRAVE_H
#define ACTION_ENGRAVE_H

#include "Saiph.h"
#include "World.h"
#include "Debug.h"
#include "Inventory.h"
#include "Actions/Action.h"

// TODO: (can't engrave)
#define MESSAGE_YOU_CANT_HOLD "  You can't even hold anything!  "

namespace action {
	class Engrave : public Action {
	public:
		static const int ID;

		Engrave(analyzer::Analyzer* analyzer, const std::string& engrave_msg, unsigned char engrave_with, int priority, bool should_append = true);
		virtual ~Engrave();

		static bool canEngrave();
		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _engrave;
		const Command _message;
		const Command _item;
		const Command _append;
	};
}
#endif

#ifndef ACTION_CALL_H
#define ACTION_CALL_H

#include "Inventory.h"
#include "World.h"
#include "Actions/Action.h"

namespace action {
	class Call : public Action {
	public:
		static const int ID;

		Call(analyzer::Analyzer* analyzer, unsigned char item, const std::string& call);
		virtual ~Call();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _do_call;
		const Command _call_individual;
		const Command _call_item;
		const Command _set_call;
	};
}
#endif

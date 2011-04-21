#ifndef ACTION_PAY_H
#define ACTION_PAY_H

#include "World.h"
#include "Inventory.h"
#include "Actions/Action.h"

namespace action {
	class Pay : public Action {
	public:
		static const int ID;

		Pay(analyzer::Analyzer* analyzer, int priority);
		virtual ~Pay();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _pay;
		const Command _answer_no;
		const Command _answer_yes;
	};
}
#endif

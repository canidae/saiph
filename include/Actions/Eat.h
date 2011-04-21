#ifndef ACTION_EAT_H
#define ACTION_EAT_H

#include "Inventory.h"
#include "Actions/Action.h"

namespace action {
	class Eat : public Action {
	public:
		static const int ID;

		Eat(analyzer::Analyzer* analyzer, unsigned char key, int priority);
		virtual ~Eat();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _eat;
		const Command _item;
		const Command _answer_no;
		const Command _answer_yes;
	};
}
#endif

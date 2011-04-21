#ifndef ACTION_NAME_H
#define ACTION_NAME_H

#include "Inventory.h"
#include "World.h"
#include "Actions/Action.h"

namespace action {
	class Name : public Action {
	public:
		static const int ID;

		Name(analyzer::Analyzer* analyzer, unsigned char item, const std::string& name);
		virtual ~Name();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _do_name;
		const Command _name_individual;
		const Command _name_item;
		const Command _set_name;
	};
}
#endif

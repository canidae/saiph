#ifndef ACTION_APPLY_H
#define ACTION_APPLY_H

#include "Actions/Action.h"

namespace action {
	class Apply : public Action {
	public:
		static const int ID;

		Apply(analyzer::Analyzer* analyzer, unsigned char key, const int& priority, bool update_inventory = true);
		virtual ~Apply();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _do_apply;
		const Command _apply_item;
		const bool _update_inventory;
	};
}
#endif

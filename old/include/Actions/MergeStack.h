#ifndef ACTION_MERGE_STACK_H
#define ACTION_MERGE_STACK_H

#include "Actions/Action.h"

namespace action {
	class MergeStack : public Action {
	public:
		static const int ID;

		MergeStack(analyzer::Analyzer* analyzer, unsigned char item);
		virtual ~MergeStack();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _do_adjust;
		const Command _name_item;
	};
}
#endif

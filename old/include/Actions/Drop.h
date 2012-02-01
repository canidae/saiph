#ifndef ACTION_DROP_H
#define	ACTION_DROP_H

#include <queue>
#include "Actions/Action.h"

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

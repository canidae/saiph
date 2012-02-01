#ifndef ACTION_DROPGOLD_H
#define	ACTION_DROPGOLD_H

#include "Actions/Action.h"

namespace action {
	class DropGold : public Action {
	public:
		static const int ID;

		DropGold(analyzer::Analyzer* analyzer, int priority);
		virtual ~DropGold();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _drop;
		const Command _gold;
	};
}
#endif

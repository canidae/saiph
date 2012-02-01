#ifndef ACTION_SEARCH_H
#define ACTION_SEARCH_H

#include "Actions/Action.h"

namespace action {
	class Search : public Action {
	public:
		static const int ID;

		Search(analyzer::Analyzer* analyzer, int priority);
		virtual ~Search();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		const int _turn_before_search;
		const Command _search;
	};
}
#endif

#ifndef ACTION_REST_H
#define ACTION_REST_H

#include "Actions/Action.h"

namespace action {
	class Rest : public Action {
	public:
		static const int ID;

		Rest(analyzer::Analyzer* analyzer, int priority);
		virtual ~Rest();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		const Command _rest;
	};
}
#endif

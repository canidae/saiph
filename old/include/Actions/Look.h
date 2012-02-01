#ifndef ACTION_LOOK_H
#define ACTION_LOOK_H

#include "Actions/Action.h"

namespace action {
	class Look : public Action {
	public:
		static const int ID;

		Look(analyzer::Analyzer* analyzer);
		virtual ~Look();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		const Command _look;
	};
}
#endif

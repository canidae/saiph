#ifndef ACTION_THROW_H
#define ACTION_THROW_H

#include "Actions/Action.h"

namespace action {
	class Throw : public Action {
	public:
		static const int ID;

		Throw(analyzer::Analyzer* analyzer, unsigned char key, unsigned char direction, int priority);
		virtual ~Throw();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _do_throw;
		const Command _throw_item;
		const Command _throw_direction;
	};
}
#endif

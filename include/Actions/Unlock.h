#ifndef ACTION_UNLOCK_H
#define ACTION_UNLOCK_H

#include "Actions/Action.h"
#include "Globals.h"

namespace action {
	class Unlock : public Action {
	public:
		static const int ID;

		Unlock(analyzer::Analyzer* analyzer, unsigned char key, unsigned char direction, int priority);
		virtual ~Unlock();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _do_apply;
		const Command _apply_item;
		const Command _apply_direction;
		const Command _do_unlock;
	};
}
#endif

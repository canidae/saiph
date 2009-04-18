#ifndef _MOVE_H
#define _MOVE_H

#include "Action.h"

namespace action {
	class Move : public Action {
	public:
		Move(unsigned char direction, int priority);
		virtual ~Move() {}

		virtual const Command &getCommand();
		virtual void updateAction();

	private:
		const Command move;
	};
}
#endif

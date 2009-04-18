#ifndef _MOVE_H
#define _MOVE_H

#include "Action.h"

namespace action {
	class Move : public Action {
	public:
		static const int id;

		Move(unsigned char direction, int priority);
		virtual ~Move() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction();

	private:
		const Command move;
	};
}
#endif

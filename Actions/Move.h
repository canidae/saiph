#ifndef _MOVE_H
#define _MOVE_H

#include "Action.h"

class Saiph;

namespace Action {
	class Move : public Action {
	public:
		Move(unsigned char direction, int priority);
		virtual ~Move();

		virtual const Command &execute(Saiph *saiph);

	private:
		const Command move;
	};
}
#endif

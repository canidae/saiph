#ifndef _KICK_H
#define _KICK_H

#include "Action.h"

class Saiph;

namespace action {
	class Kick : public Action {
	public:
		Kick(unsigned char direction, int priority);
		virtual ~Kick();

		virtual const Command &execute(Saiph *saiph);

	private:
		const Command kick;
		const Command kick_direction;
	};
}
#endif

#ifndef _KICK_H
#define _KICK_H

#include "Action.h"

namespace action {
	class Kick : public Action {
	public:
		Kick(unsigned char direction, int priority);
		virtual ~Kick();

		virtual const Command &getCommand();
		virtual void updateAction();

	private:
		const Command kick;
		const Command kick_direction;
	};
}
#endif

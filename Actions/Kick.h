#ifndef _KICK_H
#define _KICK_H

#include "Action.h"

namespace action {
	class Kick : public Action {
	public:
		static const int id;

		Kick(unsigned char direction, int priority);
		virtual ~Kick() {}

		virtual int getID() {return id;}
		virtual const Command &getCommand();
		virtual void updateAction(const std::string &messages);

	private:
		const Command kick;
		const Command kick_direction;
	};
}
#endif

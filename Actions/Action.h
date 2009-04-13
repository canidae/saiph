#ifndef _ACTION_H
#define _ACTION_H

#include "../Command.h"

class Saiph;

namespace action {
	class Action {
	public:
		static const Command noop;

		Action();
		virtual ~Action();

		virtual const Command &getCommand();
		virtual void updateAction(const Saiph *saiph);

	protected:
		int sequence;
	};
}
#endif

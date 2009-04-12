#ifndef _ACTION_H
#define _ACTION_H

#include "../Command.h"
#include "../Globals.h"

class Saiph;

namespace action {
	class Action {
	public:
		static const Command noop;

		Action();
		virtual ~Action();

		virtual const Command &execute(Saiph *saiph);

	protected:
		int sequence;
	};
}
#endif

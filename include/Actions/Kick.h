#ifndef ACTION_KICK_H
#define ACTION_KICK_H

#include "Actions/Action.h"
#include "Globals.h"

namespace action {
	class Kick : public Action {
	public:
		static const int ID;

		Kick(analyzer::Analyzer* analyzer, unsigned char direction, int priority);
		virtual ~Kick();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _kick;
		const Command _kick_direction;
	};
}
#endif

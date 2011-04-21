#ifndef ACTION_DONATE_H
#define	ACTION_DONATE_H

#include <sstream>
#include "Globals.h"
#include "Actions/Action.h"

namespace action {
	class Donate : public Action {
	public:
		static const int ID;

		Donate(analyzer::Analyzer* analyzer, unsigned char direction, int amount, int priority);
		virtual ~Donate();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _do_donate;
		const Command _donate_direction;
		Command _donate_amount;
	};
}
#endif

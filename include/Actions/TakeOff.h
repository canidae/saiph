#ifndef TAKEOFF_H
#define	TAKEOFF_H

#include "Actions/Action.h"

#define TAKE_OFF_ARMOR_CURSED "  You can't. "

namespace action {
	class TakeOff : public Action {
	public:
		static const int ID;

		TakeOff(analyzer::Analyzer* analyzer, unsigned char key, int priority);
		virtual ~TakeOff();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _take_off;
		const Command _take_off_key;
	};
}
#endif

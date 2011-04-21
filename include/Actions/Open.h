#ifndef ACTION_OPEN_H
#define ACTION_OPEN_H

#include "Actions/Action.h"
#include "Globals.h"

namespace action {
	class Open : public Action {
	public:
		static const int ID;

		Open(analyzer::Analyzer* analyzer, unsigned char direction, int priority);
		virtual ~Open();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _open;
		const Command _open_direction;
	};
}
#endif

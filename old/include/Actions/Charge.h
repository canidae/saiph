#ifndef ACTION_CHARGE_H
#define ACTION_CHARGE_H

#include "Actions/Action.h"

namespace action {
	class Charge : public Action {
	public:
		static const int ID;

		Charge(analyzer::Analyzer* analyzer, unsigned char scroll, unsigned char item, int priority);
		virtual ~Charge();

		virtual int id();
		virtual bool canRead();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _read;
		const Command _charge_scroll;
		const Command _charge_item;
	};
}
#endif

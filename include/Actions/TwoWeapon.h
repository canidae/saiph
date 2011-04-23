#ifndef ACTION_TWOWEAPON_H
#define ACTION_TWOWEAPON_H

#include "Actions/Action.h"

namespace action {
	class TwoWeapon : public Action {
	public:
		static const int ID;

		TwoWeapon(analyzer::Analyzer* analyzer, int priority);
		virtual ~TwoWeapon();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _two_weap;
	};
}
#endif

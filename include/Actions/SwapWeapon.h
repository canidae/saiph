#ifndef ACTION_SWAPWEAPON_H
#define ACTION_SWAPWEAPON_H

#include "Actions/Action.h"

namespace action {
	class SwapWeapon : public Action {
	public:
		static const int ID;

		SwapWeapon(analyzer::Analyzer* analyzer, int priority);
		virtual ~SwapWeapon();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _swap_weap;
	};
}
#endif

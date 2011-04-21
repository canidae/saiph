#ifndef ACTION_DIP_H
#define ACTION_DIP_H

#include "Actions/Action.h"

namespace action {
	class Dip : public Action {
	public:
		static const int ID;

		Dip(analyzer::Analyzer* analyzer, unsigned char key, unsigned char dip_into, const int& priority, bool update_inventory = true);
		virtual ~Dip();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _do_dip;
		const Command _dip_item;
		const Command _dip_in_water;
		const Command _dip_in_item;
		const bool _update_inventory;
	};
}
#endif

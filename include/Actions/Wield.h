#ifndef ACTION_WIELD_H
#define	ACTION_WIELD_H

#include "Inventory.h"
#include "World.h"
#include "Actions/Action.h"

/* messages */
#define WEAPON_WHAT_TO_WIELD "  What do you want to wield? "
#define WIELD_WEAPON_CURSED " is welded to your "

namespace action {
	class Wield : public Action {
	public:
		static const int ID;

		Wield(analyzer::Analyzer* analyzer, unsigned char key, int priority);
		virtual ~Wield();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _wield;
		const Command _wield_key;
	};
}
#endif

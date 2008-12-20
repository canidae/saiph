#ifndef VALKYRIE_H
#define VALKYRIE_H

#include "../Analyzer.h"
#include "../Request.h"

class Saiph;

class Valkyrie : public Analyzer {
	public:
		Valkyrie(Saiph *saiph);

		void init();

	private:
		Saiph *saiph;
		Request req;
		unsigned char loot_group;

		void setupAmulet();
		void setupArmor();
		void setupFood();
		void setupPotion();
		void setupRing();
		void setupTool();
		void setupWand();
		void setupWeapon();
};
#endif

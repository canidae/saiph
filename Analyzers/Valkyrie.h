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

		void setupArmor(int &groupstart);
		void setupFood(int &groupstart);
		void setupTool(int &groupstart);
		void setupWand(int &groupstart);
		void setupWeapon(int &groupstart);
};
#endif

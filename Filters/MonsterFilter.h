#ifndef MONSTERFILTER_H
#define MONSTERFILTER_H

/* forward declare */
class MonsterFilter;

#include "../Filter.h"
#include "../Saiph.h"

using namespace std;

class MonsterFilter : public Filter {
	public:
		/* variables */
		Saiph *saiph;

		/* constructors */
		MonsterFilter(Saiph *saiph);

		/* destructors */
		~MonsterFilter();

		/* methods */
		virtual void filter(int row, int col) const;
};

#endif

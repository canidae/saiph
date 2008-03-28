#include "MonsterFilter.h"

/* constructors */
MonsterFilter::MonsterFilter(Saiph *saiph) {
	this->saiph = saiph;
	this->type = FILTER_MONSTER;
}

/* destructors */
MonsterFilter::~MonsterFilter() {
}

/* methods */
void MonsterFilter::filter(int row, int col) const {
	/*
	saiph->farlook(row, col);
	saiph->parseMessages();
	*/
}

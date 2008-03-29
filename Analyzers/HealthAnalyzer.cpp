#include "HealthAnalyzer.h"

/* constructors */
HealthAnalyzer::HealthAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	this->type = ANALYZE_NONE;
}

/* destructors */
HealthAnalyzer::~HealthAnalyzer() {
}

/* methods */
void HealthAnalyzer::finish() {
	cerr << "HealthAnalyzer finish" << endl;
	/* figure out if we're in danger of dying */
	/* illness */
	if (saiph->world->player.status.ill) {
		/* find out the best way to cure this
		 * spell, potion, unihorn, pray
		 *
		 * pray for now */
		saiph->setNextCommand("#pray\n", 100);
	}
	/* hp */
	int hp = saiph->world->player.status.hitpoints;
	int hp_max = saiph->world->player.status.hitpoints_max;
	if (hp < 6 || hp * 7 <= hp_max) {
		/* almost dead, find an urgent way to heal up.
		 * spell, potion, (elbereth?), pray
		 *
		 * pray for now */
		saiph->setNextCommand("#pray", 100);
	} else if (hp * 2 < hp_max) {
		/* health is going low.
		 * elbereth, run, potion, lots of options
		 *
		 * elbereth for now */
		saiph->setNextCommand("E- Elbereth\n", 90);
	}
}

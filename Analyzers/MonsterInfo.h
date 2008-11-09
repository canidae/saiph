#ifndef MONSTERINFO_H
#define MONSTERINFO_H

#include <map>
#include <string>
#include "../Analyzer.h"
#include "../Monster.h"

class Saiph;

class MonsterInfo : public Analyzer {
	public:
		MonsterInfo(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		std::map<Point, Monster>::iterator look_at;
		int last_check_internal_turn;
};
#endif

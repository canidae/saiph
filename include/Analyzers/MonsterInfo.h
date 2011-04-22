#ifndef ANALYZER_MONSTERINFO_H
#define ANALYZER_MONSTERINFO_H

#include <map>
#include <string>
#include <vector>
#include "Monster.h"
#include "Point.h"
#include "Analyzers/Analyzer.h"
#include "Actions/FarLook.h"

namespace analyzer {
	class MonsterInfo : public Analyzer {
	public:
		MonsterInfo();

		void analyze();
		void actionCompleted(const std::string&);

	private:
		std::vector<action::FarLook::Request> _requests;
		std::map<Point, unsigned int> _checked;
	};
}
#endif

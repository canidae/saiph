#include "Analyzers/MonsterInfo.h"

#include <stdlib.h>
#include "Debug.h"
#include "Saiph.h"
#include "World.h"
#include "Level.h"
#include "Actions/FarLook.h"
#include "Data/Monster.h"

using namespace analyzer;
using namespace std;

/* public constructors */
MonsterInfo::MonsterInfo() : Analyzer("MonsterInfo") {
}

/* methods */
void MonsterInfo::analyze() {
	_requests.clear();
	vector<Point> lreq = World::level().farlooksNeeded();
	for (vector<Point>::iterator lri = lreq.begin(); lri != lreq.end(); ++lri)
		_requests.push_back(action::FarLook::Request(*lri));
	if (!_requests.empty())
		World::setAction(new action::FarLook(this, _requests));
}

void MonsterInfo::actionCompleted(const string&) {
}

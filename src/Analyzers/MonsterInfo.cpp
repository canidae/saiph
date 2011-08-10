#include "Analyzers/MonsterInfo.h"

#include <stdlib.h>
#include "Debug.h"
#include "Saiph.h"
#include "World.h"
#include "Level.h"
#include "Actions/Christen.h"
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
	if (!_requests.empty()) {
		World::setAction(new action::FarLook(this, _requests));
		return;
	}

	vector<pair<Point, string> > christen_req;
	for (map<Point, Monster*>::const_iterator i = World::level().monsters().begin(); i != World::level().monsters().end(); ++i) {
		if (i->second->visible() && !i->second->called() && !i->second->priest() && i->second->data() && (i->second->data()->genoFlags() & G_UNIQ) == 0) {
			Debug::notice() << "Will name " << i->second->id() << " (" << (i->second->data() ? i->second->data()->name() : "unknown") << ") at " << i->first << endl;
			christen_req.push_back(make_pair(i->first, i->second->id()));
		}
	}
	if (!christen_req.empty())
		World::setAction(new action::Christen(this, christen_req));
}

void MonsterInfo::actionCompleted(const string&) {
}

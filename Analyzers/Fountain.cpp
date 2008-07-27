#include "Fountain.h"

/* constructors */
Fountain::Fountain(Saiph *saiph) : Analyzer("Fountain"), saiph(saiph), target(-1, -1, -1, -1), target_priority(ILLEGAL_PRIORITY) {
}

/* methods */
void Fountain::complete() {
}

void Fountain::finish() {
	if (target.branch == -1)
		return;
	/* move towards fountain */
	if (target.branch != saiph->position.branch) {
		/* we need to path to the right branch.
		 * however, we're not quite there yet.
		 * we currently don't handle branches,
		 * so this won't ever happen anyways */
		return;
	}
	if (target.level != saiph->position.level) {
		/* request to go up/down */
		req.priority = target_priority;
		if (target.level < saiph->position.level) {
			/* go up */
			req.request = REQUEST_ASCEND_DUNGEON;
		} else {
			/* go down */
			req.request = REQUEST_DESCEND_DUNGEON;
		}
		saiph->request(req);
		return;
	}
	int distance = -1;
	unsigned char move = saiph->shortestPath(target, false, &distance);
	if (move == ILLEGAL_MOVE) {
		/* well, we're screwed. we can't path to this fountain.
		 * if this happen, we got ourselves an oscillobot */
		return;
	}
	command = move;
	priority = target_priority;
}

void Fountain::inspect(const Point &point) {
	if (saiph->dungeonmap[saiph->position.branch][saiph->position.level][point.row][point.col] != FOUNTAIN) {
		if (fountains[saiph->position.branch][saiph->position.level].find(point) != fountains[saiph->position.branch][saiph->position.level].end()) {
			/* this fountain is gone */
			fountains[saiph->position.branch][saiph->position.level].erase(point);
		}
		return;
	}
	/* fountain found */
	fountains[saiph->position.branch][saiph->position.level][point] = true;
	/* since we might find a fountain while heading towards a fountain,
	 * we'll have to reset target.branch so it'll repath */
	target.branch = -1;
}

void Fountain::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_FOUNTAIN_DRIES_UP, 0) != string::npos || messages.find(MESSAGE_FOUNTAIN_DRIES_UP2, 0) != string::npos) {
		/* fountain is now gone */
		fountains[saiph->position.branch][saiph->position.level].erase(saiph->position);
	}
}

bool Fountain::request(const Request &request) {
	if (request.request != REQUEST_VISIT_NEAREST_FOUNTAIN)
		return false;
	/* someone wants to visit the nearest fountain */
	if (request.priority > target_priority)
		target_priority = request.priority;
	if (target.branch != -1)
		return true; // we're already on our way
	for (map<int, map<int, map<Point, bool> > >::iterator a = fountains.begin(); a != fountains.end(); ++a) {
		if (target.branch != -1 && abs(target.branch - a->first) < abs(saiph->position.branch - a->first))
			continue;
		for (map<int, map<Point, bool> >::iterator b = a->second.begin(); b != a->second.end(); ++b) {
			if (target.branch != -1 && abs(target.level - b->first) < abs(saiph->position.level - b->first))
				continue;
			for (map<Point, bool>::iterator l = b->second.begin(); l != b->second.end(); ++l) {
				target.branch = a->first;
				target.level = b->first;
				target.row = l->first.row;
				target.col = l->first.col;
			}
		}
	}
	if (target.branch == -1)
		return false;
	return true;
}

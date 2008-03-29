#include "DoorAnalyzer.h"

/* constructors */
DoorAnalyzer::DoorAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	this->type = ANALYZE_CLOSED_DOOR | ANALYZE_OPEN_DOOR;
}

/* destructors */
DoorAnalyzer::~DoorAnalyzer() {
}

/* methods */
void DoorAnalyzer::analyze(int row, int col, char symbol) {
	/* found a door */
	if (symbol == OPEN_DOOR)
		saiph->branch[saiph->current_branch].door[row][col] = DA_DOOR_OPEN;
	else
		saiph->branch[saiph->current_branch].door[row][col] = DA_DOOR_CLOSED;
}

void DoorAnalyzer::finish() {
	/* we could remove doors here if they seem to get destroyed */
}

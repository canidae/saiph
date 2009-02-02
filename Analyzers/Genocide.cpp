#include "Genocide.h"
#include "../Globals.h"

using namespace std;

Genocide::Genocide(Saiph* saiph) : Analyzer("Genocide"), saiph(saiph), genoedL(false), genoedSemicolon(false), genoedRegularFlayer(false), genoedMasterFlayer(false), genoedRust(false), genoedDisenchant(false) {
}

void Genocide::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_WHAT_TO_GENOCIDE_UNCURSED) != string::npos) {
		if (!genoedRegularFlayer) {
			command = "mind flayer\n";
			genoedRegularFlayer = true;
		} else if (!genoedMasterFlayer) {
			command = "master mind flayer\n";
			genoedMasterFlayer = true;
		} else if (!genoedRust) {
			command = "rust monster\n";
			genoedRust = true;
		} else if (!genoedDisenchant) {
			command = "disenchanter\n";
			genoedDisenchant = true;
		} else
			command = "none\n";
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_WHAT_TO_GENOCIDE_BLESSED) != string::npos) {
		if (!genoedL) {
			command = "L\n";
			genoedL = true;
		} else if (!genoedSemicolon) {
			command = ";\n";
			genoedSemicolon = true;
		} else if (!genoedRegularFlayer || !genoedMasterFlayer) {
			command = "h\n";
			genoedRegularFlayer = true;
			genoedMasterFlayer = true;
		} else if (!genoedRust || !genoedDisenchant) {
			command = "R\n";
			genoedDisenchant = true;
			genoedRust = true;
		} else
			command = "none\n";
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

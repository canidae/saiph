#include "Genocide.h"
#include "../Globals.h"
#include "../World.h"

using namespace analyzer;
using namespace std;

Genocide::Genocide(Saiph* saiph) : Analyzer("Genocide"), saiph(saiph) {
	monsters.push_back("mind flayer");
	monsters.push_back("master mind flayer");
	monsters.push_back("rust monster");
	monsters.push_back("disenchanter");

	classes.push_back('L');
	classes.push_back(';');
	classes.push_back('h');
	classes.push_back('R');
}

void Genocide::parseMessages(const string& messages) {
	if (!saiph->world->question)
		return;
	if (messages.find(MESSAGE_WHAT_TO_GENOCIDE_UNCURSED) != string::npos) {
		if (monsters.size() > 0) {
			command = monsters.front();
			monsters.pop_front();
		} else {
			command = "none";
		}
		command.push_back('\n');
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_WHAT_TO_GENOCIDE_BLESSED) != string::npos) {
		if (classes.size() > 0) {
			command = classes.front();
			classes.pop_front();
		} else {
			command = "none";
		}
		command.push_back('\n');
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

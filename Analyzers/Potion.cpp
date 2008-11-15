#include "Potion.h"
#include "../Debug.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Potion::Potion(Saiph *saiph) : Analyzer("Potion"), saiph(saiph), command2("") {
	appearance.push_back("black");
	appearance.push_back("brilliant blue");
	appearance.push_back("brown");
	appearance.push_back("bubbly");
	appearance.push_back("clear");
	appearance.push_back("cloudy");
	appearance.push_back("cyan");
	appearance.push_back("effervescent");
	appearance.push_back("emerald");
	appearance.push_back("dark");
	appearance.push_back("dark green");
	appearance.push_back("fizzy");
	appearance.push_back("golden");
	appearance.push_back("magenta");
	appearance.push_back("milky");
	appearance.push_back("murky");
	appearance.push_back("orange");
	appearance.push_back("pink");
	appearance.push_back("puce");
	appearance.push_back("purple-red");
	appearance.push_back("ruby");
	appearance.push_back("sky blue");
	appearance.push_back("smoky");
	appearance.push_back("swirly");
	appearance.push_back("white");
	appearance.push_back("yellow");
}

/* methods */
void Potion::analyze() {
	if (saiph->world->player.experience > 10 && priority < POTION_QUAFF_GAIN_LEVEL) {
		/* see if we got a potion of gain level and quaff it */
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name != "potion of gain level")
				continue;
			/* cool, we do */
			command = QUAFF;
			priority = POTION_QUAFF_GAIN_LEVEL;
			command2 = i->first;
		}
	}
}

void Potion::parseMessages(const string &messages) {
	if (command2 != "" && messages.find(MESSAGE_WHAT_TO_DRINK, 0) != string::npos) {
		/* quaff the potion */
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
		command2.clear();
	} else if (saiph->world->question) {
		string::size_type stop = messages.find(POTION_CALL_END, 0);
		if (stop == string::npos)
			return;
		string::size_type start = messages.rfind(POTION_CALL_POTION1, stop);
		if (start == string::npos) {
			start = messages.rfind(POTION_CALL_POTION2, stop);
			if (start != string::npos)
				start += sizeof (POTION_CALL_POTION2) - 1;
		} else {
			start += sizeof (POTION_CALL_POTION1) - 1;
		}
		if (start == string::npos)
			return;
		string name = messages.substr(start, stop - start);
		Debug::notice() << "[Potion     ] Asking for name for a " << name << " potion" << endl;
		for (vector<string>::iterator a = appearance.begin(); a != appearance.end(); ++a) {
			if (name != *a)
				continue;
			/* recognized the potion */
			command = "hello :)\n";
			priority = PRIORITY_CONTINUE_ACTION;
		}
	}
}

bool Potion::request(const Request &request) {
	if (request.request == REQUEST_QUAFF_HEALING) {
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name != "potion of healing" && i->second.name != "potion of extra healing" && i->second.name != "potion of full healing")
				continue;
			/* just pick the first healing potion for now */
			command = QUAFF;
			priority = request.priority;
			command2 = i->first;
			return true;
		}
	}
	return false;
}

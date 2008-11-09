#include "Potion.h"
#include "../Debug.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Potion::Potion(Saiph *saiph) : Analyzer("Potion"), saiph(saiph) {
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
void Potion::complete() {
	sequence = -1;
}

void Potion::parseMessages(const string &messages) {
	if (saiph->world->question) {
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
			setCommand(0, PRIORITY_CONTINUE_ACTION, "hello :)\n");
			sequence = 0;
		}
	}
}

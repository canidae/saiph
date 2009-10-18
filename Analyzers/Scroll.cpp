#include "Scroll.h"
#include "../Debug.h"
#include "../Saiph.h"
#include "../World.h"

using namespace analyzer;
using namespace std;

/* constructors/destructor */
Scroll::Scroll(Saiph* saiph) : Analyzer("Scroll"), saiph(saiph) {
	appearance.push_back("ANDOVA BEGARIN");
	appearance.push_back("DAIYEN FOOELS");
	appearance.push_back("DUAM XNAHT");
	appearance.push_back("ELAM EBOW");
	appearance.push_back("ELBIB YLOH");
	appearance.push_back("FOOBIE BLETCH");
	appearance.push_back("GARVEN DEH");
	appearance.push_back("HACKEM MUCHE");
	appearance.push_back("JUYED AWK YACC");
	appearance.push_back("KERNOD WEL");
	appearance.push_back("KIRJE");
	appearance.push_back("LEP GEX VEN ZEA");
	appearance.push_back("NR 9");
	appearance.push_back("PRATYAVAYAH");
	appearance.push_back("PRIRUTSENIE");
	appearance.push_back("READ ME");
	appearance.push_back("TEMOV");
	appearance.push_back("THARR");
	appearance.push_back("VE FORBRYDERNE");
	appearance.push_back("VELOX NEB");
	appearance.push_back("VENZAR BORGAVVE");
	appearance.push_back("VERR YED HORRE");
	appearance.push_back("XIXAXA XOXAXA XUXAXA");
	appearance.push_back("YUM YUM");
	appearance.push_back("ZELGO MER");
}

/* methods */
void Scroll::parseMessages(const string& messages) {
	if (saiph->world->question) {
		string::size_type start = messages.find(SCROLL_CALL_SCROLL, 0);
		if (start == string::npos)
			return;
		string::size_type stop = messages.find(SCROLL_CALL_END, start);
		if (stop == string::npos)
			return;
		start += sizeof (SCROLL_CALL_SCROLL) - 1;
		string name = messages.substr(start, stop - start);
		Debug::info(saiph->last_turn) << SCROLL_DEBUG_NAME << "Asking for name for a " << name << " scroll" << endl;
		for (vector<string>::iterator a = appearance.begin(); a != appearance.end(); ++a) {
			if (name != *a)
				continue;
			/* recognized the scroll */
			command = "will you be my friend?\n";
			priority = PRIORITY_CONTINUE_ACTION;
		}
	}
}

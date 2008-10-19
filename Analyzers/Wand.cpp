#include "Wand.h"
#include "../Saiph.h"

using namespace std;

/* constructors/destructor */
Wand::Wand(Saiph *saiph) : Analyzer("Wand"), saiph(saiph) {
}

/* methods */
void Wand::analyze() {
}

void Wand::parseMessages(const string &messages) {
}

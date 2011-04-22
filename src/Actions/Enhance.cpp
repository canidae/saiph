#include "Globals.h"
#include "Analyzers/Analyzer.h"
#include "Actions/Enhance.h"
#include "EventBus.h"
#include "Events/ChangedSkills.h"
#include "Debug.h"
#include "World.h"

#include <string>

using namespace action;
using namespace std;

/* constructors/destructor */
Enhance::Enhance(analyzer::Analyzer* analyzer) : Action(analyzer, false), _do_enhance("#enhance\n", PRIORITY_LIST_PLAYER_SKILLS), _next_page(CLOSE_PAGE, PRIORITY_CONTINUE_ACTION), _enhance_a("a", PRIORITY_CONTINUE_ACTION) {
	for (int i = 0; i < P_NUM_SKILLS; ++i)
		_results[i] = 0;
}

Enhance::~Enhance() {
}

/* methods */
int Enhance::id() {
	return ID;
}

const Command& Enhance::command() {
	switch (_sequence) {
	case 0:
		return _do_enhance;

	case 1:
		return _next_page;

	case 2:
		return _enhance_a;

	default:
		return Action::NOOP;
	}
}

void Enhance::update(const std::string& messages) {
	if (_sequence <= 1 && World::menu()) {
		bool has_a = false;

		for (string::size_type ptr = messages.find("]  "); ptr != string::npos; ptr = messages.find("]  ", ptr + 3)) {
			string::size_type begin_lev_m2 = messages.find_last_of('[', ptr) - 1;
			string::size_type end_name_m1 = messages.find_last_not_of(' ', begin_lev_m2);
			string::size_type begin_name_m2 = messages.rfind("  ", end_name_m1);

			string name = messages.substr(begin_name_m2 + 2, (end_name_m1 + 1) - (begin_name_m2 + 2));
			string level = messages.substr(begin_lev_m2 + 2, ptr - (begin_lev_m2 + 2));

			int which_skill = data::Skill::parse(name);
			int which_level = data::Skill::levelParse(level);

			unsigned char letter = messages[begin_name_m2-1];
			if (messages[begin_name_m2-2] == '-')
				letter = messages[begin_name_m2-4];
			else if (letter != '*' && letter != '#')
				letter = ' ';

			if (which_skill == 0 || which_level == 0)
				Debug::custom("Skills") << "Found odd line <<" << letter << ">> <<" << name << ">> <<" << level << ">>" << endl;
			else
				_results[which_skill] = which_level;

			if (letter == 'a') has_a = true;
		}

		_sequence = has_a ? 2 : 1;
	} else if (_sequence == 1) {
		event::ChangedSkills cs;
		Saiph::updateSkills(_results);
		for (int i = 1; i < P_NUM_SKILLS; ++i)
			Debug::custom("Skills") << data::Skill::name(i) << " = " << data::Skill::levelName(Saiph::skill(i)) << endl;
		EventBus::broadcast(&cs);
		_sequence = 3;
	} else if (_sequence == 2) {
		_sequence = 3;
		World::queueAction(new Enhance(analyzer()));
	} else {
		Debug::warning() << "Phase error in Enhance" << endl;
		_sequence = 3;
	}
}

#include "Analyzer.h"
#include "Debug.h"
#include "AnalyzerBus.h"

#define ANALYZERBUS_DEBUG_NAME "AnalyzerBus] "

using namespace std;
using namespace analyzer;

vector<vector<Analyzer *> > AnalyzerBus::analyzers;

int AnalyzerBus::createAnalyzerType() {
	int analyzer_type = AnalyzerBus::analyzers.size();
	AnalyzerBus::analyzers.resize(analyzer_type + 1);
	return analyzer_type;
}

void AnalyzerBus::registerAnalyzer(int analyzer_type, Analyzer *analyzer) {
	if (analyzer_type < 0 || analyzer_type >= (int) AnalyzerBus::analyzers.size())
		return;
	Debug::notice() << ANALYZERBUS_DEBUG_NAME << "Registering " << analyzer->name << " for analyzer " << analyzer_type << endl;
	analyzers[analyzer_type].push_back(analyzer);
}

void AnalyzerBus::unregisterAnalyzer(int analyzer_type, Analyzer *analyzer) {
	if (analyzer_type < 0 || analyzer_type >= (int) AnalyzerBus::analyzers.size())
		return;
	vector<Analyzer *> &subscribers = analyzers[analyzer_type];
	for (vector<Analyzer *>::iterator s = subscribers.begin(); s != subscribers.end(); ++s) {
		if (*s == analyzer) {
			subscribers.erase(s);
			Debug::notice() << ANALYZERBUS_DEBUG_NAME << "Unregistered " << analyzer->name << " for analyzer " << analyzer_type << endl;
			return;
		}
	}
	Debug::warning() << ANALYZERBUS_DEBUG_NAME << "Failed to unregister " << analyzer->name << " for analyzer " << analyzer_type << endl;
}

void AnalyzerBus::broadcast(Analyzer *const analyzer) {
	if (analyzer->getType() < 0 || analyzer->getType() >= (int) AnalyzerBus::analyzers.size())
		return;
	vector<Analyzer *> &subscribers = analyzers[analyzer->getType()];
	for (vector<Analyzer *>::iterator s = subscribers.begin(); s != subscribers.end(); ++s)
		(*s)->handle(analyzer);
}

#ifndef _ANALYZERBUS_H
#define	_ANALYZERBUS_H

#include <vector>
#include "Analyzers/Analyzer.h"

class Analyzer;

class AnalyzerBus {
public:
	static int createAnalyzerType();
	static void registerAnalyzer(int analyzer_type, Analyzer *analyzer);
	static void unregisterAnalyzer(int analyzer_type, Analyzer *analyzer);
	static void broadcast(analyzer::Analyzer *const analyzer);

private:
	static std::vector<std::vector<Analyzer *> > analyzers;
};
#endif

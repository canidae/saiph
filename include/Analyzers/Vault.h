#ifndef ANALYZER_VAULT_H
#define ANALYZER_VAULT_H

#include <string>
#include "Analyzers/Analyzer.h"
#include "Point.h"

/* Croesus */
#define VAULT_CROESUS "Croesus\n"
/* messages */
#define VAULT_MESSAGE_STRANGER "  \"Hello stranger, who are you?\" -  "
#define VAULT_MESSAGE_DISAPPEAR "  Suddenly, the guard disappears.  "

namespace analyzer {

	class Vault : public Analyzer {
	public:
		Vault();

		void parseMessages(const std::string& messages);
		void actionCompleted(const std::string& messages);

	private:
		int _last_saw_guard_turn;
		bool _answering_guard;
		bool _dropping_gold;
		bool _following_guard;
	};
}
#endif

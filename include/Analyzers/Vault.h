#ifndef ANALYZER_VAULT_H
#define ANALYZER_VAULT_H

#include <string>
#include "Analyzer.h"

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
		void actionCompleted();

	private:
		bool _answering_guard;
		bool _dropping_gold;
		bool _looking_at_ground;
		bool _following_guard;
	};
}
#endif

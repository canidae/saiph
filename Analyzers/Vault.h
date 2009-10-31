#ifndef ANALYZER_VAULT_H
#define ANALYZER_VAULT_H

#include <string>
#include "Analyzer.h"

/* Croesus */
#define VAULT_CROESUS "Croesus\n"
/* messages */
#define VAULT_MESSAGE_STRANGER "  \"Hello stranger, who are you?\" -  "
#define VAULT_MESSAGE_DISAPPEAR "  Suddenly, the guard disappears.  "

class Saiph;

namespace analyzer {

	class Vault : public Analyzer {
	public:
		Vault();

		void parseMessages(const std::string& messages);
		void actionCompleted();

	private:
		bool _drop_gold;
		bool _look_at_ground;
		bool _follow_guard;
	};
}
#endif

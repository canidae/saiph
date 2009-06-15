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
		Vault(Saiph *saiph);

		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		bool drop_gold;
		bool look_at_ground;
		bool follow_guard;
	};
}
#endif

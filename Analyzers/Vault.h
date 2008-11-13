#ifndef VAULT_H
#define VAULT_H
/* Croesus */
#define VAULT_CROESUS "Croesus\n"
/* messages */
#define VAULT_MESSAGE_STRANGER "  \"Hello stranger, who are you?\" -  "
#define VAULT_MESSAGE_DISAPPEAR "  Suddenly, the guard disappears.  "
/* priority */
#define VAULT_GO_OUT_PRIORITY 999

#include <string>
#include "../Analyzer.h"

class Saiph;

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
#endif

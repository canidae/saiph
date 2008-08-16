#ifndef VAULT_H
/* defines */
#define VAULT_H
/* Croesus */
#define VAULT_CROESUS "Croesus\n"
/* messages */
#define VAULT_MESSAGE_STRANGER "  \"Hello stranger, who are you?\" -  "

/* forward declare */
class Vault;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Vault : public Analyzer {
	public:
		/* constructors */
		Vault(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		bool drop_gold;
};
#endif

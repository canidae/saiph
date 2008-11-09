#ifndef DOOR_H
#define DOOR_H
/* priorities */
#define DOOR_OPEN_PRIORITY 150

#include <string>
#include "../Analyzer.h"
#include "../Point.h"

class Saiph;

class Door : public Analyzer {
	public:
		Door(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		Point cur_door;
		unsigned char unlock_tool_key;

		void findUnlockingTool();
};
#endif

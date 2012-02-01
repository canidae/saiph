#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>

/* unique priorities */
#define ILLEGAL_PRIORITY -1
#define PRIORITY_CONTINUE_ACTION 1007
#define PRIORITY_SELECT_ITEM 1006
#define PRIORITY_CLOSE_SELECT_PAGE 1005
#define PRIORITY_LIST_PLAYER_ATTRIBUTES 1004
#define PRIORITY_LIST_PLAYER_SKILLS 1003
#define PRIORITY_LIST_INVENTORY 1002
#define PRIORITY_NAME_ITEM 1001
#define PRIORITY_LOOK 1000
#define PRIORITY_TURN_MAX 999
/* loot priority (when picking up, not moving to), due to centralized handling of looting */
#define PRIORITY_LOOT 300

/* some common answers */
#define CLOSE_PAGE ' '
#define DESELECT_PAGE '\\'
#define HANDS '-'
#define NO 'n'
#define QUIT "#quit\n"
#define SELECT_PAGE ','
#define YES 'y'

class Command {
public:

	Command(const std::string& command = "", int priority = ILLEGAL_PRIORITY);
	Command(unsigned char key, int priority);

	const std::string& command() const;
	const std::string& command(const std::string& command);
	int priority() const;
	int priority(int priority);

	bool operator==(const Command& c) const;

private:
	std::string _command;
	int _priority;
};

std::ostream& operator<<(std::ostream& os, const Command& c);
#endif

#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Analyzers/Analyzer.h"
#include "Connection.h"
#include "Debug.h"
#include "Globals.h"
#include "Saiph.h"
#include "Stash.h"
#include "World.h"
/* actions */
#include "Actions/Action.h"
/* data */
#include "Data/Item.h"
#include "Data/Monster.h"
/* analyzers */
#include "Analyzers/Amulet.h"
#include "Analyzers/Armor.h"
#include "Analyzers/Beatitude.h"
#include "Analyzers/Blind.h"
#include "Analyzers/Dig.h"
#include "Analyzers/Donate.h"
#include "Analyzers/Door.h"
#include "Analyzers/Elbereth.h"
#include "Analyzers/Enhance.h"
#include "Analyzers/Excalibur.h"
#include "Analyzers/Explore.h"
#include "Analyzers/Fight.h"
#include "Analyzers/Food.h"
#include "Analyzers/Genocide.h"
#include "Analyzers/Health.h"
#include "Analyzers/Lamp.h"
#include "Analyzers/Loot.h"
#include "Analyzers/MonsterInfo.h"
#include "Analyzers/Passtune.h"
#include "Analyzers/Potion.h"
#include "Analyzers/Pray.h"
#include "Analyzers/Ring.h"
#include "Analyzers/Rub.h"
#include "Analyzers/Scroll.h"
#include "Analyzers/Shop.h"
#include "Analyzers/Sokoban.h"
#include "Analyzers/Throne.h"
#include "Analyzers/Unihorn.h"
#include "Analyzers/Valkyrie.h"
#include "Analyzers/Vault.h"
#include "Analyzers/Wand.h"
#include "Analyzers/Weapon.h"
#include "Analyzers/Wish.h"

using namespace analyzer;
using namespace std;

/* static variables */
/* attributes */
int Saiph::alignment = NEUTRAL; // see defined constants
int Saiph::charisma = 0;
int Saiph::constitution = 0;
int Saiph::dexterity = 0;
int Saiph::intelligence = 0;
int Saiph::strength = 0;
int Saiph::wisdom = 0;
/* status */
int Saiph::armor_class = 0;
int Saiph::encumbrance = UNENCUMBERED; // see defined constants
int Saiph::experience = 0;
int Saiph::hunger = CONTENT; // see defined constants
int Saiph::hitpoints = 0;
int Saiph::hitpoints_max = 0;
int Saiph::power = 0;
int Saiph::power_max = 0;
/* effects */
bool Saiph::blind = false;
bool Saiph::confused = false;
bool Saiph::foodpoisoned = false;
bool Saiph::hallucinating = false;
bool Saiph::ill = false;
bool Saiph::slimed = false;
bool Saiph::stunned = false;
bool Saiph::lycanthropy = false;
bool Saiph::hurt_leg = false;
bool Saiph::polymorphed = false;
bool Saiph::levitating = false;
bool Saiph::engulfed = false;
/* position */
Coordinate Saiph::position;
/* zorkmids */
int Saiph::zorkmids = 0;
/* intrinsics/extrinsics */
unsigned long long int Saiph::intrinsics = 0;
unsigned long long int Saiph::extrinsics = 0;


/* constructors/destructor */
Saiph::Saiph() {
	/* bools for branches */
	sokoban_found = false;

	/* internal turn counter.
	 * whenever priority < 1000, we increase it by 1 */
	internal_turn = 0;

	/* clear last_command */
	last_command = "";

	/* inventory changed */
	inventory_changed = false;

	/* pickup/drop menu not showing */
	got_pickup_menu = false;
	got_drop_menu = false;

	/* set on_ground to NULL */
	on_ground = NULL;

	/* used to determine if we seem to be in a loop */
	last_turn = 0;
	stuck_counter = 0;

	/* used for cps/fps/tps */
	start_time = time(NULL);

	/* Analyzers */
	analyzers.push_back(new Amulet(this));
	analyzers.push_back(new Armor(this));
	analyzers.push_back(new Beatitude(this));
	analyzers.push_back(new Blind(this));
	analyzers.push_back(new Dig(this));
	analyzers.push_back(new Donate(this));
	analyzers.push_back(new Door(this));
	analyzers.push_back(new Elbereth(this));
	analyzers.push_back(new Enhance(this));
	analyzers.push_back(new Excalibur(this));
	analyzers.push_back(new Explore(this));
	analyzers.push_back(new Fight(this));
	analyzers.push_back(new Food(this));
	analyzers.push_back(new Genocide(this));
	analyzers.push_back(new Health(this));
	analyzers.push_back(new Lamp(this));
	analyzers.push_back(new Loot(this));
	analyzers.push_back(new MonsterInfo(this));
	analyzers.push_back(new Passtune(this));
	analyzers.push_back(new Potion(this));
	analyzers.push_back(new Pray(this));
	analyzers.push_back(new Ring(this));
	analyzers.push_back(new Rub(this));
	analyzers.push_back(new Scroll(this));
	analyzers.push_back(new Shop(this));
	analyzers.push_back(new Sokoban(this));
	analyzers.push_back(new Throne(this));
	analyzers.push_back(new Unihorn(this));
	analyzers.push_back(new Valkyrie(this));
	analyzers.push_back(new Vault(this));
	analyzers.push_back(new Wand(this));
	analyzers.push_back(new Weapon(this));
	analyzers.push_back(new Wish(this));

	/* run init in analyzers */
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		(*a)->init();

	/* set best_analyzer */
	best_analyzer = analyzers.end();
}

Saiph::~Saiph() {
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		delete *a;
	Debug::close();
}

/* methods */
bool Saiph::addItemToInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return false;
	Debug::notice(last_turn) << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to inventory slot " << key << endl;
	if (inventory.find(key) != inventory.end()) {
		/* existing item, add amount */
		inventory[key].count += item.count;
	} else {
		/* new item */
		inventory[key] = item;
	}
	return true;
}

unsigned char Saiph::directLine(Point point, bool ignore_sinks, bool ignore_boulders) {
	/* is the target in a direct line from the player? */
	if (point.row < MAP_ROW_BEGIN || point.row > MAP_ROW_END || point.col < MAP_COL_BEGIN || point.col > MAP_COL_END) {
		/* outside map */
		return ILLEGAL_DIRECTION;
	} else if (point == position) {
		/* eh? don't do this */
		return NOWHERE;
	} else if (point.row == position.row) {
		/* aligned horizontally */
		if (point.col > position.col) {
			while (--point.col > position.col) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return E;
		} else {
			while (++point.col < position.col) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return W;
		}
	} else if (point.col == position.col) {
		/* aligned vertically */
		if (point.row > position.row) {
			while (--point.row > position.row) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return S;
		} else {
			while (++point.row < position.row) {
				if (!directLineHelper(point, ignore_sinks, ignore_boulders))
					return ILLEGAL_DIRECTION;
			}
			return N;
		}
	} else if (abs(point.row - position.row) == abs(point.col - position.col)) {
		/* aligned diagonally */
		if (point.row > position.row) {
			if (point.col > position.col) {
				while (--point.row > position.row) {
					--point.col;
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return SE;
			} else {
				while (--point.row > position.row) {
					++point.col;
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return SW;
			}
		} else {
			if (point.col > position.col) {
				while (++point.row < position.row) {
					--point.col;
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return NE;
			} else {
				while (++point.row < position.row) {
					++point.col;
					if (!directLineHelper(point, ignore_sinks, ignore_boulders))
						return ILLEGAL_DIRECTION;
				}
				return NW;
			}
		}
	}
	return ILLEGAL_DIRECTION;
}

const string &Saiph::farlook(const Point &target) {
	/* look at something, eg. monster */
	farlook_command = ";";
	Point cursor = position;
	while (cursor != target) {
		unsigned char move;
		if (cursor.row < target.row && cursor.col < target.col) {
			move = SE;
			++cursor.row;
			++cursor.col;
		} else if (cursor.row < target.row && cursor.col > target.col) {
			move = SW;
			++cursor.row;
			--cursor.col;
		} else if (cursor.row > target.row && cursor.col < target.col) {
			move = NE;
			--cursor.row;
			++cursor.col;
		} else if (cursor.row > target.row && cursor.col > target.col) {
			move = NW;
			--cursor.row;
			--cursor.col;
		} else if (cursor.row < target.row) {
			move = S;
			++cursor.row;
		} else if (cursor.row > target.row) {
			move = N;
			--cursor.row;
		} else if (cursor.col < target.col) {
			move = E;
			++cursor.col;
		} else {
			move = W;
			--cursor.col;
		}
		farlook_command.push_back(move);
	}
	farlook_command.push_back(',');
	return farlook_command;
}

bool Saiph::removeItemFromInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return false;
	map<unsigned char, Item>::iterator i = inventory.find(key);
	if (i == inventory.end())
		return false;
	Debug::notice(last_turn) << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from inventory slot " << key << endl;
	if (i->second.count > item.count)
		i->second.count -= item.count; // we got more than we remove
	else
		inventory.erase(i); // removing all we got
	return true;
}

bool Saiph::run() {
	/* clear pickup list */
	pickup.clear();
	/* and drop list */
	drop.clear();
	/* set on_ground to NULL */
	on_ground = NULL;

	/* check if we're engulfed */
	if (position.row > MAP_ROW_BEGIN && position.row < MAP_ROW_END && position.col > MAP_COL_BEGIN && position.col < MAP_COL_END && World::view[position.row - 1][position.col - 1] == '/' && World::view[position.row - 1][position.col + 1] == '\\' && World::view[position.row + 1][position.col - 1] == '\\' && World::view[position.row + 1][position.col + 1] == '/')
		engulfed = true;
	else
		engulfed = false;

	/* detect player position */
	if (!World::menu && !World::question && !engulfed)
		detectPosition();

	/* global message parsing */
	parseMessages(World::messages);

	/* level message parsing */
	Debug::notice(last_turn) << MESSAGES_DEBUG_NAME << "'" << World::messages << "'" << endl;
	World::levels[position.level].parseMessages(World::messages);

	/* more debugging */
	if (World::question)
		Debug::notice(last_turn) << SAIPH_DEBUG_NAME << "Question asked" << endl;
	if (World::menu)
		Debug::notice(last_turn) << SAIPH_DEBUG_NAME << "Menu shown" << endl;
	if (engulfed)
		Debug::notice(last_turn) << SAIPH_DEBUG_NAME << "Saiph engulfed" << endl;

	/* update level */
	if (!World::menu) {
		if (!engulfed) {
			/* update changed symbols */
			for (vector<Point>::iterator c = World::changes.begin(); c != World::changes.end(); ++c)
				World::levels[position.level].updateMapPoint(*c, (unsigned char) World::view[c->row][c->col], World::color[c->row][c->col]);
			/* update monsters */
			World::levels[position.level].updateMonsters();
			/* update pathmap */
			World::levels[position.level].updatePathMap();
		} else {
			/* we'll still need to update monster's "visible" while engulfed,
			 * or she may attempt to farlook a monster */
			for (map<Point, Monster>::iterator m = World::levels[position.level].monsters.begin(); m != World::levels[position.level].monsters.end(); ++m)
				m->second.visible = false;
		}
	}
	/* print maps so we see what we're doing */
	dumpMaps();

	/* set the on_ground pointer if there's loot here */
	if (World::levels[position.level].stashes.find(position) != World::levels[position.level].stashes.end())
		on_ground = &World::levels[position.level].stashes[position];

	/* unless we're in the middle of an action, let the analyzers figure out what to do */
	Command command = action::Action::noop;
	if (best_analyzer != analyzers.end() && (*best_analyzer)->action != NULL) {
		(*best_analyzer)->action->updateAction(this);
		command = (*best_analyzer)->action->getCommand();
	}
	if (command == action::Action::noop) {
		/* analyzer stuff comes here */
		/* parse messages */
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ) {
			/* parse messages */
			(*a)->parseMessages(World::messages, command);
			Command a_command = (*a)->action == NULL ? action::Action::noop : (*a)->action->getCommand();
			/* set command & best_analyzer if a_command is more urgent */
			if (a_command.priority > command.priority) {
				command = a_command;
				best_analyzer = a;
			}
			++a;
		}

		/* analyze */
		if (!World::question && !World::menu) {
			for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
				/* analyze */
				(*a)->analyze(command);
				Command a_command = (*a)->action == NULL ? action::Action::noop : (*a)->action->getCommand();
				/* set command & best_analyzer if a_command is more urgent */
				if (a_command.priority > command.priority) {
					command = a_command;
					best_analyzer = a;
				}
			}
		}

		/* need to check priority once more, because of events */
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
			Command a_command = (*a)->action == NULL ? action::Action::noop : (*a)->action->getCommand();
			if (a_command.priority > command.priority) {
				command = a_command;
				best_analyzer = a;
			}
		}
	}

	/* check if we got a command */
	if (World::question && command == action::Action::noop) {
		Debug::warning(last_turn) << SAIPH_DEBUG_NAME << "Unhandled question: " << World::messages << endl;
		World::executeCommand(string(1, (char) 27));
		return true;
	} else if (World::menu && command == action::Action::noop) {
		Debug::warning(last_turn) << SAIPH_DEBUG_NAME << "Unhandled menu: " << World::messages << endl;
		World::executeCommand(string(1, (char) 27));
		return true;
	} else if (command == action::Action::noop) {
		Debug::warning(last_turn) << SAIPH_DEBUG_NAME << "I have no idea what to do... Searching 42 times" << endl;
		cout << (unsigned char) 27 << "[1;82H";
		cout << (unsigned char) 27 << "[K"; // erase everything to the right
		cout << "No idea what to do: 42s";
		/* return cursor back to where it was */
		cout << (unsigned char) 27 << "[" << World::cursor.row + 1 << ";" << World::cursor.col + 1 << "H";
		World::executeCommand("42s");
		++internal_turn;
		return true;
	}

	/* print what we're doing */
	cout << (unsigned char) 27 << "[1;82H";
	cout << (unsigned char) 27 << "[K"; // erase everything to the right
	cout << (*best_analyzer)->name << " " << command;
	/* return cursor back to where it was */
	cout << (unsigned char) 27 << "[" << World::cursor.row + 1 << ";" << World::cursor.col + 1 << "H";
	/* and flush cout. if we don't do this our output looks like garbage */
	cout.flush();
	/* let an analyzer do its command */
	Debug::notice(last_turn) << (*best_analyzer)->name << " " << command << endl;
	World::executeCommand(command.command);
	if (stuck_counter % 42 == 41) {
		/* if we send the same command n times and the turn counter doesn't increase, we probably got a problem */
		/* let's see if we're moving somewhere */
		bool was_move = false;
		if (command.command.size() == 1 && last_command == command.command) {
			/* command is movement, and so was last_command.
			 * it's likely that we're moving */
			Point to;
			switch (command.command[0]) {
			case NW:
			case NE:
			case SW:
			case SE:
				/* moving diagonally failed.
				 * we could be trying to move diagonally into a door we're
				 * unaware of because of an item blocking the door symbol.
				 * make the tile UNKNOWN_TILE_DIAGONALLY_UNPASSABLE */
				to = directionToPoint((unsigned char) command.command[0]);
				World::setDungeonSymbol(to, UNKNOWN_TILE_DIAGONALLY_UNPASSABLE);
				was_move = true;
				break;

			case N:
			case E:
			case S:
			case W:
				/* moving cardinally failed, possibly item in wall.
				 * make the tile UNKNOWN_TILE_UNPASSABLE */
				to = directionToPoint((unsigned char) command.command[0]);
				World::setDungeonSymbol(to, UNKNOWN_TILE_UNPASSABLE);
				was_move = true;
				break;
			}
		}
		if (!was_move) {
			/* apparently it wasn't a failed movement,
			 * that means an analyzer is screwing up */
			Debug::warning(last_turn) << SAIPH_DEBUG_NAME << "Command failed for analyzer " << (*best_analyzer)->name << ": " << command << endl;
		}
	} else if (stuck_counter > 1680) {
		/* failed too many times, #quit */
		Debug::error(last_turn) << SAIPH_DEBUG_NAME << "Appear to be stuck, quitting game" << endl;
		World::executeCommand(string(1, (char) 27));
		World::executeCommand(QUIT);
		World::executeCommand(YES);
		return false;
	}
	if (last_turn == World::turn)
		stuck_counter++;
	else
		stuck_counter = 0;
	last_command = command.command;
	last_turn = World::turn;
	if (command.priority <= PRIORITY_MAX)
		++internal_turn;
	return true;
}

/* private methods */
bool Saiph::directLineHelper(const Point &point, bool ignore_sinks, bool ignore_boulders) {
	unsigned char symbol = World::getDungeonSymbol(point);
	if (!Level::passable[symbol] && (!ignore_boulders || symbol != BOULDER))
		return false;
	else if (!ignore_sinks && symbol == SINK)
		return false;
	else if (World::getMonsterSymbol(point) != ILLEGAL_MONSTER && World::levels[position.level].monsters[point].visible)
		return false;
	return true;
}

Point Saiph::directionToPoint(unsigned char direction) {
	/* return the position we'd be at if we do the given move */
	Point pos = position;
	switch (direction) {
		case NW:
			--pos.row;
			--pos.col;
			break;

		case N:
			--pos.row;
			break;

		case NE:
			--pos.row;
			++pos.col;
			break;

		case E:
			++pos.col;
			break;

		case SE:
			++pos.row;
			++pos.col;
			break;

		case S:
			++pos.row;
			break;

		case SW:
			++pos.row;
			--pos.col;
			break;

		case W:
			--pos.col;
			break;
	}
	if (pos.row >= MAP_ROW_BEGIN && pos.row <= MAP_ROW_END && pos.col >= MAP_COL_BEGIN && pos.col <= MAP_COL_END)
		return pos;
	else
		return position;
}

void Saiph::dumpMaps() {
	/* XXX: World echoes output from the game in the top left corner */
	/* commands/frames/turns per second */
	int seconds = (int) difftime(time(NULL), start_time);
	if (seconds == 0)
		++seconds;
	int cps = World::command_count / seconds;
	int fps = World::frame_count / seconds;
	int tps = World::turn / seconds;
	cout << (unsigned char) 27 << "[25;1H";
	cout << "CPS/FPS/TPS: ";
	cout << (unsigned char) 27 << "[34m" << cps << (unsigned char) 27 << "[0m/";
	cout << (unsigned char) 27 << "[35m" << fps << (unsigned char) 27 << "[0m/";
	cout << (unsigned char) 27 << "[36m" << tps << (unsigned char) 27 << "[0m      ";

	/* monsters and map as saiph sees it */
	Point p;
	for (p.row = MAP_ROW_BEGIN; p.row <= MAP_ROW_END; ++p.row) {
		cout << (unsigned char) 27 << "[" << p.row + 26 << ";2H";
		for (p.col = MAP_COL_BEGIN; p.col <= MAP_COL_END; ++p.col) {
			unsigned char monster = World::getMonsterSymbol(p);
			if (p.row == position.row && p.col == position.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (monster != ILLEGAL_MONSTER)
				cout << monster;
			else
				cout << World::getDungeonSymbol(p);
		}
	}

	/* path map */
	/*
	for (p.row = MAP_ROW_BEGIN; p.row <= MAP_ROW_END; ++p.row) {
		cout << (unsigned char) 27 << "[" << p.row + 26 << ";2H";
		for (p.col = MAP_COL_BEGIN; p.col <= MAP_COL_END; ++p.col) {
			if (p.row == postion.row && p.col == position.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (World::levels[position.level].pathmap[p.row][p.col].dir != ILLEGAL_DIRECTION)
				//cout << (unsigned char) World::levels[position.level].pathmap[p.row][p.col].dir;
				cout << (char) (World::levels[position.level].pathmap[p.row][p.col].cost % 64 + 48);
			else
				cout << World::getDungeonSymbol(p);
		}
	}
	*/

	/* status & inventory */
	cout << (unsigned char) 27 << "[2;82H";
	if (intrinsics & PROPERTY_COLD)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[34m" << "Cold " << (unsigned char) 27 << "[m";
	if (intrinsics & PROPERTY_DISINT)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[35m" << "DisInt " << (unsigned char) 27 << "[m";
	if (intrinsics & PROPERTY_FIRE)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[31m" << "Fire " << (unsigned char) 27 << "[m";
	if (intrinsics & PROPERTY_POISON)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[32m" << "Poison " << (unsigned char) 27 << "[m";
	if (intrinsics & PROPERTY_SHOCK)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[36m" << "Shock " << (unsigned char) 27 << "[m";
	if (intrinsics & PROPERTY_SLEEP)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[33m" << "Sleep " << (unsigned char) 27 << "[m";

	cout << (unsigned char) 27 << "[3;82H";
	if (intrinsics & PROPERTY_ESP)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[35m" << "ESP " << (unsigned char) 27 << "[m";
	if (intrinsics & PROPERTY_TELEPORT_CONTROL)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[36m" << "TeleCon " << (unsigned char) 27 << "[m";
	if (intrinsics & PROPERTY_TELEPORT)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[33m" << "Teleport " << (unsigned char) 27 << "[m";
	if (lycanthropy)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[31m" << "Lycan " << (unsigned char) 27 << "[m";
	if (hurt_leg)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[34m" << "Leg " << (unsigned char) 27 << "[m";
	if (polymorphed)
		cout << (unsigned char) 27 << "[1m" << (unsigned char) 27 << "[32m" << "Poly " << (unsigned char) 27 << "[m";

	int ir = 0;
	for (map<unsigned char, Item>::iterator i = inventory.begin(); i != inventory.end() && ir < 46; ++i) {
		cout << (unsigned char) 27 << "[" << (4 + ir) << ";82H";
		cout << (unsigned char) 27 << "[K"; // erase everything to the right
		if (i->second.beatitude == BLESSED)
			cout << (unsigned char) 27 << "[32m";
		else if (i->second.beatitude == CURSED)
			cout << (unsigned char) 27 << "[31m";
		else if (i->second.beatitude == UNCURSED)
			cout << (unsigned char) 27 << "[33m";
		cout << i->first;
		cout << " - " << i->second;
		cout << (unsigned char) 27 << "[m";
		++ir;
	}
	for (; ir < 46; ++ir) {
		cout << (unsigned char) 27 << "[" << (5 + ir) << ";82H";
		cout << (unsigned char) 27 << "[K"; // erase everything to the right
	}
}

void Saiph::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_FOR_INSTRUCTIONS, 0) != string::npos) {
		/* a bit unique case, this is a question.
		 * the data doesn't end with the sequence we check in World */
		World::question = true;
	}
	if (messages.find(SAIPH_GAIN_COLD_RES1, 0) != string::npos)
		intrinsics |= PROPERTY_COLD;
	if (messages.find(SAIPH_LOSE_COLD_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_COLD;
	if (messages.find(SAIPH_GAIN_DISINTEGRATION_RES1, 0) != string::npos || messages.find(SAIPH_GAIN_DISINTEGRATION_RES2, 0) != string::npos)
		intrinsics |= PROPERTY_DISINT;
	if (messages.find(SAIPH_GAIN_FIRE_RES1, 0) != string::npos || messages.find(SAIPH_GAIN_FIRE_RES2, 0) != string::npos)
		intrinsics |= PROPERTY_FIRE;
	if (messages.find(SAIPH_LOSE_FIRE_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_FIRE;
	if (messages.find(SAIPH_GAIN_POISON_RES1, 0) != string::npos || messages.find(SAIPH_GAIN_POISON_RES2, 0) != string::npos)
		intrinsics |= PROPERTY_POISON;
	if (messages.find(SAIPH_LOSE_POISON_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_POISON;
	if (messages.find(SAIPH_GAIN_SHOCK_RES1, 0) != string::npos || messages.find(SAIPH_GAIN_SHOCK_RES2, 0) != string::npos)
		intrinsics |= PROPERTY_SHOCK;
	if (messages.find(SAIPH_LOSE_SHOCK_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_SHOCK;
	if (messages.find(SAIPH_GAIN_SLEEP_RES1, 0) != string::npos)
		intrinsics |= PROPERTY_SLEEP;
	if (messages.find(SAIPH_LOSE_SLEEP_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_SLEEP;
	if (messages.find(SAIPH_GAIN_TELEPATHY1, 0) != string::npos)
		intrinsics |= PROPERTY_ESP;
	if (messages.find(SAIPH_LOSE_TELEPATHY1, 0) != string::npos)
		intrinsics &= ~PROPERTY_ESP;
	if (messages.find(SAIPH_GAIN_TELEPORT_CONTROL1, 0) != string::npos || messages.find(SAIPH_GAIN_TELEPORT_CONTROL2, 0) != string::npos)
		intrinsics |= PROPERTY_TELEPORT_CONTROL;
	if (messages.find(SAIPH_GAIN_TELEPORTITIS1, 0) != string::npos || messages.find(SAIPH_GAIN_TELEPORTITIS2, 0) != string::npos)
		intrinsics |= PROPERTY_TELEPORT;
	if (messages.find(SAIPH_LOSE_TELEPORTITIS1, 0) != string::npos)
		intrinsics &= ~PROPERTY_TELEPORT;
	if (messages.find(SAIPH_FEEL_PURIFIED, 0) != string::npos)
		lycanthropy = false;
	if (messages.find(SAIPH_FEEL_FEVERISH, 0) != string::npos)
		lycanthropy = true;
	if (messages.find(SAIPH_HURT_LEFT_LEG, 0) != string::npos || messages.find(SAIPH_HURT_RIGHT_LEG, 0) != string::npos)
		hurt_leg = true;
	if (messages.find(SAIPH_LEG_IS_BETTER, 0) != string::npos)
		hurt_leg = false;
	if (messages.find(SAIPH_POLYMORPH, 0) != string::npos)
		polymorphed = true;
	if (messages.find(SAIPH_UNPOLYMORPH, 0) != string::npos)
		polymorphed = false;
	if (messages.find(SAIPH_BEGIN_LEVITATION, 0) != string::npos || messages.find(SAIPH_BEGIN_LEVITATION_PIT, 0) != string::npos)
		levitating = true;
	if (messages.find(SAIPH_END_LEVITATION, 0) != string::npos || messages.find(SAIPH_END_LEVITATION_SINK, 0) != string::npos)
		levitating = false;
}

string dirname(const string &path) {
	size_t offset = path.find_last_of("/\\");

	if (offset != string::npos) {
		return path.substr(0, offset);
	} else {
		return "./";
	}
}

void usage(const string &executable) {
	cout << "Usage: " << executable << " [-l|-t] [-L <logfile>]" << endl;
	cout << endl;
	cout << "\t-l  Use local nethack executable" << endl;
	cout << "\t-t  Use telnet nethack server" << endl;
	cout << endl;
	cout << "\t-L <logfile>  Log file to write Saiph output" << endl;
}

/* main */
int main(int argc, const char *argv[]) {
	int connection_type = CONNECTION_TELNET;
	string logfile = "saiph.log";

	bool showUsage = false;
	if (argc > 1) {
		for (int a = 1; a < argc; ++a) {
			if (strlen(argv[a]) < 2) {
				showUsage = true;
				continue;
			}

			if (argv[a][0] == '-') {
				switch (argv[a][1]) {
				case 'h':
					showUsage = true;
					break;
				case 'l':
					connection_type = CONNECTION_LOCAL;
					break;
				case 't':
					connection_type = CONNECTION_TELNET;
					break;
				case 'L':
					if (argc > ++a)
						logfile = argv[a];
					else
						showUsage = true;
					break;
				default:
					cout << "Invalid argument " << argv[a] << endl;
					showUsage = true;
					break;
				}
			} else {
				cout << "Unknown argument specified." << endl;
			}
		}

		if (showUsage) {
			usage(argv[0]);
			return 1;
		}
	}

	Debug::open(logfile);
	data::Monster::init();
	data::Item::init();
	World::init(connection_type);
	Saiph *saiph = new Saiph(); //dirname(argv[0]), connection_type);
	//for (int a = 0; a < 200 && saiph->run(); ++a)
	//	;
	while (saiph->run())
		;
	Debug::notice() << SAIPH_DEBUG_NAME << "Quitting gracefully" << endl;
	delete saiph;
	World::destroy();
	data::Item::destroy();
	data::Monster::destroy();
	Debug::close();
}

#ifndef ACTION_LOOT_H
#define ACTION_LOOT_H

#include <map>
#include <queue>
#include <sstream>
#include <vector>
#include "EventBus.h"
#include "Inventory.h"
#include "Item.h"
#include "World.h"
#include "Actions/Action.h"
#include "Events/WantItems.h"

namespace action {

	class Loot : public Action {
	public:
		static const int ID;

		Loot(analyzer::Analyzer* analyzer, int priority, bool safe_stash) : Action(analyzer), _loot(",", priority), _close_page(CLOSE_PAGE, PRIORITY_CONTINUE_ACTION), _look(":", PRIORITY_LOOK), _safe_stash(safe_stash), _keys() {
		}

		virtual ~Loot() {
		}

		static bool canLoot() {
			std::map<Point, int>::const_iterator s = World::level().symbols(TRAP).find(Saiph::position());
			return s == World::level().symbols(TRAP).end() || (s->second != TRAP_PIT && s->second != TRAP_SPIKED_PIT);
		}

		virtual int id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _loot;

			case 1:
				return _loot_item;

			case 2:
				return _close_page;

			case 3:
				return _look;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string& messages) {
			std::string::size_type pos = std::string::npos;
			if (_sequence == 0) {
				if ((pos = messages.find(MESSAGE_PICK_UP_WHAT)) != std::string::npos) {
					/* looting multiple items */
					_sequence = 1;
				} else {
					/* looted 1 item */
					_sequence = 3;
				}
			} else if (_sequence == 1 && _keys.size() <= 0) {
				/* all the items we want on this page has been selected for looting, close page */
				_sequence = 2;
			} else if (_sequence == 2) {
				if (World::menu()) {
					/* more than 1 page */
					_sequence = 1;
					pos = 0;
				} else {
					/* all pages shown */
					_sequence = 3;
				}
			} else if (_sequence == 3) {
				/* looked at ground */
				_sequence = 4;
			}
			if (_sequence == 1 && _keys.size() <= 0) {
				/* figure out which items we would like to loot */
				event::WantItems wi(false, _safe_stash);
				while (pos != std::string::npos && messages.size() > pos + 6) {
					pos += 6;
					std::string::size_type length = messages.find("  ", pos);
					if (length == std::string::npos)
						break;
					length = length - pos;
					if (messages[pos - 2] == '-')
						wi.addItem(messages[pos - 4], Item(messages.substr(pos, length), 0));
					pos += length;
				}
				/* broadcast event */
				EventBus::broadcast(static_cast<event::Event*> (&wi));
				/* pick up stuff that was wanted by analyzers */
				std::ostringstream tmp;
				for (std::map<unsigned char, Item>::iterator i = wi.items().begin(); i != wi.items().end(); ++i) {
					if (i->second.want() <= 0)
						continue;
					tmp.str("");
					if (i->second.want() >= i->second.count())
						tmp << i->first;
					else
						tmp << i->second.want() << i->first;
					_keys.push(tmp.str());
				}
			}
			if (_sequence == 1) {
				if (_keys.size() > 0) {
					/* make _loot_item the first item in our list of stuff to pick up */
					_loot_item = Command(_keys.front(), PRIORITY_SELECT_ITEM);
					_keys.pop();
				} else {
					/* we shouldn't get here, but apparently we did. close page */
					_sequence = 2;
				}
			}
		}

	private:
		const Command _loot;
		const Command _close_page;
		const Command _look;
		const bool _safe_stash;
		Command _loot_item;
		std::queue<std::string> _keys;
	};
}
#endif

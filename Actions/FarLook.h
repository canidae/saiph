#ifndef ACTION_FAR_LOOK_H
#define	ACTION_FAR_LOOK_H

#include "Action.h"
#include "../Globals.h"

namespace action {

	class FarLook : public Action {
	public:
		static const int ID;

		FarLook(analyzer::Analyzer* analyzer, const Point& location) : Action(analyzer), _far_look(FarLook::farLook(location), PRIORITY_LOOK) {
		}

		virtual ~FarLook() {
		}

		static std::string farLook(const Point& location) {
			std::string farlook_command = ";";
			Point cursor = Saiph::position();
			while (cursor != location) {
				if (cursor.row() < location.row() && cursor.col() < location.col()) {
					cursor.moveSoutheast();
					farlook_command.push_back(SE);
				} else if (cursor.row() < location.row() && cursor.col() > location.col()) {
					cursor.moveSouthwest();
					farlook_command.push_back(SW);
				} else if (cursor.row() > location.row() && cursor.col() < location.col()) {
					cursor.moveNortheast();
					farlook_command.push_back(NE);
				} else if (cursor.row() > location.row() && cursor.col() > location.col()) {
					cursor.moveNorthwest();
					farlook_command.push_back(NW);
				} else if (cursor.row() < location.row()) {
					cursor.moveSouth();
					farlook_command.push_back(S);
				} else if (cursor.row() > location.row()) {
					cursor.moveNorth();
					farlook_command.push_back(N);
				} else if (cursor.col() < location.col()) {
					cursor.moveEast();
					farlook_command.push_back(E);
				} else {
					cursor.moveWest();
					farlook_command.push_back(W);
				}
			}
			farlook_command.push_back(',');
			return farlook_command;
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Command& command() {
			switch (_sequence) {
			case 0:
				return _far_look;

			default:
				return Action::NOOP;
			}
		}

		virtual void update(const std::string&) {
			++_sequence;
		}

	private:
		const Command _far_look;
	};
}
#endif

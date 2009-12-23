#ifndef ACTION_FAR_LOOK_H
#define	ACTION_FAR_LOOK_H

#include "Globals.h"
#include "Actions/Action.h"

namespace action {

	class FarLook : public Action {
	public:
		static const int ID;

		FarLook(analyzer::Analyzer* analyzer, const Point& location) : Action(analyzer, false), _far_look(";" + World::cursorMoves(Saiph::position(), location) + ".", PRIORITY_LOOK) {
		}

		virtual ~FarLook() {
		}

		virtual int id() {
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
			if (_sequence == 0)
				_sequence = 1;
		}

	private:
		const Command _far_look;
	};
}
#endif

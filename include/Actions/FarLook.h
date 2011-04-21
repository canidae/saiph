#ifndef ACTION_FAR_LOOK_H
#define	ACTION_FAR_LOOK_H

#include "Globals.h"
#include "Point.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Action.h"

namespace action {
	class FarLook : public Action {
	public:
		static const int ID;

		FarLook(analyzer::Analyzer* analyzer, const Point& location);
		virtual ~FarLook();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		const Command _far_look;
	};
}
#endif

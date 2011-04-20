#ifndef EVENT_SHOP_DETECTED_H
#define EVENT_SHOP_DETECTED_H

#include "Events/Event.h"
#include "Point.h"

namespace event {

	class ShopDetected : public Event {
	public:
		static const int ID;

		ShopDetected(Point ul, Point lr) : Event("ShopDetected"), _ul(ul), _lr(lr) { }

		virtual ~ShopDetected() { }

		virtual int id() { return ID; }

		const Point& ul() const { return _ul; }
		const Point& lr() const { return _lr; }

	private:
		Point _ul, _lr;
	};
}
#endif

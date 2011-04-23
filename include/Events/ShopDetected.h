#ifndef EVENT_SHOP_DETECTED_H
#define EVENT_SHOP_DETECTED_H

#include "Point.h"
#include "Events/Event.h"

namespace event {
	class ShopDetected : public Event {
	public:
		static const int ID;

		ShopDetected(const Point& ul, const Point& lr);
		virtual ~ShopDetected();

		virtual int id();
		const Point& ul() const;
		const Point& lr() const;

	private:
		Point _ul;
		Point _lr;
	};
}
#endif

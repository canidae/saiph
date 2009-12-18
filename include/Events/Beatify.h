#ifndef EVENT_BEATIFY_H
#define	EVENT_BEATIFY_H

namespace event {

	class Beatify : public Event {
	public:
		static const int ID;

		Beatify(unsigned char key, int priority) : Event("Beatify"), _key(key), _priority(priority) {
		}

		virtual ~Beatify() {
		}

		virtual int id() {
			return ID;
		}

		virtual unsigned char key() {
			return _key;
		}

		virtual int priority() {
			return _priority;
		}

	private:
		const unsigned char _key;
		const int _priority;
	};
}
#endif

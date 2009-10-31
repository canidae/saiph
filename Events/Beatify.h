#ifndef EVENT_BEATIFY_H
#define	EVENT_BEATIFY_H

namespace event {

	class Beatify : public Event {
	public:
		static const int ID;

		Beatify(const unsigned char& key, const int& priority) : Event("Beatify"), _key(key), _priority(priority) {
		}

		virtual ~Beatify() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const unsigned char& key() {
			return _key;
		}

		virtual const int& priority() {
			return _priority;
		}

	private:
		const unsigned char _key;
		const int _priority;
	};
}
#endif

#ifndef FILTER_H
#define FILTER_H

/* forward declare */
class Filter;

using namespace std;

/* a filter gets a position in the map and can
 * make decisions based upon that */
class Filter {
	public:
		/* variables */
		int type;

		/* constructors */
		Filter();

		/* destructors */
		virtual ~Filter();

		/* methods */
		virtual void filter(int row, int col) const = 0;
};

#endif

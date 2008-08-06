#ifndef CONNECTION_H
/* defines */
#define CONNECTION_H
/* connection interfaces */
#define CONNECTION_LOCAL 1
#define CONNECTION_TELNET 2

/* forward declare */
class Connection;

/* includes */
#include <fstream>
#include <string>

/* namespace */
using namespace std;

/* connection handles communication with a local/remote game */
class Connection {
	public:
		/* constructors */
		Connection(ofstream *debugfile);

		/* destructor */
		virtual ~Connection();

		/* static methods */
		static Connection *create(int interface, ofstream *debugfile);

		/* methods */
		virtual int retrieve(char *buffer, int count);
		virtual int transmit(const string &data);
		virtual void start();
		virtual void stop();

	protected:
		/* variables */
		ofstream *debugfile;
};
#endif

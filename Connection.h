#ifndef CONNECTION_H
#define CONNECTION_H
/* connection interfaces */
#define CONNECTION_LOCAL 1
#define CONNECTION_TELNET 2

#include <fstream>
#include <string>

class Connection {
	public:
		Connection(std::ofstream *debugfile);
		virtual ~Connection();

		static Connection *create(int interface, std::ofstream *debugfile);

		virtual int retrieve(char *buffer, int count);
		virtual int transmit(const std::string &data);
		virtual void start();
		virtual void stop();

	protected:
		std::ofstream *debugfile;
};
#endif

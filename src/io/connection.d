module io.connection;

abstract class Connection {
	public:
		abstract void connect();

		abstract void disconnect();

		abstract void send(char[] data);

		abstract char[] receive();
}

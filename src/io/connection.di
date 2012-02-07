module io.connection;

interface Connection {
	void connect();
	void send(char[] data);
	char[] receive(char[] inputEndsWith = []);
	void disconnect();
}

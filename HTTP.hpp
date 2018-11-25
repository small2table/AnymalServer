/*
	HTTP.hpp
	- HTTP Parser

	Author @ Juan Lee (juanlee@kaist.ac.kr)
*/

#include <string>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>

namespace anymal {

class HTTP{
public:
	class HTTPPacket{
	private:
		std::string command;
		std::string path;
		std::string version;
		std::map<std::string, std::string> headers;
		std::string body;

	public:
		HTTPPacket();

		void setCommand(std::string command);
		std::string getCommand();

		void setPath(std::string path);
		std::string getPath();	

		void setVersion(std::string version);
		std::string getVersion();	

		void setOption(std::string key, std::string value);
		std::string getOption(std::string key);

		void setBody(std::string body);
		std::string getBody();

		std::string to_string();
	};

public:
	static void HTTP_send(int clientfd, HTTPPacket *packet);
	static HTTPPacket* HTTP_receive(int clientfd);
};

}
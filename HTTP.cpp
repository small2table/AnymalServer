#include "HTTP.hpp"

#include <iostream>
using namespace std;

namespace anymal {

//-------------------- Constructor --------------------//

HTTP::HTTPPacket::HTTPPacket(){
	command = "GET";
	path = "/";
	version = "HTTP/1.0";
	body = "";
}

//-------------------- set & get --------------------//

void HTTP::HTTPPacket::setCommand(std::string command){
	this->command = command;
}

std::string HTTP::HTTPPacket::getCommand(){
	return command;
}

void HTTP::HTTPPacket::setPath(std::string path){
	this->path = path;
}

std::string HTTP::HTTPPacket::getPath(){
	return path;
}

void HTTP::HTTPPacket::setVersion(std::string version){
	this->version = version;
}

std::string HTTP::HTTPPacket::getVersion(){
	return version;
}

void HTTP::HTTPPacket::setOption(std::string key, std::string value){
	headers[key] = value;
}

std::string HTTP::HTTPPacket::getOption(std::string key){
	return headers[key];
}

void HTTP::HTTPPacket::setBody(std::string body){
	this->body = body;
	setOption("Content-Length", std::to_string(body.length()));
	setOption("Content-Type", "text");
}

std::string HTTP::HTTPPacket::getBody(){
	return body;
}

std::string HTTP::HTTPPacket::to_string(){
	std::string packet = command + " " + path + " " + version + "\n";
	for(auto iter = headers.begin(); iter != headers.end(); iter++){
		packet += (iter->first + ":" + iter->second + "\n");
	}
	packet = packet.substr(0, packet.length()-1);
	packet += "\r\n\r\n";
	packet += body;

	return packet;
}

//-------------------- HTTP --------------------//

void HTTP::HTTP_send(int clientfd, HTTP::HTTPPacket *packet){
	std::string form = packet->to_string();
	write(clientfd, form.c_str(), form.length());

	cout << form.c_str() << endl;
}

HTTP::HTTPPacket* HTTP::HTTP_receive(int clientfd){
	std::string packet;

	char ch;
	while(true){
		read(clientfd, &ch, 1);
		packet += ch;

		if(packet.find("\r\n\r\n") != std::string::npos){
			break;
		}
	}

	std::string command;
	HTTP::HTTPPacket *form = new HTTP::HTTPPacket;
	form->setOption("Content-Length", "0");

	int last_pos = 0;
	int pos = packet.find_first_of("\n");
	command = packet.substr(last_pos, pos - last_pos);

	int sep1 = command.find_first_of(" ");
	int sep2 = command.find_first_of(" ", sep1+1);

	last_pos = packet.find_first_not_of("\n", pos);
	pos = packet.find_first_of("\n", last_pos);
	while(std::string::npos != pos){
		std::string found = packet.substr(last_pos, pos - last_pos);

		int seperator = found.find_first_of(":");
		if(seperator == std::string::npos) break;
		form->setOption(found.substr(0, seperator), found.substr(seperator+1));

		last_pos = packet.find_first_not_of("\n", pos);
		pos = packet.find_first_of("\n", last_pos);
	}

	form->setCommand(command.substr(0, sep1));
	form->setPath(command.substr(sep1+1, sep2 - sep1 - 1));
	
	int length = std::stoi(form->getOption("Content-Length"));
	if(length != 0){
		char* body = new char[length];
		read(clientfd, body, length);
		std::string str_body(body);
		delete[] body;

		form->setBody(str_body);
	}
	return form;
}

}
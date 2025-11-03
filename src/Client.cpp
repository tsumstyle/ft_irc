/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:04:26 by aroux             #+#    #+#             */
/*   Updated: 2025/11/03 12:46:27 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

// constructors
Client::Client() : _nick(""), _user(""), _state(NEW), _socket(-1) {}

Client::Client(int client_socket) : _nick(""), 
									_user(""), 
									_state(NEW), 
									_socket(client_socket),
									_buffer("") {}

Client::Client(const Client& copy) : _nick(copy._nick),
									 _user(copy._user),
									 _state(copy._state),
									 _socket(copy._socket),
									 _buffer(copy._buffer) {}

Client::~Client() {}

// assignment operator
Client&	Client::operator=(const Client& other) {
	if (this != &other) {
		_nick = other._nick;
		_user = other._user;
		_state = other._state;
		_socket	= other._socket;
		_channels = other._channels;
		_buffer = other._buffer;
	}
	return *this;
}

// getters
int	Client::getSocket() const { return _socket; }

std::string	Client::getNick() const { 
	if (_nick.empty())
		return "*";
	return _nick; 
}
		
const std::string&	Client::getUser() const { return _user; }
clientState	Client::getState() const { return _state; }
std::string	Client::getSource() const { return _nick + "!" + _user + "@" + SERVER_NAME; }
std::vector<Channel*>	Client::getChannels() const { return _channels; }

// setters
void	Client::setNick(const std::string& nick) { _nick = nick; }
void	Client::setUser(const std::string& user) { _user = user; }
void	Client::setState(clientState state) { _state = state; }

//other member functions
void	Client::sendMessage(const std::string& msg) const {
	if (_socket != -1) {
		::send(_socket, msg.c_str(), msg.size(), 0);
	}
}

void	Client::addChannel(Channel* channel) {
	if (std::find(_channels.begin(), _channels.end(), channel) == _channels.end()) {
		_channels.push_back(channel);
	}
}

void	Client::removeChannel(Channel* channel) {
	std::vector<Channel*>::iterator it = std::find(_channels.begin(), _channels.end(), channel);
	if (it != _channels.end()) {
		_channels.erase(it);
	}
}

bool	Client::isOnChannel(Channel* channel) {
	if (std::find(_channels.begin(), _channels.end(), channel) == _channels.end())
		return false;
	else
		return true;
}

int	Client::appendBuffer(const std::string& msg) {
	if (msg.size() > MAX_MESSAGE_SIZE) {
		sendMessage("ERROR :Message too long\r\n");
		return 1;
	}
	if (_buffer.size() + msg.size() > MAX_BUFFER_SIZE) {
		_buffer.clear();
		sendMessage("ERROR :Input line too long- max is " + toString(MAX_MESSAGE_SIZE) + " bytes. You will be disconnected.\r\n");
		serverLog(this, " ERROR: Buffer overflow detected. Disconnecting client...");
		setState(DISCONNECTED);
		return 0;
	}
	_buffer += msg;
	return 1;
}

void	Client::clearBuffer() { _buffer.clear(); }

const std::string&	Client::getBuffer() { return _buffer; }

bool	Client::hasFullMessage() const { return _buffer.find("\r\n") != std::string::npos; }

std::string	Client::getMessage() {
	size_t pos = _buffer.find("\r\n");
	if (pos != std::string::npos)	{
		std::string message = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 2);
		return message;
	}
	pos = _buffer.find('\n');
	if (pos != std::string::npos) {
		std::string message = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 1);
		return message;
	}
	return "";
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:04:26 by aroux             #+#    #+#             */
/*   Updated: 2025/10/06 13:48:43 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

// constructors
Client::Client() : _nick(""), _user(""), _state(NEW), _socket(-1) {}

Client::Client(int client_socket) : _nick(""), 
									_user(""), 
									_state(NEW), 
									_socket(client_socket) {}

Client::Client(const Client& copy) : _nick(copy._nick),
									 _user(copy._user),
									 _state(copy._state),
									 _socket(copy._socket)	{}

Client::~Client() {}

// assignment operator
Client&	Client::operator=(const Client& other) {
	if (this != &other) {
		_nick = other._nick;
		_user = other._user;
		_state = other._state;
		_socket	= other._socket;
		_channels = other._channels;
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
	if (std::find(_channels.begin(), _channels.end(), channel) == _channels.end()) {		// check that client isn't already in this channel
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
	if (std::find(_channels.begin(), _channels.end(), channel) == _channels.end())		// if client is not on channel
		return false;
	else
		return true;
}
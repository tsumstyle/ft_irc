/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:04:26 by aroux             #+#    #+#             */
/*   Updated: 2025/09/13 15:29:22 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

// constructors
Client::Client() : _nick(""), _user(""), _state(NOT_REGISTERED), _socket(-1) {}

Client::Client(int client_socket) : _nick(""), 
									_user(""), 
									_state(NOT_REGISTERED), 
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
const std::string&	Client::getNick() const { return _nick; }
const std::string&	Client::getUser() const { return _user; }
clientState	Client::getState() const { return _state; }

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


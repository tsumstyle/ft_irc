/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:06:42 by nboer             #+#    #+#             */
/*   Updated: 2025/09/08 16:50:03 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Represents each connected user

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include "Channel.hpp"

class Client {
private:
	std::string	_nick;
	std::string	_user;
	bool		_state;
	int			_socket;
	std::vector<Channel>	channels; // channels it is part of/ or in the format of std::vector<bool> ?

public:
// constructors
	Client();
	Client(int client_socket);
	Client(const Client& copy);
	~Client();
	Client& operator=(const Client& other);
		// create socket
		// connect
		// send data
		// receive data
		// disconnect

};

Client::Client() {}

Client::Client(int client_socket) : _socket(client_socket) {}

Client::Client(const Client& copy) : _nick(copy._nick),
									 _user(copy._user),
									 _state(copy._state),
									 _socket(copy._socket)	{}

Client::~Client() {}

Client&	Client::operator=(const Client& other) {
	if (this != &other) {
		_nick = other._nick;
		_user = other._user;
		_state = other._state;
		_socket	= other._socket;
	}
	return *this;
}

#endif

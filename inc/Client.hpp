/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:06:42 by nboer             #+#    #+#             */
/*   Updated: 2025/09/10 14:10:18 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Represents each connected user

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <sys/socket.h>		// socket()
#include <vector>

enum	clientState {
	NOT_REGISTERED,		// just accepted by server, no NICK or USER yet
	PARTIAL_REGISTERED,	// sent NICK but not USER
	REGISTERED,			// NICK+USER set, can join channel 
	DISCONNECTED,
};

class Channel;

class Client {
private:
	std::string	_nick;
	std::string	_user;
	clientState	_state;
	int			_socket;
	std::vector<Channel*>	_channels; // channels it is part of/ or in the format of std::vector<bool> ?

public:
// constructors
	Client();
	Client(int client_socket);
	Client(const Client& copy);
	~Client();
	Client& operator=(const Client& other);
	
// getters and setters
	int					getSocket() const;
	const std::string&	getNick() const;
	const std::string&	getUser() const;
	clientState			getState() const;
	void				setNick(const std::string& nick);
	void				setUser(const std::string& user);
	void				setState(clientState state);

	
		// create socket
		// connect
		// send data
		// receive data
		// disconnect

};

#endif

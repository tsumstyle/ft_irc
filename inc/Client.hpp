/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:06:42 by nboer             #+#    #+#             */
/*   Updated: 2025/09/14 17:32:06 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Represents each connected user

#pragma	once

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
	std::vector<Channel*>	_channels;

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

// other member functions	
	void	sendMessage(const std::string& msg) const;	// send data
	// addChannel: becomes part of this channel, and channel is part of the Client's list of channels
	// removeChannel: is removed from this channel, and channel is remove from the client's list of channels

};


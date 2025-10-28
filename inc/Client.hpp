/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:06:42 by nboer             #+#    #+#             */
/*   Updated: 2025/10/27 11:55:55 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Represents each connected user

#pragma	once

#include <iostream>
#include <sys/socket.h>		// socket()
#include <vector>
#include <algorithm>
#include "Channel.hpp"

#ifndef	SERVER_NAME
	# define SERVER_NAME "InstantRegretChat"
#endif

#ifndef MAX_MESSAGE_SIZE
	# define MAX_MESSAGE_SIZE 512
#endif

#ifndef MAX_BUFFER_SIZE
	# define MAX_BUFFER_SIZE 1024
#endif

enum	clientState {
	NEW,			// just accepted by server, no PASS, NICK or USER yet
	PASS_OK,
	NICK_OK,		// sent NICK but not USER
	USERNAME_OK,	// sent USER but not NCIK
	REGISTERED,		// NICK+USER set, can join channel and do other stuff
	DISCONNECTED,	// to be cleaned from the server on next loop
};

class Channel;

class Client {
private:
	std::string	_nick;
	std::string	_user;
	clientState	_state;
	int			_socket;
	std::vector<Channel*>	_channels;
	std::string	_buffer;

public:
// constructors
	Client();
	Client(int client_socket);
	Client(const Client& copy);
	~Client();
	Client& operator=(const Client& other);
	
// getters and setters
	int						getSocket() const;
	std::string				getNick() const;
	const std::string&		getUser() const;
	clientState				getState() const;
	std::string				getSource() const;
	std::vector<Channel*>	getChannels() const;
	void				setNick(const std::string& nick);
	void				setUser(const std::string& user);
	void				setState(clientState state);

// other member functions	
	void	sendMessage(const std::string& msg) const;
	void	addChannel(Channel* channel);
	void	removeChannel(Channel* channel);
	bool	isOnChannel(Channel* channel);

// handle client buffer
	int			appendBuffer(const std::string& bytesReceived);
	void		clearBuffer();
	const		std::string& getBuffer();
	bool		hasFullMessage() const;
	std::string	getMessage();
};


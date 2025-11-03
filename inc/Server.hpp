/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:32:51 by aroux             #+#    #+#             */
/*   Updated: 2025/11/03 11:17:45 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

#include <iostream>
#include <map>
#include <cerrno>			// errno
#include <cstdlib>			// exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <csignal>			// signal()
#include <errno.h>			// not sure if we're allowed to use it
#include <poll.h>
#include <unistd.h>			// read(), open(), close()
#include <sys/socket.h>		// socket()
#include <netinet/in.h>		// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))
#include <fcntl.h>			// fcntl() and O_NONBLOCK
#include "Client.hpp"
#include "Channel.hpp"
#include "utilities.hpp"
#include "parser.hpp"
#include "colors.hpp"

#ifndef	SERVER_NAME
	#define SERVER_NAME "InstantRegretChat"
#endif

struct ParsedCmd;

class Server {
private:
	bool							_running;
	static Server*					_instance;
	int								_port;
	std::string						_server_pass;
	int								_server_socket;
	std::vector<pollfd>				_fds;
	std::map<int, Client*>			_connected;	
	std::map<std::string, Channel>	_channels;

public:
// constructors
	Server();
	Server(const int& port, const std::string& password);
	~Server();
	Server(const Server& copy);
	Server& operator=(const Server& other);

// setters & getters
	void				setPass(const std::string& pass) {_server_pass = pass;};
	const std::string	getPass() const {return _server_pass;};
	
// member functions
	void		start();
	void		run();
	void		acceptClient();
	void		handleClient(int fd);
	bool		isNickTaken(const std::string& nick);
	bool		isValidNick(const std::string& str);
	bool		isValidChar(const char c);
	bool		isValidUsername(const std::string& username);

// CMD handlers:
	void		handleCmd(Client *c, const ParsedCmd &data);
	void		handlePass(Client *c, const ParsedCmd &data);
	void		handleNick(Client *c, const ParsedCmd &data);
	void		handleUser(Client *c, const ParsedCmd &data);
	void		handlePing(Client *c, const ParsedCmd &data);
	void		handleJoin(Client *c, const ParsedCmd &data);
	void		handleJoinOneChannel(Client *c, const std::string& channel_name, const std::string& key);
	void		handlePrivMsg(Client *c, const ParsedCmd &data);
	void		handleNames(Client *c, const ParsedCmd &data);
	void		handleChannelMsg(Client *c, std::string target, std::string msg);
	void		handleDirectMsg(Client *sender, std::string target, std::string msg);
	void		handlePart(Client *c, const ParsedCmd &data);
	void		handleQuit(Client *c, const ParsedCmd &data);
	void		partFromChannel(Client* c, Channel* channel, const std::string& reason);
	void		handleList(Client *c);
	void		handleMode(Client *c, const ParsedCmd &data);
	std::string	handleMode_channel(Client *c, const ParsedCmd &data);
	void		handleTopic(Client *c, const ParsedCmd &data);
	void		handleKick(Client *c, const ParsedCmd &data);
	void		handleInvite(Client *c, const ParsedCmd &data);

// signal handlers
	void		setSignals();
	static void	signalHandler(int signal);
	bool		isRunning() const;
	void		stop();

// error handlers
	void		handleSocketError(int fd);
	void		cleanShutdown();

// utils
	void		setSocketToNonBlocking(int socket_fd, bool isServerSocket);
	Channel*	findChannel(std::string target);
	Client* 	findClientByNick(const std::string& nick);
	void		cleanupDisconnectedClients();
	void		cleanupEmptyChannels();

//	invalid
	void		InvalidCmd(Client *c,const ParsedCmd &data);
};


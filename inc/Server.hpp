/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:32:51 by aroux             #+#    #+#             */
/*   Updated: 2025/09/28 17:30:11 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

#include <iostream>
#include <map>
#include <cstdlib>			// exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <errno.h>			// not sure if we're allowed to use it
#include <poll.h>
#include <unistd.h>			// read(), open(), close()
#include <sys/socket.h>		// socket()
#include <netinet/in.h>		// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))
#include "Client.hpp"
#include "Channel.hpp"
#include "utilities.hpp"
#include "parser.hpp"

struct ParsedCmd;

class Server {
private:
	int								_port;
	std::string						_server_pass; //server password to connect;
	int								_server_socket;
	std::vector<pollfd>				_fds;	// vector of pollfds : // pollfd describes which fd poll() should watch and what events to look for // events: input, output, errors; and revents (what events actually happened, set by poll(
	std::map<int, Client*>			_connected;	
	std::map<std::string, Channel>	_channels;	//1509A: chattie recommends storing the channels as real objects (rather than pointers), contrary to clients (stored as pointers)

public:
// constructors
	Server();
	Server(const int& port, const std::string& password);	// param constructor with a specified port value
	~Server();
	Server(const Server& copy);
	Server& operator=(const Server& other);

// setters & getters
	void				setPass(const std::string& pass) {_server_pass = pass;};
	const std::string	getPass() const {return _server_pass;};
	
// member functions
	void		start();	// setup socket, bind, listen
	void		run();		// main loop with accept(), recv()
	void		acceptClient();	// accept a new client and adds to client list
	void		handleClient(int fd); 	//read from a client
	bool		isNickTaken(const std::string& nick);
	bool		isValidNick(const std::string& str);
	bool		isValidChar(const char c);


// CMD handlers:
	void		handleCmd(Client *c, const ParsedCmd &data);
	void		handlePass(Client *c, const ParsedCmd &data);
	void		handleNick(Client *c, const ParsedCmd &data);
	void		handleUser(Client *c, const ParsedCmd &data);
	void		handlePing(Client *c, const ParsedCmd &data);
	void		handleJoin(Client *c, const ParsedCmd &data);
	void		handlePrivMsg(Client *c, const ParsedCmd &data);
	void		handleNames(Client *c, const ParsedCmd &data);
	void		handleChannelMsg(Client *c, std::string target, std::string msg);
	void		handleDirectMsg(Client *sender, std::string target, std::string msg);
	void		handlePart(Client *c, const ParsedCmd &data);
	void		handleList(Client *c);
// utils
	Channel*	findChannel(std::string target);
	Client* 	findClientByNick(const std::string& nick);


	// 	TODO: other main cmds
//	void		handlePart(Client *c, const ParsedCmd &data);
//	void		handleQuit(Client *c, const ParsedCmd &data);

// 	TODO: operator cmds : 
//		MODE (i, t, k, o, l): change specific things about channel
//		NAMES; gives all names of people in the channel
//		KICK, 
//		INVITE, 
//		TOPIC 


	void		InvalidCmd(Client *c,const ParsedCmd &data);
	//void	createChannel ?

};


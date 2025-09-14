/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:32:51 by aroux             #+#    #+#             */
/*   Updated: 2025/09/14 17:42:41 by nboer            ###   ########.fr       */
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
#include "utilities.hpp"
#include "parser.hpp"

struct ParsedCmd;

class Server {
private:
	int								_port;
	int								_server_socket;
	std::vector<pollfd>				_fds;	// vector of pollfds : // pollfd describes which fd poll() should watch and what events to look for // events: input, output, errors; and revents (what events actually happened, set by poll(
	std::map<int, Client*>			_connected;	// also stores the list of clients? //
	std::map<std::string, Channel*>	_channels;	// list of all existing channels //
	std::string						_globalpass; //global password to connect;
public:
// constructors
	Server();
	Server(const int& port);	// param constructor with a specified port value
	~Server();

// setters & getters
	void		setPass(const std::string& pass) {_globalpass = pass;};
	const std::string	getPass() const {return _globalpass;};
	
// member functions
	void		start();	// setup socket, bind, listen
	void		run();		// main loop with accept(), recv()
	void		acceptClient();	// accept a new client and adds to client list
	void		handleClient(int fd); 	//read from a client
	void		handleCmd(Client *c, const ParsedCmd &data);
	void		handlePing(Client *c, const ParsedCmd &data);
	void		handleNick(Client *c, const ParsedCmd &data);
	void		handlePass(Client *c, const ParsedCmd &data);
	void		InvalidCmd(Client *c,const ParsedCmd &data);
	//void	createChannel ?

};


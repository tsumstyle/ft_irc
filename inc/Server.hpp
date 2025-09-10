/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:32:51 by aroux             #+#    #+#             */
/*   Updated: 2025/09/10 14:18:23 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <map>
#include <cstdlib>			// exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <errno.h>			// not sure if we're allowed to use it
#include <poll.h>
#include <unistd.h>			// read(), open(), close()
#include <sys/socket.h>		// socket()
#include <netinet/in.h>		// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))
#include "Client.hpp"

class Server {
private:
	int						_port;
	int						_server_socket;
	std::vector<pollfd>		_fds;	// vector of pollfds : 
									// pollfd describes which fd poll() should watch and what events to look for
									// events: input, output, errors; and revents (what events actually happened, set by poll(
	std::map<int, Client>	_connected;// also stores the list of clients?
	
public:
// constructors
	Server();					// let's default it to _port value 8080
	Server(const int& port);	// param constructor with a specified port value
	Server(const Server& copy);
	~Server();
	Server& operator=(const Server& other);

// member functions
	void	start();	// setup socket, bind, listen
	void	run();		// main loop with accept(), recv()
	void	acceptClient();	// accept a new client and adds to client list
	void	handleClient(int fd); 	//read from a client
};

#endif
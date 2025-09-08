/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:32:51 by aroux             #+#    #+#             */
/*   Updated: 2025/09/08 14:00:12 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <map>
#include "Client.hpp"

class Server {
private:
	int	_port;
	std::map<int, Client>	connected;// also stores the list of clients?
	int	_server_socket;
	
	
public:
// constructors
	Server();					// let's default it to _port value 8080
	Server(const int& port);	// param constructor with a specified port value
	Server(const Server& copy);
	~Server();
	Server& operator=(const Server& other);

		// accept client connection
		//
// member functions
	void	start();	// setup socket, bind, listen
	void	run();		// main loop with accept(), recv()
	void	acceptClient();	// accept a new client and adds to client list
	void	handleClient(int fd); 	//read from a client
};

#endif
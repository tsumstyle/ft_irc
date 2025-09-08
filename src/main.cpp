/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:22:32 by aroux             #+#    #+#             */
/*   Updated: 2025/09/08 15:10:11 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>		// socket()
#include <netinet/in.h>		// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))
#include <unistd.h>			// read()
#include <cstdlib>			// exit(), EXIT_FAILURE
#include <cerrno>			// errno
#include <poll.h>			// poll(), pollfd
#include "utils.hpp"
#include "Server.hpp"

int	main(void)	{
	std::cout << "testing simple server" << std::endl;

// 1) creating a socket (fd that will be used for communication) : IPv4, TCP
// we're using Internet IPv4 protocol, so it's AF_INET 
// SOCK_STREAM means we're using a TCP (two-way reliable communication type)
// 0 is to use the default protocol associated with the type SOCK_STREAM
	Server	irc(8080);
	int	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		std::cout << "Failed to create socket. errno: " << errno <<std::endl;	// dont think we're allowed errno
		exit(EXIT_FAILURE);
	}
//	2) specify address
// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))
// struct sockaddr_in {
//    sa_family_t    sin_family; /* address family: AF_INET */
//    in_port_t      sin_port;   /* port in network byte order */
//    struct in_addr sin_addr;   /* internet address */
// };

// listen to port 8080
	sockaddr_in	server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;	// accepts from any IP
	server_address.sin_port = htons(8080); 			// htons converts to network byte order

// 3) bind socket : bind() assigns an IP address and port to the socket:
	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)	{
		std::cout << "Failed to bind port 8080. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
// 4) listen to port: 
	if (listen(server_socket, 10) < 0)	{ // we set it to accept 10 queued connections
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

// 5) Setup poll and create server_poll: fds[0]: server listening socket
//										 fd[1 - N]: client sockets
	std::vector<pollfd>	fds;			// vector of pollfds
										// pollfd describes which fd poll() should watch and what events to look for
										// events: input, output, errors; and revents (what events actually happened, set by poll())
	pollfd				server_poll;	
	server_poll.fd = server_socket;
	server_poll.events = POLLIN;		// "tell me when this socket is ready to read" (when data is avaiable to recv())
	fds.push_back(server_poll);			// adding the server poll the pollfd vector
	char	buffer[510];

// 6) listen to clients
	while (true) {
		int	activity = poll(fds.data(), fds.size(), -1); // poll() blocks until any of these sockets has an event (new connection, incoming data, etc.), and then you check the .revents field of each pollfd.
		
	// iterate through all fds:
		for (size_t i = 0; i < fds.size(); i++)	{
		// if there are events (if the option POLLIN is set in revents i.e. socket is readable)
			if (fds[i].revents & POLLIN) {	// bitwise 'AND'
			// if it's the server socket: accept new client
				if(fds[i].fd == server_socket) { 
					int	client_socket = accept(server_socket, nullptr, nullptr);
					if (client_socket < 0)	{
						std::cout << "accept failed" << std::endl;
						continue;
					}
					std::cout << "New client connected: fd " << client_socket << std::endl;
				// add new client to fds
					pollfd	client_poll;
					client_poll.fd = client_socket;
					client_poll.events = POLLIN;
					fds.push_back(client_poll);
				}
			// handle data from already existing clients:
				else {	
					// read from the connection
					ssize_t	bytes_read = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);		// read fucntion for sockets		
					if (bytes_read <= 0)	{
						std::cout << "Client disconnected: fd " << fds[i].fd << std::endl;
						close(fds[i].fd);
						fds[i].fd = -1; // mark socket as closed
					}
					else { // if there's something, handle message
						buffer[bytes_read] = '\0';
						std::cout << "Message: " << buffer << std::endl;
						std::string	response = "Received: ";
						response.append(buffer);
						send(fds[i].fd, response.c_str(), response.size(), 0);
					}
					
				}
			}
		}
	}
// close connections
	close(server_socket);
	return 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:22:32 by aroux             #+#    #+#             */
/*   Updated: 2025/09/08 13:50:36 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>		// socket()
#include <netinet/in.h>		// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))
#include <unistd.h>			// read()
#include <cstdlib>			// exit(), EXIT_FAILURE
#include <cerrno>			// errno
#include "utils.hpp"

int	main(void)	{
	std::cout << "testing simple server" << std::endl;

// 1) creating a socket (fd that will be used for communication) : IPv4, TCP
// we're using Internet IPv4 protocol, so it's AF_INET 
// SOCK_STREAM means we're using a TCP (two-way reliable communication type)
// 0 is to use the default protocol associated with the type SOCK_STREAM
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
	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(sockaddr)) < 0)	{
		std::cout << "Failed to bind port 8080. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

// 4) listen
	while (1) {
		int	client_socket;

		if (listen(server_socket, 10) < 0)	{ // we set it to accept 10 queued connections
			std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
			exit(EXIT_FAILURE);
		}

		int	client_socket = accept(server_socket, nullptr, nullptr);
		if (client_socket < 0)	{
			std::cout << "Failed to grab connection. errno: " << errno << std::endl;
			exit(EXIT_FAILURE);
		}
		// read from the connection
		char	buffer[510];
		recv(client_socket, buffer, sizeof(buffer), 100);		// read fucntion for sockets		
		std::cout << "Message: " << buffer << std::endl;
		std::string	response = "Received: ";
		response.append(buffer);
		send(client_socket, response.c_str(), response.size(), 0);
		close(client_socket);	// is it right to close it here or rather outside of the loop?
	}
	
// close connections
	
	close(server_socket);
	return 0;
}
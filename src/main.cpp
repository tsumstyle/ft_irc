/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:22:32 by aroux             #+#    #+#             */
/*   Updated: 2025/09/05 14:45:20 by aroux            ###   ########.fr       */
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

// creating a socket (fd that will be used for communication) : IPv4, TCP
// we're using Internet IPv4 protocol, so it's AF_INET 
// SOCK_STREAM means we're using a TCP (two-way reliable communication type)
// 0 is to use the default protocol associated with the type SOCK_STREAM
	int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cout << "Failed to create socket. errno: " << errno <<std::endl;	// dont think we're allowed errno
		exit(EXIT_FAILURE);
	}

// sockaddr_in (struct used to define the address we want to assign to the socket in the case of an Internet Protocol (IP))
// struct sockaddr_in {
//    sa_family_t    sin_family; /* address family: AF_INET */
//    in_port_t      sin_port;   /* port in network byte order */
//    struct in_addr sin_addr;   /* internet address */
// };

// listen to port 8080
	sockaddr_in	sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;		// ?
	sockaddr.sin_port = htons(8080); 			// htons converts a number to a network byte order

// bind() assigns an IP address and port to the socket:
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)	{
		std::cout << "Failed to bind port 8080. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

// start listening, and hold max 10 connections in the queue
	if (listen(sockfd, 10) < 0)	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

// grab a connection from the queue
	ssize_t 	addrlen = sizeof(sockaddr);
	int			connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
	if (connection < 0)	{
		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

// read from the connection
	char	buffer[510];
	ssize_t	bytesRead = read(connection, buffer, 100);
	std::cout << "Message: " << buffer << " of size: " << bytesRead << "bytes";

// send a message to the connection
	std::string	response = "Message received \n";
	send(connection, response.c_str(), response.size(), 0);

// close connections
	close(connection);
	close(sockfd);
	return 0;
}
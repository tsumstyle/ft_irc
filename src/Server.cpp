/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:00:47 by aroux             #+#    #+#             */
/*   Updated: 2025/09/09 16:28:55 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

//constructors
Server::Server() : _port(8080) {}

Server::Server(const int& port) : _port(port) {}

Server::Server(const Server& copy) : _port(copy._port),
									 _server_socket(copy._server_socket),
									 _fds(copy._fds),
									 _connected(copy._connected) {}

Server::~Server() {}

//assigment operator
Server&	Server::operator=(const Server& other) {
	if (this != &other) {
		_port = other._port;
		_server_socket = other._server_socket;
		_fds = other._fds;
		_connected = other._connected;
	}
	return *this;
}

//other member functions
void	Server::start() {
// 1) creating a socket (fd that will be used for communication) : IPv4, TCP
// we're using Internet IPv4 protocol, so it's AF_INET 
// SOCK_STREAM means we're using a TCP (two-way reliable communication type)
// 0 is to use the default protocol associated with the type SOCK_STREAM
	_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_socket == -1) {
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
	if (bind(_server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)	{
		std::cout << "Failed to bind port 8080." << std::endl;
		exit(EXIT_FAILURE);
	}
// 4) listen to port: 
	if (listen(_server_socket, 10) < 0)	{ // we set it to accept 10 queued connections
		std::cout << "Failed to listen on socket." << std::endl;
		exit(EXIT_FAILURE);
	}
// 5) Setup poll and create server_poll: fds[0]: server listening socket
//										 fds[1 - N]: client sockets
	pollfd				server_poll;	
	server_poll.fd = _server_socket;
	server_poll.events = POLLIN;		// "tell me when this socket is ready to read" (when data is avaiable to recv())
	_fds.push_back(server_poll);		// adding the server poll the pollfd vector
}

void	Server::acceptClient() {
	int	client_socket = accept(_server_socket, NULL, NULL);
	if (client_socket < 0)	{
		std::cout << "accept failed" << std::endl;
		return;
	}
	std::cout << "New client connected: fd " << client_socket << std::endl;
// add new client to fds
	pollfd	client_poll;
	client_poll.fd = client_socket;
	client_poll.events = POLLIN;
	_fds.push_back(client_poll);
// instantiate Client object and add it to the _connected map
	_connected[client_socket] = Client(client_socket);
}

void	Server::handleClient(int fd) {
	// read from the connection
	char	buffer[510];
	ssize_t	bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);		// read fucntion for sockets		
	if (bytes_read <= 0)	{
		std::cout << "Client disconnected: fd " << fd << std::endl;
		close(fd);
		fd = -1; // mark socket as closed
	}
	else { // if there's something, handle message
		buffer[bytes_read] = '\0';
		std::cout << "Message: " << buffer << std::endl;
		std::string	response = "Received: ";
		response.append(buffer);
		send(fd, response.c_str(), response.size(), 0);
	}			
}

void	Server::run() {
	while (true) {
		int	activity = poll(_fds.data(), _fds.size(), -1); // poll() blocks until any of these sockets has an event (new connection, incoming data, etc.), and then you check the .revents field of each pollfd.	
		(void)activity; 	// will use it later but for now just silence the compile error
		// iterate through all fds:
		for (size_t i = 0; i < _fds.size(); i++)	{
			if (_fds[i].fd == -1)
				continue;
		// if there are events (if the option POLLIN is set in revents i.e. socket is readable)
			if (_fds[i].revents & POLLIN) {	// bitwise 'AND'
			// if it's the server socket: accept new client
				if(_fds[i].fd == _server_socket) 
					acceptClient();
			// handle data from already existing clients:
				else
					handleClient(_fds[i].fd);
			}
		}
	}
	close(_server_socket);
}

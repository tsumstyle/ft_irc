/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nick <nick@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:00:47 by aroux             #+#    #+#             */
/*   Updated: 2025/10/06 14:49:28 by nick             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/utilities.hpp"
#include "../inc/parser.hpp"

Server*	Server::_instance = NULL;

#define BUFSIZE 510
//#define QUEUE_SIZE 10 --> inside the listen() function

//constructors
Server::Server() : _running(true), _port(6667), _server_pass("default_pw"), _server_socket(-1) { _instance = this; }

Server::Server(const int& port, const std::string& password) : _running(true), 
															   _port(port),
															   _server_pass(password),
															   _server_socket(-1) { _instance = this; }

Server::Server(const Server& copy) : _running(true),
									 _port(copy._port),
									 _server_pass(copy._server_pass),
									 _server_socket(copy._server_socket),
									 _fds(copy._fds),
									 _connected(copy._connected) { _instance = this; }

Server::~Server() {
// close client sockets and delete Client objects
	serverLog(NULL, "Server destructor: cleaning up resources");
	int	closedClients = 0;
	for (std::map<int, Client*>::iterator it = _connected.begin(); it != _connected.end(); ++it) {
		if (it->second) {
			serverLog(NULL, "Closing client fd" + toString(it->first));
			close (it->first);
			delete it->second;
			closedClients++;
		}	
	}
	_connected.clear();
// close server socket
	if (_server_socket != -1) {
		serverLog(NULL, "Closing server socket");
		close(_server_socket);
		_server_socket = -1;
	}
// clear other containers
	_fds.clear();
	_channels.clear();

	serverLog(NULL, "Server cleanup complete. Closed " + toString(closedClients) + " client connections.");
}

//assigment operator
Server&	Server::operator=(const Server& other) {
	if (this != &other) {
		 _instance = other._instance; 
		_port = other._port;
	 	_server_pass = other._server_pass,
		_running = other._running;
		_server_socket = other._server_socket;
		_fds = other._fds;
		_connected = other._connected;
	}
	return *this;
}

//other member functions
void	Server::start() {
// 1) creating a socket (fd that will be used for communication) : IPv4, TCP
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

// listen to port _port
	sockaddr_in	server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;	// accepts from any IP
	server_address.sin_port = htons(_port); 			// htons converts to network byte order

// 3) bind socket : bind() assigns an IP address and port to the socket:
	if (bind(_server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)	{
		std::cout << "Failed to bind port " << _port << std::endl;
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

void	Server::run() {
	setSignals();
	serverLog(NULL, "Server started on port " + toString(_port));
	while (_running) {
		int	activity = 0;
		activity = poll(_fds.data(), _fds.size(), -1); // poll() blocks until any of these sockets has an event (new connection, incoming data, etc.), and then you check the .revents field of each pollfd.	
		if (activity == -1) {
			if (errno == EINTR) {
				if (!_running)
					break;
				continue;
			}
			serverLog(NULL, "Error: poll system call failed");
			break;
		}
		if (activity == 0) {		//timeout - check if we should shutdown
			if (!_running)
				break;
			continue;
		}
		// iterate through all fds:
		for (size_t i = 0; i < _fds.size(); i++)	{
			if (!_running)
				break;
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
			if (_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
					handleSocketError(_fds[i].fd);
		}
		cleanupDisconnectedClients();
		cleanupEmptyChannels();
	}
	cleanShutdown();
}

void	Server::acceptClient() {
	int	client_socket = accept(_server_socket, NULL, NULL);
	if (client_socket < 0)	{
		std::cout << "accept failed" << std::endl;
		return;
	}
	std::cout << "New client connected: fd " << client_socket << std::endl;
	pollfd	client_poll;
	client_poll.fd = client_socket;
	client_poll.events = POLLIN;
	client_poll.revents = 0;
	_fds.push_back(client_poll);
	_connected[client_socket] = new Client(client_socket); // dont forget to DELETE against memleaks
	Client *c = _connected[client_socket];


	//std::string welcome = "Welcome to InstantRegretChat! Please enter your password using the PASS command.\r\n";
	std::string welcome = welcome_message();
	c->sendMessage(welcome);
}

void	Server::handleClient(int fd) {		// read from the connection
	char	buffer[BUFSIZE];
	ParsedCmd parse_data;
	Client*	client = _connected[fd];
	ssize_t	bytes_read = recv(fd, buffer, BUFSIZE - 1, 0); // read fucntion for sockets
	
	if (bytes_read <= 0)	{
		std::cout << "Client disconnected: fd " << fd << std::endl;
		client->setState(DISCONNECTED); 	// mark for cleanup
	}
	else {
		buffer[bytes_read] = '\0';
		std::string input(buffer);		// INPUT validation
		if (input.find_first_not_of(" \t\n\r") == std::string::npos)
			return ;
		parse_data = parseMsg(std::string(buffer));
		toUpperCmd(&parse_data);
		if (!parse_data.cmd.empty()) {
			std::cout << "CMD: " << parse_data.cmd << std::endl; 						//for debug
			for (unsigned long i = 0; i < parse_data.args.size(); i++)					//for debug
				std::cout << "Arg " << i << " " << parse_data.args[i] << std::endl;	//for debug
			handleCmd(client, parse_data);
		}
	}
}

void	Server::handleCmd(Client *c, const ParsedCmd &data) {
// 1. login commands
	if (data.cmd == "PASS")
		handlePass(c, data);
	else if (data.cmd == "NICK")
		handleNick(c, data);
	else if (data.cmd == "USER")		// username --> idem
		handleUser(c, data);
	
// 2. user commands
	else if (data.cmd == "JOIN") 		// join a channel
		handleJoin(c, data);
	else if (data.cmd == "PRIVMSG") 	// send a message to another user, or to a whole channel
		handlePrivMsg(c, data);
	else if (data.cmd == "PART") 		// quit one channel
		handlePart(c, data);
	else if (data.cmd == "QUIT")		// quit server
		handleQuit(c, data);
	else if (data.cmd == "NAMES")	//display all nicks in a channel
		handleNames(c, data);
	else if (data.cmd == "LIST") //display list of all channels and details
		handleList(c);
	else if (data.cmd == "PING")
		handlePing(c, data);
// 3. operator commands (TODO) MODE, INVITE, KICK, TOPIC
	else if (data.cmd == "MODE")
		handleMode(c, data);
	else if (data.cmd == "TOPIC")
		handleTopic(c, data);
	else if (data.cmd == "KICK")
		handleKick(c, data);
	else if (data.cmd == "INVITE")
		handleInvite(c, data);
	else
		InvalidCmd(c, data);
}




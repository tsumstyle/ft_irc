/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:00:47 by aroux             #+#    #+#             */
/*   Updated: 2025/09/11 15:38:14 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/utils.hpp"

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
// we're using Internet IPv4 protocol (AF_INET) 
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
// 4) listen to port: set server_socket as passive eg listening to whatever clients will send
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
	char	buffer[512];		// IRC max msg size is 512 bytes

	int	bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);		// read fucntion for sockets		
	if (bytes_read <= 0)	{
		std::cout << "Client disconnected: fd " << fd << std::endl;
		close(fd);
		fd = -1; // mark socket as closed
		return ;
	}
// if there's something, handle message
	buffer[bytes_read] = '\0';
	Client&	client = _connected[fd];
	client.getBuffer().append(buffer);
// process all complete lines (end with "\r\n" according to IRC protocol)
	size_t pos;
	while ((pos = client.getBuffer().find("\r\n")) != std::string::npos) { // as long as I find complete message (ie terminated by \r\n), process them
		std::string	line = client.getBuffer().substr(0, pos);
		client.getBuffer().erase(0, pos+ 2); // remove processed line
		parseCommand(server, client, line);
	}
}

void	Server::parseCommand(Client& client, const std::string& line) {
	std::cout << "Received from client " << client.getSocket()
			  << ": " << line << std::endl;
// convert string to a input stringstream:
	std::istringstream	iss(line);
	std::string			command;
	iss >> command;					// the >> operator reads the 1st whitespace-sperated token from the stream into command and then advances the cursor (ie it does your parsing for you token by token)

	if (command == "NICK")			// nickname --> store it in the Client class
		handleNick(client, iss);
	else if (command == "USER")		// username --> idem
		handleUser(client, iss);
	else if (command == "PASS") {	// give super secret password 
		}
	else if (command == "JOIN") 	// join a channel
		handleJoin(client, iss);
	else if (command == "PRIVMSG") 	// send a message to another user, or to a whole channel
		handlePrivmsg(client, iss);
	else if (command == "PART") {	// quit one channel

	}
	else if (command == "QUIT") {	// quit server

	}	
	else							// unknown command
		handleUnknown(client, iss);
}

// parse command:
void	Server::handleNick(Client& client, std::istringstream iss) {
	std::string	nick;
	iss >> nick;			// next token is the value given for nickname, store it in a string
	client.setNick(nick);
	if (client.getState() == USERNAME_REGISTERED) {
		client.setState(FULLY_REGISTERED);
		// write server log: Client XX set nickname to 'YYY' 
		// sendMessage(msg, client); [TODO: implement] // send IRC welcome messages to client
	}
	else {
		client.setState(NICK_REGISTERED);
		// write server log: Client XX set nickname to 'YYY' 
	}
	std::cout << "Client " << client.getSocket() << " set NICK = " << nick << std::endl;
}

void	Server::handleUser(Client& client, std::istringstream iss) {
	std::string	user;
	iss >> user;
	client.setUser(user);
	if (client.getState() == NICK_REGISTERED) {
		client.setState(FULLY_REGISTERED);
		// write server log: Client XX set username to 'YYY' 
		// sendMessage(msg, client); [TODO: implement] // send IRC welcome messages to client
	}
	else {
		client.setState(USERNAME_REGISTERED);
		// write server log: Client XX set username to 'YYY' 
	}
}

void	Server::handleJoin(Client& client, std::istringstream iss) {
	std::string channel;
	std::string	channel_name;
	iss >> channel;
	if (channel[0] != '#')
		// invalid, return 
	channel_name = channel.substr(1, channel.size() - 1);
	for (size_t i = 0; i < _channels.size(); i++) {
		if (_channels[i] == channel_name) {
			// channel  already exists, add user to channel
			_channels[i].addUser(client);
			// add channel to the client's list of channels (or useless step?) : client.joinChannel(_channels[i])
			// server and client output
		}
		else {
			Channel	newChannel = Channel(channel_name); // create new channel
			_channels[channel_name] = newChannel;		// add it to the server
			_channels[i].addUser(client);
			// server output: channel created + user joined channel + made operator
			// client output: channel created + joined the channel + made operator
		}	
	}
}

void	Server::handlePrivmsg(Client& client, std::istringstream iss) {
	std::string	target, msg;
	iss >> target;
	std::getline(iss, msg); // gets the rest of the stream after the first argument token
	if (!msg.empty() && msg[0] == ' ') // trim leading space
		msg.erase(0, 1);
	if (!msg.empty() && msg[0] == ':') // trim leading :
		msg.erase(0, 1);
	// send msg to target (user or channel)
}

void	Server::handleUnknown(Client& client, std::istringstream iss) {
	std::cout << "Unknown command: " << command << std::endl;
}


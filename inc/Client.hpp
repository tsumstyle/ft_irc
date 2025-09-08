/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:06:42 by nboer             #+#    #+#             */
/*   Updated: 2025/09/08 13:36:54 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Represents each connected user

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include "Channel.hpp"

class Client {
private:
	std::string	_nick;
	std::string	_user;
	bool		_state;
	int			_fd;
	std::vector<Channel>	channels; // channels it is part of/ or in the format of std::vector<bool> ?

public:
// constructors
	Client();
	Client(const Client& copy);
	~Client();
	Client& operator=(const Client& other);
		// create socket
		// connect
		// send data
		// receive data
		// disconnect

};

#endif

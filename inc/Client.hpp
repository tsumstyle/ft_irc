/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:06:42 by nboer             #+#    #+#             */
/*   Updated: 2025/09/04 17:43:20 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Represents each connected user

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

class Client {
	private:
		std::string	_nick;
		std::string	_user;
		bool		_state;
		int			_fd;
	public:
		Client();
		~Client();
		// create socket
		// connect
		// send data
		// receive data
		// disconnect

};

#endif

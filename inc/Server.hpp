/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:32:51 by aroux             #+#    #+#             */
/*   Updated: 2025/09/08 12:03:42 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>

class Server {
private:
	int	_port;
	
public:
// constructors
	Server();
	Server(const Server& copy);
	~Server();
	Server& operator=(const Server& other);

		// accept client connection
		//

};

#endif
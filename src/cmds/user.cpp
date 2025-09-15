/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:20 by aroux             #+#    #+#             */
/*   Updated: 2025/09/15 16:28:06 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/messages.hpp"


void	Server::handleUser(Client *c, const ParsedCmd &data) {
	std::string reply;

	if (c->getState() == NEW) {
		reply = "Error XX?: Should send the pass first \r\n";
	}
	else if (data.args.empty())
		reply = "Error 431: No username given \r\n";
	else if (data.args.size() > 1)
		reply = "Error 432: No more than one argument allowed \r\n";
	else {
		c->setUser(data.args[0]);
		if (c->getState() == NICK_OK) {
			c->setState(REGISTERED);
			// write server log: Client XX set nickname to 'YYY' 
			reply = "Username set to: " + c->getUser() + "\r\n"; // ADD welcome to IRC message
		}
		else {
			c->setState(USERNAME_OK);
			reply = "Username set to: " + c->getUser() + "\r\n";
			// write server log: Client XX set nickname to 'YYY' 
		}
	}
	send(c->getSocket(), reply.c_str(), reply.size(), 0);
}

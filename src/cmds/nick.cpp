/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:26:43 by aroux             #+#    #+#             */
/*   Updated: 2025/09/15 15:31:56 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/messages.hpp"

void	Server::handleNick(Client *c, const ParsedCmd &data) {
	std::string reply;
	
	if (c->getState() == NEW) {
		reply = "Error XX?: Should send the pass first \r\n";
	}
	else if (data.args.empty())
		reply = "Error 431: No nickname given \r\n";
	else if (data.args.size() > 1)
		reply = "Error 432: No more than one argument allowed \r\n";
	else {
		c->setNick(data.args[0]);
		if (c->getState() == USERNAME_OK) {
			c->setState(REGISTERED);
			// write server log: Client XX set nickname to 'YYY' 
			reply = "Nickname set to: " + c->getNick() + "\r\n"; // ADD welcome to IRC message
		}
		else {
			c->setState(NICK_OK);
			reply = "Nickname set to: " + c->getNick() + "\r\n";
			// write server log: Client XX set nickname to 'YYY' 
		}		
	}
	send(c->getSocket(), reply.c_str(), reply.size(), 0);
}

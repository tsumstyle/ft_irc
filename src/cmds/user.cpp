/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:20 by aroux             #+#    #+#             */
/*   Updated: 2025/09/16 15:31:05 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

/* USER
Requirements:
   - Client must have sent PASS first (state != NEW), otherwise send ERR_NOTREGISTERED (451).
   - Must provide all required parameters (username, hostname, servername, realname), otherwise send ERR_NEEDMOREPARAMS (461). 
Behavior:
   - Sets the client's username and real name.
   - Updates client state:
       - If NICK_OK, client becomes REGISTERED → send welcome messages.
       - Otherwise, state becomes USERNAME_OK (waiting for NICK command).
   - Logs the username assignment on the server. 
   
 1609A: I changed most of the NICK command but not this one yet. It should follow the same structure  */
   
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

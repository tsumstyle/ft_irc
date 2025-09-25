/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:20 by aroux             #+#    #+#             */
/*   Updated: 2025/09/25 15:41:14 by aroux            ###   ########.fr       */
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

	if (c->getState() == NEW) 
		c->sendMessage(Replies::ERR_NOTREGISTERED("client", "USER"));	// TODO: check, i'm not sure
	else if (data.args.empty())
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS("USER"));
//	else if (data.args.size() > 1)
//		reply = "Error 432: No more than one argument allowed \r\n";
	else {
		c->setUser(data.args[0]);
		if (c->getState() == NICK_OK) {
			c->setState(REGISTERED);
			serverLog(c, " is fully registered");
			c->sendMessage(Replies::RPL_WELCOME(c->getNick(), c->getUser()));		
		}
		else {
			c->setState(USERNAME_OK);
			serverLog(c, " set nickname to '" + c->getNick() + "'");
			c->sendMessage(Replies::RPL_YOURHOST(c->getNick()));
		}
	}
	send(c->getSocket(), reply.c_str(), reply.size(), 0);
}

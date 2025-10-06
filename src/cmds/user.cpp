/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:20 by aroux             #+#    #+#             */
/*   Updated: 2025/10/06 13:59:50 by aroux            ###   ########.fr       */
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
   - Logs the username assignment on the server. */
   

void	Server::handleUser(Client *c, const ParsedCmd &data) {
	if (c->getState() == NEW) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "USER"));	// TODO: check, i'm not sure
		return ;
	}
	if (c->getState() == REGISTERED) {
		c->sendMessage(Replies::ERR_ALREADYREGISTERED());	// TODO: check, i'm not sure
		return ;
	}
	if (data.args.empty() || data.args.size() < 4) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), "USER"));
		return ;
	}
	std::string	username = data.args[0];
	std::string realname = data.args[3];
	if (!isValidUsername(username))
		c->sendMessage(Replies::ERR_ERRONEUSUSERNAME(username));
	else {
		c->setUser(username);
		if (c->getState() == NICK_OK) {
			c->setState(REGISTERED);
			serverLog(c, " is fully registered");
			c->sendMessage(Replies::RPL_WELCOME(c->getNick(), username));		
		}
		else {
			c->setState(USERNAME_OK);
			serverLog(c, " set username to '" + username + "'");
			c->sendMessage(Replies::RPL_YOURHOST(c->getNick()));
		}
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:26:43 by aroux             #+#    #+#             */
/*   Updated: 2025/09/16 15:28:51 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

/* NICK
Requirements:
   - Client must have sent PASS first (state != NEW), otherwise ERR_NOTREGISTERED (451).
   - NICK must be provided, otherwise ERR_NEEDMOREPARAMS (461).
   - Nickname must follow valid characters rules:
       Allowed: A-Z, a-z, 0-9, -, _, [, ], \, `, ^, {, |, } 
       Not allowed: space, control characters, special IRC chars.
       If invalid, send ERR_ERRONEUSNICKNAME (432).
   - Nickname must be unique; if already in use, send ERR_NICKNAMEINUSE (433).
Behavior:
   - Sets the nickname for the client.
   - Updates client state:
       - If USERNAME_OK, client becomes REGISTERED → send welcome messages:
           RPL_WELCOME (001), RPL_YOURHOST (002), RPL_CREATED (003), RPL_MYINFO (004)
       - Otherwise, state becomes NICK_OK (waiting for USER command).
   - Logs the nickname assignment on the server.
*/
void	Server::handleNick(Client *c, const ParsedCmd &data) {
	std::string reply;
	
	if (c->getState() == NEW)
		c->sendMessage(Replies::ERR_NOTREGISTERED("client", "NICK"));	// TODO: check, i'm not sure
	else if (data.args.empty())
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick() , "NICK"));   // TODO: check that this makes sense. if NICK fails, no getNick() ?
//	if (data.args.size() > 1)		// check the error code for this one
//	Error 432 is ERR_ERRONEUSNICKNAME: invalid characters in the nickname (see rules for nickname in our notion page)
	else if (isNickTaken(data.args[0]))
		c->sendMessage(Replies::ERR_NICKNAMEINUSE(c->getNick()));
	else {
		c->setNick(data.args[0]);
		if (c->getState() == USERNAME_OK) {
			c->setState(REGISTERED);
			serverLog(c, " is fully registered");
			c->sendMessage(Replies::RPL_WELCOME(c->getNick(), c->getUser()));
		}
		else {
			c->setState(NICK_OK);
			serverLog(c, " set nickname to '" + c->getNick() + "'");
			c->sendMessage(Replies::RPL_YOURHOST(c->getNick()));
		}
	}	
}

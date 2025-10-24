/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:20 by aroux             #+#    #+#             */
/*   Updated: 2025/10/24 14:47:13 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handleUser(Client *c, const ParsedCmd &data) {
	if (c->getState() == NEW) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "USER"));	
		return ;
	}
	if (c->getState() == REGISTERED) {
		c->sendMessage(Replies::ERR_ALREADYREGISTERED());
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


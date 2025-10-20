/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 15:26:11 by nboer             #+#    #+#             */
/*   Updated: 2025/10/20 17:06:44 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

// channel topic still has to be included in the listing.
void Server::handleList(Client *c) {
	if (c->getState() != REGISTERED) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "LIST"));
		return;
	}
	for (std::map<std::string, Channel>::iterator it = _channels.begin();
		it != _channels.end(); ++it)
	{
		const std::string &channel_name = it->first;
		Channel *ch = findChannel(channel_name);
		std::ostringstream oss;
		oss << ch->getUsers().size();
		std::string reply = "322 " + c->getNick() + " " 
							+ channel_name + " "
							+ oss.str();
		c->sendMessage(reply + "\r\n");
	}
	c->sendMessage("323 " + c->getNick() + " :End of /LIST\r\n");
}

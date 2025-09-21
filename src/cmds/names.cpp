/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 16:01:18 by nboer             #+#    #+#             */
/*   Updated: 2025/09/21 17:56:05 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handleNames(Client *c, const ParsedCmd &data){
	std::string reply;
	if (c->getState() != REGISTERED)
		reply = Replies::ERR_NOTREGISTERED(c->getNick(), "NAMES");
	else if (data.args.empty())
		reply = Replies::ERR_NEEDMOREPARAMS(data.cmd);
	else {
		Channel *ch = findChannel(data.args[0]);
		if (!ch)
			reply = "Channel does not exist. \r\n";
		else {
			std::string names;
			std::vector<Client*> _users = ch->getUsers();
			for (size_t i = 0; i < _users.size(); ++i) {
					names += _users[i]->getNick();
				if (i < _users.size() - 1)
					names += " ";
				reply = Replies::RPL_NAMES(c, ch->getName(), names);
			}	
		}
	}
	c->sendMessage(reply);
}

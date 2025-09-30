/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 16:01:18 by nboer             #+#    #+#             */
/*   Updated: 2025/09/30 16:10:27 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handleNames(Client *c, const ParsedCmd &data){
	std::string nick = c->getNick().empty() ? "*" : c->getNick(); 		// 30.9 Alex: I had that because in case they haven't registered and input a nick, feels a bit weird to user getNick without value
	std::string reply;
	if (c->getState() != REGISTERED)
		reply = Replies::ERR_NOTREGISTERED(nick, "NAMES");
	else if (data.args.empty())
		reply = Replies::ERR_NEEDMOREPARAMS(nick, data.cmd);
	else {
		Channel *ch = findChannel(data.args[0]);
		if (!ch)
			reply = Replies::ERR_NOSUCHCHANNEL(nick, data.args[0]);
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

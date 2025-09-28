/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 15:26:11 by nboer             #+#    #+#             */
/*   Updated: 2025/09/28 14:25:16 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void Server::handleList(Client *c) {
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

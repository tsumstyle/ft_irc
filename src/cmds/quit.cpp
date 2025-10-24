/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nboer <nboer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:16 by aroux             #+#    #+#             */
/*   Updated: 2025/10/24 15:56:43 by nboer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handleQuit(Client *c, const ParsedCmd &data) {
	std::string	reason = "Client quit";
	if (!data.args.empty()) {
		std::string firstArg = data.args[0];
		if (firstArg[0] == ':') {
			reason = firstArg.substr(1);			
			for (size_t	i = 1; i < data.args.size(); i++)
				reason += " " + data.args[i];
		}
		else {
			reason = data.args[0];
			for (size_t	i = 1; i < data.args.size(); i++)
				reason += " " + data.args[i];
		}
	}

	std::vector<Channel*>	channels = c->getChannels();
	for (size_t i = 0; i < channels.size(); i++) {
		partFromChannel(c, channels[i], reason);
	}
	c->setState(DISCONNECTED);
	serverLog(c, " quit: " + reason);
}

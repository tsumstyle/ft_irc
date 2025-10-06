/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:27:16 by aroux             #+#    #+#             */
/*   Updated: 2025/09/30 17:40:51 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

// enter after QUIT to get terminal back?

/* QUIT

Requirements:
  - Can be sent at any time.
  - Optional quit message may be included.

Behavior:
  - Disconnects the client from the server.
  - Removes client from all joined channels.
  - Sends PART/QUIT messages to other users in shared channels.
  - Sets client to disconnected; cleanup of resources will happen in the main server loop
  - Logs the client disconnection on the server.
*/

void	Server::handleQuit(Client *c, const ParsedCmd &data) {
	std::string	reason = "Client quit";
	if (data.args.size() > 0 && data.lastTokenHasColon) {
		reason = data.args[0];			
		for (size_t	i = 1; i < data.args.size(); i++)							
			reason += " " + data.args[i];
	}
	std::vector<Channel*>	channels = c->getChannels();
	for (size_t i = 0; i < channels.size(); i++) {
		partFromChannel(c, channels[i], reason);
	}
	c->setState(DISCONNECTED);
	serverLog(c, " quit: " + reason);
}

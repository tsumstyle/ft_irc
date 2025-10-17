/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:28:56 by aroux             #+#    #+#             */
/*   Updated: 2025/09/30 17:40:55 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

void Server::InvalidCmd(Client *c, const ParsedCmd &data) {
	if (data.cmd.empty())
		return;
	c->sendMessage(Replies::ERR_UNKNOWNCOMMAND(c->getNick(), data.cmd));	
}

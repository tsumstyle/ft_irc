/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:28:56 by aroux             #+#    #+#             */
/*   Updated: 2025/09/30 14:58:28 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

void Server::InvalidCmd(Client *c, const ParsedCmd &data) {
	std::string token = data.cmd;
	std::string reply = "Invalid command: " + token + "\r\n";
	c->sendMessage(reply);	
}
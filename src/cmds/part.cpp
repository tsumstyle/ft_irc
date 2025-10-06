/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 13:24:38 by aroux             #+#    #+#             */
/*   Updated: 2025/10/06 15:15:36 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

/* test:
PART single channel
PART multiple channels
PART with reason
PART from channel you're not in */

void	Server::handlePart(Client *c, const ParsedCmd &data) {
	if (c->getState() != REGISTERED) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "PART"));
		return ;
	}
	if (data.args.empty()) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), "PART"));
		return ;
	}
	std::string	reason;
	if (data.args.size() > 1 && data.lastTokenHasColon) {
		reason = data.args[1];			
		for (size_t	i = 2; i < data.args.size(); i++)							// if there are more args. append them to the reason
			reason += " " + data.args[i];
	}
	std::vector<std::string>	channelNames = split(data.args[0], ',');		// multiple channels
	std::vector<Channel*>		channelsToPart;
// validate each channel and collect valid ones
	for (size_t i = 0; i < channelNames.size(); i++) {							
		std::string	channelName = channelNames[i];
		if (channelName[0] != '#') {
			c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), data.args[i]));
			continue;
		}
		Channel *ch = findChannel(channelName);
		if (!ch) {
			c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), channelName));
			continue ;
		}	
		if (!ch->hasUser(c)) {
			c->sendMessage(Replies::ERR_NOTONCHANNEL(c->getNick(), ch->getName()));
			continue ;
		}
		channelsToPart.push_back(ch);
	}
// process valid channels
	for (size_t j = 0; j < channelsToPart.size(); j++) {
		partFromChannel(c, channelsToPart[j], reason);
	}			
}

void	Server::partFromChannel(Client* c, Channel* channel, const std::string& reason) {
		std::string reply = ":" + c->getSource() + " PART " + channel->getName();	// replace @host part with getSource() function
		if (!reason.empty()) 
			reply += " :" + reason;
		reply += "\r\n";
		channel->broadcast(reply, NULL);
		channel->removeUser(c);
		channel->removeOperator(c);
		c->removeChannel(channel);
		if (channel->getUsers().empty()) {
			_channels.erase(channel->getName());
		}	
}

/* Basic Functionality
    Purpose: Remove a client from one or more channels
    Syntax: PART <channel>[,<channel>...] [:<reason>]
    Authentication: Client must be registered (not restricted by channel modes)

Parameter Handling
    Minimum parameters: At least one channel name required
    Multiple channels: Support comma-separated channel list
    Optional reason: Reason text after the colon (can be empty)
Channel Validation
    Valid channel names: Must start with # or & and follow naming conventions
    Channel existence: Verify each channel exists on the server
    Case sensitivity: Channel names are case-insensitive (convert to consistent case)
Membership Checks
    Client in channel: Client must be a member of each channel they're trying to leave
    Error handling: Proper errors if client tries to leave channels they're not in
Notification System
    Broadcast PART message: Send PART message to ALL members of each channel
    Message format: :nick!user@host PART #channel :reason
    Exclude sender: Don't send the message back to the client who's leaving
Cleanup Operations
    Remove from channel: Remove client from channel's member list
    Remove operator status: If client was operator, remove from operators list
    Clean invitations: Remove client from channel's invitation list
    Channel deletion: If channel becomes empty, delete the channel object entirely
Error Responses
    461 ERR_NEEDMOREPARAMS - Not enough parameters
    403 ERR_NOSUCHCHANNEL - Channel doesn't exist
    442 ERR_NOTONCHANNEL - Client not in specified channel
    451 ERR_NOTREGISTERED - Client not registered */
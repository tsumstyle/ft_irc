/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:30:49 by aroux             #+#    #+#             */
/*   Updated: 2025/09/25 16:16:39 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

#include <string>
#include <vector>
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

/* Replies from server to client (confirmation or error message) when client inputs command 
	follow the IRC reply code (eg: ERR_ALREADYREGISTERED, RPL_JOIN) and format.
	We can add color to it to make it cute */
namespace Replies {
// command replies
	inline std::string	RPL_WELCOME(const std::string& nick, const std::string& user) {
		return "001 " + nick + " :Welcome to the IRC network, " + nick + "!" + " (user: " + user + ")\r\n";
	}

	inline std::string	RPL_YOURHOST(const std::string& nick) {
		const std::string	serverName = "InstantRegretChat";
		const std::string	serverVersion = "ft_irc_1.0";
		return ":" + serverName + " 002 " + nick + " :Your host is " + serverName +
		", running version " + serverVersion + "\r\n";
	}
	
	inline std::string	RPL_JOIN(const std::string& nick, const std::string& channel) {
		return ":" + nick + " JOIN #" + channel + "\r\n";
	}

	inline std::string RPL_NAMES(Client *c, const std::string& ch, const std::string& users){
		std::string reply;
		const std::string	serverName = "InstantRegretChat";
		reply = ":" + serverName + " 353 " + c->getNick() + " = " + ch + " :" + users + "\r\n";
		reply += ":" + serverName + " 366 " + c->getNick() + " " + ch + " :End of /NAMES list.\r\n";
		return reply;
	}

	inline std::string	RPL_PRIVMSG(const std::string& sender, const std::string& target, const std::string& msg) {
		return ":" + sender + " PRIVMSG " + target + " :" + msg + "\r\n"; 
	}

// error replies
	inline std::string	ERR_NOSUCHNICK(const std::string& nick, const std::string& target) {
		return "401 " + nick + " " + target + " :No such nickname \r\n";
	}

	inline std::string	ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel) {
		return "403 " + nick + " " + channel + " :No such channel \r\n";
	}

	inline std::string	ERR_NICKNAMEINUSE(const std::string& nick) {
		return "433 " + nick + " :Nickname is already in use\r\n";
	}

	inline std::string	ERR_ERRONEUSNICKNAME(const std::string& badnick) {
		return "432 " + badnick + " :Erroneus nickname\r\n";
	}

	inline std::string	ERR_NOTREGISTERED(const std::string& nick, const std::string& cmd) {
		return "451 " + nick + cmd + " :You have not registered\r\n";
	}

	inline std::string	ERR_USERONCHANNEL(const std::string& nick, const std::string& channel) {
		return "443 " + nick + " #" + channel + " :is already on channel\r\n";
	}

	inline std::string	ERR_NEEDMOREPARAMS(const std::string& nick, const std::string& cmd) {
		return "461 " + nick + " " + cmd + " :Not enough parameters\r\n";
	}

	inline std::string	ERR_ALREADYREGISTERED() {
		return "462 :You may not reregister\r\n";
	}

	inline std::string	ERR_PASSWMISMATCH() { // i think it's PASSWDMISMATCH ? with D
		return "464 :Password incorrect\r\n";
	}

	// added for chanop things:
	inline std::string	ERR_NOTONCHANNEL(const std::string &channel) {
		return "442 " + channel + " :You're not on that channel\r\n";
	}
	
	inline std::string	ERR_KEYSET(const std::string& channel) {
		return "467 " + channel + " :Channel key already set\r\n";
	}

	inline std::string	ERR_CHANNELISFULL(const std::string& channel) {
		return "471 " + channel + " :Cannot join channel (+l)\r\n";
	}

	inline std::string	ERR_INVITEONLYCHAN(const std::string& channel) {
		return "471 " + channel + " :Cannot join channel (+i)\r\n";
	}

	inline std::string	ERR_CHANOPPRIVSNEEDED(const std::string& channel) {
		return "471 " + channel + " :You're not channel operator\r\n";
	}
}

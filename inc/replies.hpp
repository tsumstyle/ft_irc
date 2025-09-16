/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:30:49 by aroux             #+#    #+#             */
/*   Updated: 2025/09/16 15:39:31 by aroux            ###   ########.fr       */
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
		const std::string	serverName = "Our cute IRC";
		const std::string	serverVersion = "ft_irc_1.0";
		return ":" + serverName + " 002 " + nick + " :Your host is " + serverName +
		", running version " + serverVersion + "\r\n";
	}
	
	inline std::string	RPL_JOIN(const std::string& nick, const std::string& channel) {
		return ":" + nick + " JOIN #" + channel + "\r\n";
	}

// error replies
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

	inline std::string	ERR_NEEDMOREPARAMS(const std::string& cmd) {
		return "461 " + cmd + " :Not enough parameters\r\n";
	}

	inline std::string	ERR_ALREADYREGISTERED() {
		return "462 :You may not reregister\r\n";
	}

	inline std::string	ERR_PASSWMISMATCH() {
		return "464 :Password incorrect\r\n";
	}

	inline std::string	ERR_BADCHANNAME(const std::string& nick, const std::string& channel) {
		return "403 " + nick + " " + channel + " :Invalid channel name \r\n";
	}

}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:30:49 by aroux             #+#    #+#             */
/*   Updated: 2025/11/03 12:52:23 by aroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

#include <string>
#include <vector>
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "colors.hpp"

#ifndef	SERVER_NAME
	#define SERVER_NAME "InstantRegretChat"
#endif

namespace Replies {
	inline std::string	RPL_WELCOME(const std::string& nick, const std::string& user) {
		return bg_blue("Welcome to the IRC network, " + nick + "!" + " (user: " + user + ")\r\n\n");
	}

	inline std::string	RPL_YOURHOST(const std::string& nick) {
		const std::string	serverName = SERVER_NAME;
		const std::string	serverVersion = "ft_irc_1.2";
		return blue(":" + serverName + " 002 " + nick + " :") + "Your host is " + blue(serverName) +
		", running version " + blue(serverVersion) + "\r\n";
	}
	
	inline std::string	RPL_JOIN(const std::string& nick, const std::string& channel) {
		return ":" + nick + " JOIN #" + yellow(channel) + "\r\n";
	}

	inline std::string RPL_NAMES(Client *c, const std::string& ch, const std::string& users){
		std::string reply;
		const std::string	serverName = SERVER_NAME; 		
		reply = ":" + serverName + " 353 " + c->getNick() + " = " + ch + " :" + users + "\r\n";
		reply += ":" + serverName + " 366 " + c->getNick() + " " + ch + " :End of /NAMES list.\r\n";
		return reply;
	}

	inline std::string	RPL_PRIVMSG(const std::string& sender, const std::string& target, const std::string& msg) { 
		return green(":" + sender + " PRIVMSG " + target + " :") + msg + "\r\n"; 
	}

	inline std::string	RPL_TOPIC(Channel *channel) {
		return yellow(channel->getName() + " Topic") + " :" + channel->getTopic() + "\r\n";
	}

	inline std::string	RPL_NOTOPIC(Channel *channel) {
		return yellow(channel->getName() + " Topic") + " :" + " No topic is set\r\n";
	}

/******************\ 
*      ERRORS      *
\******************/
	inline std::string	ERR_NOSUCHNICK(const std::string& nick, const std::string& target) {
		return bg_red("401 " + nick + " " + target + " :") + red("No such nickname\r\n");
	}

	inline std::string	ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel) {
		return bg_red("403 " + nick + " " + channel + " :") + red("No such channel\r\n");
	}

	inline std::string	ERR_UNKNOWNCOMMAND(const std::string& nick, const std::string& cmd) {
		return bg_red("421 " + nick + " " + cmd + " :") + red("Unknown command\r\n");
	}

	inline std::string	ERR_ERRONEUSNICKNAME(const std::string& badnick) {
		return bg_red("432 " + badnick + " :") + red("Erroneus nickname\r\n");
	}

	inline std::string	ERR_NICKNAMEINUSE(const std::string& nick) {
		return bg_red("433 " + nick + " :") + red("Nickname is already in use\r\n");
	}

	inline std::string	ERR_ERRONEUSUSERNAME(const std::string& baduser) {
		return bg_red("434 " + baduser + " :") + red("Erroneus username\r\n");
	}

	inline std::string ERR_NOTONCHANNEL(const std::string& nick, const std::string& channel) {
		return bg_red("442 " + nick + " " + channel + " :") + red("You're not on that channel\r\n");
	}

	inline std::string	ERR_NOTONCHANNEL(const std::string &channel) {
		return bg_red("442 " + channel + " :") + red("You're not on that channel\r\n");
	}

	inline std::string	ERR_USERONCHANNEL(const std::string& nick, const std::string& channel) {
		return bg_red("443 " + nick + " " + channel + " :") + red("is already on channel\r\n");
	}

	inline std::string	ERR_NOTREGISTERED(const std::string& nick, const std::string& cmd) {
		return bg_red("451 " + nick + " " + cmd + " :") + red("You have not registered\r\n");
	}

	inline std::string	ERR_NEEDMOREPARAMS(const std::string& nick, const std::string& cmd) {
		return bg_red("461 " + nick + " " + cmd + " :") + red("Not enough parameters\r\n");
	}

	inline std::string	ERR_ALREADYREGISTERED() {
		return bg_red("462 :") + red("You may not reregister\r\n");
	}

	inline std::string	ERR_PASSWDMISMATCH() {
		return bg_red("464 :") + red("Password incorrect\r\n");
	}

	inline std::string	ERR_KEYSET(const std::string& channel) {
		return bg_red("467 " + channel + " :") + red("Channel key already set\r\n");
	}

	inline std::string	ERR_CHANNELISFULL(const std::string& channel) {
		return bg_red("471 " + channel + " :") + red("Cannot join channel (+l)\r\n");
	}

	inline std::string	ERR_INVITEONLYCHAN(const std::string& channel) {
		return bg_red("473 " + channel + " :") + red("Cannot join channel (+i)\r\n");
	}

	inline std::string	ERR_CHANOPRIVSNEEDED(const std::string& channel) {
		return bg_red("482 " + channel + " :") + red("You're not channel operator\r\n");
	}

	inline std::string ERR_BADCHANMASK(const std::string& nick, const std::string& channel) {
		return bg_red("476 " + nick + " " + channel + " :") + red("Invalid channel mask\r\n");
	}
}

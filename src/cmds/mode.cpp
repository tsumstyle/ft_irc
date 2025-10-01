#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"



void	Server::handleMode(Client *c, const ParsedCmd &data) {

	std::string	reply;
	if (data.args.size() < 3) {
		reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd);
	}
	
	// checking channel and privileges
	Channel *chan = findChannel(data.args[1]);
	if (!chan) {
		reply = Replies::ERR_NOSUCHCHANNEL(c->getNick(), chan->getName());
	}
	else if (!chan->findUser(c->getNick())) {
		reply = Replies::ERR_NOTONCHANNEL(chan->getName());
	}
	else if (!chan->isOperator(c)) {
		reply = Replies::ERR_CHANOPRIVSNEEDED(chan->getName());
	}

	// actual things
	if (data.args[2] == "+i") {
		if (data.args.size() == 3) {
			chan->setInviteOnly(true);
			reply = "Channel set to invite only\r\n";
		}
	}
	else if (data.args[2] == "-i") {
		if (data.args.size() == 3) {
			chan->setInviteOnly(false);
			reply = "Channel no longer invite only\r\n";
		}
	}
	else if (data.args[2] == "+t") {
		if (data.args.size() == 3) {
			chan->setTopicRestricted(true);
			reply = "TOPIC restricted to channel operators\r\n";
		}
	}
	else if (data.args[2] == "-t") {
		if (data.args.size() == 3) {
			chan->setTopicRestricted(false);
			reply = "TOPIC no longer restricted to channel operators\r\n";
		}
	}
	else if (data.args[2] == "+o") {
		if (data.args.size() < 4) {
			reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd);
		}
		Client* target = chan->findUser(data.args[3]);
		if (target && !chan->isOperator(target)) {
			chan->addOperator(target);
			reply = "Added " + target->getNick() + " as an operator\r\n";
		}
	}
	else if (data.args[2] == "-o") {
		if (data.args.size() < 4) {
			reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd);
		}
		Client* target = chan->findUser(data.args[3]);
		if (target && chan->isOperator(target)) {
			chan->removeOperator(target);
			reply = "Removed " + target->getNick() + " as an operator\r\n";
		}
	}
	else if (data.args[2] == "+k") {
		if (data.args.size() < 4) {
			reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd);
		}
		if (!chan->isReqPassword()) {
			chan->setLocalPass(data.args[3]);
			chan->setReqPassword(true);
			reply = "Password is now required\r\n";
		}
		else if (chan->isReqPassword()) {
			reply = Replies::ERR_KEYSET(chan->getName());
		}
	}
	else if (data.args[2] == "-k") {
		if (data.args.size() == 3) {
			chan->setReqPassword(false);
			reply = "Password no longer required\r\n";
		}
	}
	else if (data.args[2] == "+l") {
		if (data.args.size() < 4) {
			reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd);
		}
		if (!chan->isUserLimitSet()) {
			chan->setUserLimit(data.args[3]);
			chan->setUserLimitSet(true);
			reply = "User limit set to " + data.args[3] + "\r\n";
		}
		else if (chan->isUserLimitSet()) {
			reply = "User limit is already set\r\n";
		}
	}
	else if (data.args[2] == "-l") {
		if (data.args.size() == 3) {
			chan->setUserLimitSet(false);
			reply = "User limit removed\r\n";
		}
	}
	else {
		reply = "Unknown MODE command\r\n";
	}
	c->sendMessage(reply);
	return;
}

/*
MODE:
	- MODE <channel> [+|-] i t
	- MODE <channel> [+|-] o <user>
	- MODE <channel> +k <psswd>
	- MODE <channel> +l <limit>
	- MODE <channel> <unknown char>: ERR_UNKNOWNMODE (472): "472 <nick> <char> :is unknown mode char to me"

	check:
	- at least <channel> and option param. if not: ERR_NEEDMOREPARAMS
	- user is on the channel. if not: ERR_NOTONCHANNEL
	- user has chanop privileges. if not: ERR_CHANOPRIVSNEEDED

	- ERR_UMODEUNKNOWNFLAG (501) - Invalid user mode flag (for +i, +o, +w) : "501 :Unknown MODE flag"
	- ERR_USERSDONTMATCH (502) - Trying to change another user's modes (for +i, +o, +w) : "502 :Cannot change mode for other users"

	+i:
	- if !isInviteOnly() -> set to true
	- if already true -> ignore

	-i:
	- if isInviteOnly() -> set to false
	- if already false, ignore

	+t
	- if !isTopicRestricted() -> set to true
	- if already true -> ignore

	-t
	- if isTopicRestricted() -> set to false
	- if already false -> ignore

	+k
	- must be used /MODE <channel> +k <passwd>. if not: ERR_NEEDMOREPARAMS
	- if !isReqPassword() -> set to true and _localpass = <passwd>
	- if isReqPassword() -> leave true and set _localpass = <passwd>
	- ERR_KEYSET (467) - Channel already has a key set

	-k
	- if !isReqPasswd() -> ignore
	- if isReqPasswd() -> set to false, delete _localpass?

	+o
	- must be used /MODE <channel> +o <target>. if not: ERR_NEEDMOREPARAMS
	- check if target is part of channel. if not: ERR_NOTONCHANNEL
	- add target to list of chanops

	-o
	- must be used /MODE <channel> -o <target>. if not: ERR_NEEDMOREPARAMS
	- check if target is part of channel. if not: ?
	- remove target from list of chanops

	+l
	- must be used /MODE <channel> +l <limit>. if not: ERR_NEEDMOREPARAMS
	- if !isUserLimitSet() -> set to true and _userLimit = <limit>
	- if already true -> ignore
	- ERR_UNKNOWNMODE (472) - Invalid limit value (non-numeric)

	-l
	- if already false -> ignore
	- if isUserLimitSet() -> set to false

 */


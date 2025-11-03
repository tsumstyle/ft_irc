#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

std::string	Server::handleMode_channel(Client *c, const ParsedCmd& data) {
	std::string reply;
	Channel *chan = findChannel(data.args[0]);
	if (!chan) {
		std::cout << "!chan\n";
		reply = Replies::ERR_NOSUCHCHANNEL(c->getNick(), data.args[0]);
	}
	else if (!chan->findUser(c->getNick())) {
		reply = Replies::ERR_NOTONCHANNEL(chan->getName());
	}
	else if (!chan->isOperator(c)) {
		reply = Replies::ERR_CHANOPRIVSNEEDED(chan->getName());
	}
	else {
		if (data.args[1] == "+i") {
			if (data.args.size() == 2) {
				chan->setInviteOnly(true);
				reply = yellow("Channel " + chan->getName() + " set to invite only\r\n");
			}
		}
		else if (data.args[1] == "-i") {
			if (data.args.size() == 2) {
				chan->setInviteOnly(false);
				reply = yellow("Channel " + chan->getName() + " no longer invite only\r\n");
			}
		}
		else if (data.args[1] == "+t") {
			if (data.args.size() == 2) {
				chan->setTopicRestricted(true);
				reply = yellow("TOPIC on " + chan->getName() + " restricted to channel operators\r\n");
			}
			else
				reply = red("TOPIC +t : too many arguments\r\n");
		}
		else if (data.args[1] == "-t") {
			if (data.args.size() == 2) {
				chan->setTopicRestricted(false);
				reply = yellow("TOPIC on " + chan->getName() + " no longer restricted to channel operators\r\n");
			}
			else
				reply = red("TOPIC -t : too many arguments\r\n");
		}
		else if (data.args[1] == "+o") {
			if (data.args.size() < 3) {
				reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd);
			}
			Client* target = chan->findUser(data.args[2]);
			if (!target)
				reply = Replies::ERR_NOSUCHNICK(data.args[2], data.cmd);
			else if (target && !chan->isOperator(target)) {
				chan->addOperator(target);
				reply = yellow("Added " + target->getNick() + " as an operator on " + chan->getName() + "\r\n");
				target->sendMessage(yellow("You've been made an operator in " + chan->getName() + "\r\n"));
			}
		}
		else if (data.args[1] == "-o") {
			if (data.args.size() < 2) {
				reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd);
			}
			Client* target = chan->findUser(data.args[2]);
			if (target && chan->isOperator(target)) {
				chan->removeOperator(target);
				reply = yellow("Removed " + target->getNick() + " as an operator on " + chan->getName() + "\r\n");
			}
		}
		else if (data.args[1] == "+k") {
			if (data.args.size() < 3) {
				reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd);
			}
			if (!chan->isReqPassword()) {
				chan->setLocalPass(data.args[2]);
				chan->setReqPassword(true);
				reply = yellow("Password is now required on " + chan->getName() + "\r\n");
			}
			else if (chan->isReqPassword()) {
				reply = Replies::ERR_KEYSET(chan->getName());
			}
		}
		else if (data.args[1] == "-k") {
			if (data.args.size() == 2) {
				chan->setReqPassword(false);
				reply = yellow("Password no longer required on " + chan->getName() + "\r\n");
			}
		}
		else if (data.args[1] == "+l") {
			if (data.args.size() < 3) {
				reply = Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd);
			}
			if (!chan->isUserLimitSet()) {
				chan->setUserLimit(data.args[2]);
				chan->setUserLimitSet(true);
				reply = yellow("User limit set to " + data.args[2] + " on " + chan->getName() + "\r\n");
			}
			else if (chan->isUserLimitSet()) {
				reply = yellow("User limit is already set\r\n");
			}
		}
		else if (data.args[1] == "-l") {
			if (data.args.size() == 2) {
				chan->setUserLimitSet(false);
				reply = yellow("User limit removed on " + chan->getName() + "\r\n");
			}
		}
		else {
			reply = red("Unknown MODE command\r\n");
		}
	}
	return (reply);
}

void	Server::handleMode(Client *c, const ParsedCmd &data) {
	std::string	reply;
	if (c->getState() != REGISTERED) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), data.cmd));
		return;
	}
	if (data.args.empty()) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd));
		return;
	}
	if (data.args.size() < 2) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd));
		return;
	}
	if (data.args[0][0] == '#') {
		reply = handleMode_channel(c, data);
	}
	c->sendMessage(reply);
	return;
}


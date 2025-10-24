#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

void	Server::handleKick(Client *c, const ParsedCmd &data) {
	if (c->getState() != REGISTERED) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), data.cmd));
		return;
	}
	if (data.args.size() < 2) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd));
		return ;
	}
	Channel *chan = findChannel(data.args[0]);
	if (!chan) {
		c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), data.args[0]));
		return ;
	}
	if (!chan->isOperator(c)) {
		c->sendMessage(Replies::ERR_CHANOPRIVSNEEDED(chan->getName()));
		return ;
	}
	Client *victim = chan->findUser(data.args[1]);
	if (!victim) {
		c->sendMessage(Replies::ERR_NOSUCHNICK(c->getNick(), data.args[1])); /// change
		return ;
	}
	std::string reason = "";
	if (data.args.size() > 2) {
		for (size_t i = 2; i < data.args.size(); i++) {
			if (i != 2)
				reason += " ";
			reason += data.args[i];
		}
	}
	partFromChannel(victim, chan, reason);

	// here some placeholder messages
	c->sendMessage("User " + victim->getNick() + " kicked from channel " + chan->getName() + " >:)\r\n"); ///// change
	
	std::string explanation = "You've been kicked from " + chan->getName();
	if (reason != "")
		explanation += ". Reason given: " + reason + "\r\n";
	victim->sendMessage(explanation);
}

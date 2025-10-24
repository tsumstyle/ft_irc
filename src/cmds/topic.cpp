#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

void	Server::handleTopic(Client *c, const ParsedCmd &data) {
	std::string	reply;
	if (c->getState() != REGISTERED) {
		c->sendMessage(Replies::ERR_NOTREGISTERED(c->getNick(), "TOPIC"));
		return ;
	}
	if (data.args.size() < 1) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd));
		return ;
	}
	Channel *chan = findChannel(data.args[0]);
	if (!chan) {
		c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), data.args[0]));
		return ;
	}
	if (data.args.size() == 1) {
		if (chan->getTopic() == "")
			c->sendMessage(Replies::RPL_NOTOPIC(chan));
		else
			c->sendMessage(Replies::RPL_TOPIC(chan));
		return ;
	}
	if (chan->isTopicRestricted() && !chan->isOperator(c)) {
		c->sendMessage(Replies::ERR_CHANOPRIVSNEEDED(chan->getName()));
		return ;
	}
	std::string	new_topic = "";
	for (size_t i = 1; i < data.args.size(); i++) {
		if (i != 1)
			new_topic += " ";
		new_topic += data.args[i];
	}
	chan->setTopic(new_topic);
	c->sendMessage(yellow(chan->getName()) + " :Topic changed\r\n");
}

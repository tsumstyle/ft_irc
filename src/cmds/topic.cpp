#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

/*
TOPIC:
	1) - /TOPIC <channel> 
	2) - /TOPIC <channel> <topic>

	1) returns current topic

	2) check:
	if channel.isTopicRestricted {
		if (channel.topicRestricted && !chanop)
			-> privileges needed
		else
			-> change topic
*/

void	Server::handleTopic(Client *c, const ParsedCmd &data) {
	std::string	reply;
	if (data.args.size() < 1) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd));
		return ; // <----
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
	// 2 or more args
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
	c->sendMessage(yellow(chan->getName()) + " :Topic changed\r\n"); //// change
}

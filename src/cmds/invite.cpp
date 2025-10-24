/*
Decisions: chanops can always invite people, even when channel is NOT inviteonly.
Joining a channel always removes user's name from invitelist, if it is there.
When channel is inviteonly, people not on invitelist cannot join.
*/

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handleInvite(Client *c, const ParsedCmd &data) {
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
	else if (!chan->findUser(c->getNick())) {
		c->sendMessage(Replies::ERR_NOTONCHANNEL(chan->getName()));
		return ;
	}
	else if (!chan->isOperator(c)) {
		c->sendMessage(Replies::ERR_CHANOPRIVSNEEDED(chan->getName()));
		return ;
	}
	Client *target = findClientByNick(data.args[1]);
	if (!target) {
		c->sendMessage(Replies::ERR_NOSUCHNICK(c->getNick(), data.args[1]));
		return ;
	}
	else if (chan->findUser(target->getNick())) {
		c->sendMessage(Replies::ERR_USERONCHANNEL(c->getNick(), chan->getName()));
		return ;
	}

	chan->invite(target);
	c->sendMessage("Invited " + target->getNick() + " to " + chan->getName() + "\r\n");
	target->sendMessage("You have been invited to join " + chan->getName() + "\r\n");
}

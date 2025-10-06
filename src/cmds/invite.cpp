/*
Decisions: chanops can always invite people, even when channel is NOT inviteonly.
Joining a channel always removes user's name from invitelist, if it is there.
When channel is inviteonly, people not on invitelist cannot join.
*/

#include "../../inc/Server.hpp"
#include "../../inc/replies.hpp"

void	Server::handleInvite(Client *c, const ParsedCmd &data) {
	// INVITE <channel> <target>

	// args < 3?
	if (data.args.size() < 2) {
		c->sendMessage(Replies::ERR_NEEDMOREPARAMS(c->getNick(), data.cmd));
		return ;
	}
	// channel exists?
	Channel *chan = findChannel(data.args[0]);
	if (!chan) {
		c->sendMessage(Replies::ERR_NOSUCHCHANNEL(c->getNick(), data.args[0]));
		return ;
	}
	// c member of channel?
	else if (!chan->findUser(c->getNick())) {
		c->sendMessage(Replies::ERR_NOTONCHANNEL(chan->getName()));
		return ;
	}
	// c->chanop?
	else if (!chan->isOperator(c)) {
		c->sendMessage(Replies::ERR_CHANOPRIVSNEEDED(chan->getName()));
		return ;
	}
	// target exists?
	Client *target = findClientByNick(data.args[1]);
	if (!target) {
		c->sendMessage(Replies::ERR_NOSUCHNICK(c->getNick(), data.args[1]));
		return ;
	}
	// target already in channel?
	else if (chan->findUser(target->getNick())) {
		c->sendMessage(Replies::ERR_USERONCHANNEL(c->getNick(), chan->getName())); // nick of c or of target? or both?
		return ;
	}

	chan->invite(target);
	c->sendMessage("Invited " + target->getNick() + " to " + chan->getName() + "\r\n");
	target->sendMessage("You have been invited to join " + chan->getName() + "\r\n");
}

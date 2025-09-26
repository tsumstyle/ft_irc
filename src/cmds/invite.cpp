/*
INVITE:
- usage: /INVITE <nickname> <channel>

- can only chanops invite people?
- if inviteonly mode true, can only chanops invite people?

check:
	- parameters. if not: ERR_MOREPARAMSNEEDED
	- if user already on channel: ERR_USERONCHANNEL

if channel.isInviteOnly() {
	if user.isChanOp() {
		-> invite user
	}
	else {
		-> ERR_CHANOPRIVSNEEDED
	}
}
else
	-> invite user

*/

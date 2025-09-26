/*
KICK:
	- kicks a user from a channel
	- usage: /KICK <channel> <target> [<comment>]

	check:
		- at least channel and target. if not: ERR_MOREPARAMSNEEDED
		- user is chanop. if not: ERR_CHANOPRIVSNEEDED
	
	- if target not on channel -> ignore
	- if target on channel -> remove from channel members. msg with comments
*/

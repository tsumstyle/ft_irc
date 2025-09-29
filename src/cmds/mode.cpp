#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

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

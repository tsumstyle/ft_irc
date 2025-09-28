#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/replies.hpp"

bool	valid_prefix(const std::string& cmd) {
	if (cmd[0] != '-' && cmd[0] != '+')
		return false;
	for (size_t i = 1; i < cmd.size(); i++) {
		if (cmd[i] == '-' || cmd[i] == '+')
			return false; // if theres another one
	}
	return true;
}

bool	is_in_str(const char c, const std::string& str) {
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == c)
			return true;
	}
	return false;
}

bool	valid_chars(const std::string& options) {
	for (size_t i = 1; i < options.size(); i++) {
		if (!is_in_str(options[i], "itokl"))
			return false; // not of valid chars
		if (is_in_str(options[i], options.substr(i + 1, options.size() - 1)))
			return false; // if its repeated?
	}
	return true;
}

void	Server::handleMode(Client *c, const ParsedCmd &data) {
	// - at least <channel> and option param. if not: ERR_NEEDMOREPARAMS
	// - user is on the channel. if not: ERR_NOTONCHANNEL
	// - user has chanop privileges. if not: ERR_CHANOPRIVSNEEDED
	Channel *chan = findChannel(data.args[1]);
	if (!c) { return; } // for compilation
	if (!chan) {
		// no such channel
		std::cout << "BAD INPUT: no such channel\n";
		return ;
	}
	if (data.args[2] == "+i") {
		// - if !isInviteOnly() -> set to true
		//- if already true -> ignore
	}
	else if (data.args[2] == "-i") {
		// - if isInviteOnly() -> set to false
		// - if already false, ignore
	}
	else if (data.args[2] == "+t") {
		// - if !isTopicRestricted() -> set to true
		// - if already true -> ignore
	}
	else if (data.args[2] == "-t") {
		// - if isTopicRestricted() -> set to false
		// - if already false -> ignore
	}
	else if (data.args[2] == "+o") {
		// - must be used /MODE <channel> +o <target>. if not: ERR_NEEDMOREPARAMS
		// - check if target is part of channel. if not: ?
		// - add target to list of chanops
	}
	else if (data.args[2] == "-o") {
		// - must be used /MODE <channel> -o <target>. if not: ERR_NEEDMOREPARAMS
		// - check if target is part of channel. if not: ?
		// - remove target from list of chanops
	}
	else if (data.args[2] == "+k") {
		// - must be used /MODE <channel> +k <passwd>. if not: ERR_NEEDMOREPARAMS
		// - if !isReqPassword() -> set to true and _localpass = <passwd>
		// - if isReqPassword() -> leave true and set _localpass = <passwd>
	}
	else if (data.args[2] == "-k") {
		// - if !isReqPasswd() -> ignore
		// - if isReqPasswd() -> set to false, delete _localpass?
	}
	else if (data.args[2] == "+l") {
		// - must be used /MODE <channel> +l <limit>. if not: ERR_NEEDMOREPARAMS
		// - if !isUserLimitSet() -> set to true and _userLimit = <limit>
		// - if already true -> ignore
	}
	else if (data.args[2] == "-l") {
		// - if already false -> ignore
		// - if isUserLimitSet() -> set to false
	}
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

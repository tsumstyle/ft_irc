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
		if client.isChanOp() {
			-> change topic
		}
		else {
			-> ERR_CHANOPRIVSNEEDED
		}
	}
	else
		-> change topic
*/

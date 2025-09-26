// KICK -- kick a user from a channel
// INVITE -- invite a user to a channal
// TOPIC -- set the channel's topic
// MODE -- 
// 		i: Set/remove Invite-only channel
// 		t: Set/remove the restrictions of the TOPIC command to channel operators
// 		k: Set/remove the channel key (password)
// 		o: Give/take channel operator privilege
// 		l: Set/remove the user limit to channel




 442     ERR_NOTONCHANNEL
                        "<channel> :You're not on that channel"

                - Returned by the server whenever a client tries to
                  perform a channel effecting command for which the
                  client isn't a member.

443     ERR_USERONCHANNEL
                        "<user> <channel> :is already on channel"

                - Returned when a client tries to invite a user to a
                  channel they are already on.


 464     ERR_PASSWDMISMATCH
                        ":Password incorrect"

                - Returned to indicate a failed attempt at registering
                  a connection for which a password was required and
                  was either not given or incorrect.


467     ERR_KEYSET
                        "<channel> :Channel key already set"

471     ERR_CHANNELISFULL
                        "<channel> :Cannot join channel (+l)"

473     ERR_INVITEONLYCHAN
                        "<channel> :Cannot join channel (+i)"

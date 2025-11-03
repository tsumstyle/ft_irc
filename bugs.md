- second user saw their own message when privmsg into their the new chat
- kick message and invite message were in one line:
	You've been kicked from #c1You have been invited to join #c1
- mode doest return if it has too many parameters:
	mode #c2 +t dont talk about c2
	topic #c2 
- mode should throw errors if the channel or the user cant be found
	MODE #C2 +o nn
- fix log message here:
	mode #c2 +l 1    
	User limit set to 1on #c2
- are you supposed to be able to change the key with this command, if one already exists?
	mode #c2 +k asldkfjasldjkfalsdkjf:
	467 #c2 :Channel key already set
- if password is wrong, disconnect the client
	pass ttt
	464 :Password incorrect
	pass lk
	464 :Password incorrect
- behavior for buffer overflow? Now we disconnect the client
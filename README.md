# InstantRegretChat (ircserv)

Lightweight IRC server in C++ (C++98) — minimal, educational implementation of common IRC features (clients connect to channels, moderation options), and following the official IRC protocol (RFC 1459). Group project created as part of the 42 curriculum.


## Quick start
Build:
```sh
make
# binary: ./ircserv
```

Run:
```sh
./ircserv <port> <password>
# example
./ircserv 8080 test
```

Connect from another terminal:
```sh
telnet 127.0.0.1 8080
# or
nc 127.0.0.1 8080
```

Register:
```
# password mandatory as first step:
PASS test
# then nickname and username: 
NICK amadeus
USER amadeus 0 * :Amadeus Example
```

---

## Features
- Basic IRC flow: PASS, NICK, USER registration
- Channel management: JOIN, PART, NAMES, LIST
- Messaging: PRIVMSG (channels and users)
- Server replies using numeric codes (RPL/ERR)
- Channel modes and operator (aka moderators) actions (KICK, INVITE, +o, +v, invite-only, user limit)
- PING/PONG heartbeat
- Colored console output and welcome banner (dev convenience)

---

## Commands implemented
PASS, NICK, USER, JOIN, PART, PRIVMSG, PING, PONG, QUIT, NAMES, MODE, KICK, INVITE, LIST

(See handlers in `src/cmds/` for behaviour and permission checks.)

---

## Project layout (important files)
- `Makefile` — build targets
- `src/` — implementation
  - `main.cpp` — entry point
  - `Server*`, `Client*`, `Channel*` — server core
  - `parser.cpp` — message parsing
  - `cmds/` — individual command handlers (join, nick, user, ping, ...)
- `inc/` — headers (Server.hpp, Client.hpp, Channel.hpp, replies.hpp, ...)
- `test.cpp` — manual test notes / snippets

---

## Development notes
- Targeted for C++98; build flags in `Makefile` enforce warnings (`-Wall -Wextra -Werror`).
- Add a new command:
  1. Implement handler in `src/cmds/`.
  2. Register / dispatch it in `Server::handleCmd`.
  3. Add numeric replies in `inc/replies.hpp` if needed.
- Parser: `parseMsg` returns a structured command; canonicalize case where appropriate.


## Resources
- Official IRC specification (RFC 1459): [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- Practical reference: [RFC 2812 — Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- Beej’s guide to network programming: https://beej.us/guide/bgnet/html/split/
- setup basic local server: https://ncona.com/2019/04/building-a-simple-server-with-cpp/
- setup basic server: https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
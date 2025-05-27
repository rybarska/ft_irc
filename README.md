27/05/2005: here is about 30% of the mandatory part of ft__irc.

Reference: the newest version of the "Beej's Guide to Network Programming", https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf

How you can test it at the moment:
* Terminal 1:
```bash
make
./ircserv <port> <password>
```
* Terminal 2 (or more):
```bash
telnet localhost <port>
PASS <password>
NICK nick
USER user
```

😁️  WHAT’S IMPLEMENTED

Command-line parsing with argument validation <port> <password>.

Server socket setup using getaddrinfo(), socket(), bind(), and listen() (IPv4/IPv6-compatible).

Graceful accept() of new clients, added to a pollfd array and tracked with client FDs.

Basic I/O loop using poll(), handling incoming messages and connection drops.

Ring buffer (circular buffer) input parsing per client with line-by-line processing.

Message parsing via the Message class.

Command dispatch via _cmdControl.processCommand().

Authentication hooks: attemptAuth() and attemptRegistration() stubs are in place.


🤔️  WHAT STILL NEEDS TO BE DONE

User & channel state management: Implement structures and logic for managing nicknames, usernames, channels, modes, etc.

Full command support: Only partial or no handling for standard IRC commands (JOIN, PRIVMSG, NICK, etc.).

Authentication enforcement: Actually check passwords and reject invalid login attempts.

Graceful shutdown:

Handle SIGINT or similar signals to cleanly close connections and free memory.

Send QUIT or ERROR messages to clients before disconnecting.

Input sanitization:

Prevent malformed command crashes.

Logging: Replace std::cout/cerr with a proper logging or debug mode.

Anti-flood/rate-limiting: Prevent clients from spamming commands.

Cleanup of disconnected clients: Currently done ad hoc in processClientInput(); could be cleaner/more robust.


## ⚠️ Security Disclaimer

**This project (`ft_irc`) is for educational purposes only.**

Due to project constraints, including:

- ❌ No external libraries (e.g., OpenSSL, libsodium)
- ❌ No use of standard cryptographic tools
- ❌ No TLS/SSL for encrypted connections

it is **not possible** to implement secure authentication, password handling, or data transmission in a way that meets real-world security standards.

Any password storage, verification, or data transmission in this project should be considered **insecure** and vulnerable to common attacks (e.g., sniffing, spoofing, replay, brute force).

The focus of this project is on learning how IRC-like protocols and server/client architecture work — **not** on implementing production-grade security.

Cross-cutting TODOs

TODO: Implement full IRC registration flow, including NICK and USER validation.

TODO: Implement command routing for commands like JOIN, PRIVMSG, PART, QUIT, MODE, TOPIC, etc.

TODO: Implement channel management: create, join, part, topic, modes, user lists, etc.

TODO: Implement message broadcasting to channels and users.

TODO: Implement server replies (numerics) and errors properly, following the IRC RFC.

TODO: Validate nicknames and usernames according to the spec.

TODO: Add logging framework or verbosity levels.

TODO: Add unit tests / integration tests to cover connection handling, command parsing, etc.

TODO: Implement flood protection / ping timeout detection (to disconnect idle clients).

TODO: Implement support for multiple connections with proper user tracking.

TODO: Handle QUIT command to clean up user state and inform channels.

TODO: Handle nickname collisions and enforce uniqueness.


## ⚠️ Security Disclaimer

**This project (`ft_irc`) is for educational purposes only.**

Due to project constraints, including:

- ❌ No external libraries (e.g., OpenSSL, libsodium)
- ❌ No use of standard cryptographic tools
- ❌ No TLS/SSL for encrypted connections

it is **not possible** to implement secure authentication, password handling, or data transmission in a way that meets real-world security standards.

Any password storage, verification, or data transmission in this project should be considered **insecure** and vulnerable to common attacks (e.g., sniffing, spoofing, replay, brute force).

The focus of this project is on learning how IRC-like protocols and server/client architecture work — **not** on implementing production-grade security.

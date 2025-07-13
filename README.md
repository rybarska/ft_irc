ft_irc is a simplified IRC (Internet Relay Chat) server written in C++98 as part of the 42 curriculum. It supports basic client-server communication, multi-client handling using poll(), IRC command parsing, and more — all built without external libraries or frameworks.

📌 Educational focus: This project is about learning networking, parsing, and server-client architectures — not production-ready security or IRC compliance.

The server is stable, supports multiple clients, and handles core IRC commands. We also added a spam bot for testing, a robust signal handler to prevent crashes, and a file transfer feature.

⚙️ HOW TO RUN IT

* Use "make" to compile the server, "make bot" to compile the bot, and "make re" to compile both

In one terminal:
```bash
make re
./ircserv <port> <password>
```

In another terminal (or several):
```bash
telnet localhost <port>
PASS <password>
NICK mynick
USER myuser 0 * :My Real Name
```
Or test it with our bot (optional):
```bash
./ircbot <host> <port> <password>
```


🧩 Server & Client Core

🔌 Server setup

Uses getaddrinfo(), socket(), bind(), listen(), and poll() for cross-platform IPv4/IPv6 compatibility.

Gracefully accepts new clients and tracks them in a dynamic pollfd array.

🔁 Ring Buffer per client

Each client has a circular buffer to read incoming data line-by-line without truncation or overflow.

✉️ Message parsing

The Message class extracts IRC command name, parameters, and trailing text from each client input.

🧠 Command dispatch

Input lines are dispatched via _cmdControl.processCommand() to corresponding command handlers.

🔐 Signal handling

sigaction() is used to cleanly catch signals (e.g., SIGINT) and shut down the server without crashes.

🤖 Spam bot

A separate IRC bot connects and sends a flood of messages to test stability and rate-limiting.

🧑‍💻 IRC Functionality

🗣️ IRC Commands

Full support for core IRC commands: PASS, NICK, USER, JOIN, PART, PRIVMSG, NOTICE, PART, QUIT, etc.

Proper error messages and registration enforcement.

📁 File transfer

Implementation of DCC-like functionality for client-to-client file transmission over raw sockets.

👥 User & Channel Management

Data structures and logic to manage nicknames, usernames, channels, and user modes.

🧹 Client cleanup

Robust disconnect and cleanup logic to handle QUIT, timeouts, and unexpected drops.


## ⚠️ Security Disclaimer

**This project (`ft_irc`) is for educational purposes only.**

Due to project constraints, including:

- ❌ No external libraries (e.g., OpenSSL, libsodium)
- ❌ No use of standard cryptographic tools
- ❌ No TLS/SSL for encrypted connections

it is **not possible** to implement secure authentication, password handling, or data transmission in a way that meets real-world security standards.

Any password storage, verification, or data transmission in this project should be considered **insecure** and vulnerable to common attacks (e.g., sniffing, spoofing, replay, brute force).

The focus of this project is on learning how IRC-like protocols and server/client architecture work — **not** on implementing production-grade security.

📚 REFERENCES

Beej's Guide to Network Programming (latest version):
📖 https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf

RFC 1459 – Internet Relay Chat Protocol:
🧾 https://datatracker.ietf.org/doc/html/rfc1459

🧑‍💻 AUTHORS

Agata Rybarska – server architecture, signal handling, ring buffer, bot, and infrastructure

Ivan Baranov – IRC functionality, file transfer, and command handling

Contributions visible in Git history.

> 🤝 This project was built with great collaboration. Props to my partner for strong contributions and smooth teamwork.


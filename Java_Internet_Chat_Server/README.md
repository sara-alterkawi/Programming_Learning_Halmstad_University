# T2-Internet-Chat-Server
Develop a concurrent server / client for an Internet multi-party chat service (like IRC) using the TCP protocol.
Users run the client program, which connects to the server. Then users can send typed-in user messages to that server which then distributes them to the other clients.
Incoming messages can be distributed either directly (immediately) or alternatively the server can only inform the other clients about fresh messages and their origin. This way the client can decide whether to fetch the new message(s) or not.
The server should also inform all connected parties when one of the users losses the connection or logs out.
For the sake of simplicity, avoid writing any GUIs for the client (unless you really really really want to), concentrate on the networking / concurrency part and use a simple console input / output for the client. Also, you should use the basic networking and threads Java API for this.

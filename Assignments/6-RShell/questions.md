
1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_The remote client determines a commands output is fully recieved when it comes across the EOF character. The best technique to use is to make sure bytes recieved are greater than 0._

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_A shell protocol should define and connect over a TCP connection by making sure a delimiter is used. If it sees `\0` or `\n` then it knows that is the end of the command. Anything after it is the next command until another delimiter is found. The issue that can come with this is misinterpretation of messages. If messages are not ended correctly, the TCP may not understand what is being recieved._

3. Describe the general differences between stateful and stateless protocols.

_The general differences between stateful and stateless protocols revolve around how the information of the client is managed. For stateful protocols, the client state of the client is maintained while stateless does not. When it comes to sessions, each request in stateful protocols is dependant on the previous. In stateless, they are independant._

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_Though UDP is considered "unreliable" it is useful in scenarios when low latency and reduced overhead are more important._

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_The interface/abstraction provided by the OS is `socket`._

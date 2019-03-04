# Assignment 2
EECS325 Spring 2019
Yue Shu
Due: Tuesday, March 5, 2019

---

### 1. Under what circumstances can the HTTP response have empty body?

The HTTP response can have an empty body when there is no content to be sent in the response payload body. 
For example, HTTP response with `HTTP status 204` could have an empty body. 

### 2. Suppose Alice, with a Web-based e-mail account (such as Hotmail or Gmail), sends a message to Bob, who accesses his mail from his mail server using POP3. Discuss how the message gets from Alice’s host to Bob’s host. List the series of application-layer protocols that are used to move the message between the two hosts.

- Alice's web-based email user agent used HTTP to transfer the mail message from her user agent to her mail server
- The SMTP client side (Alice's mail server) transfers the mail message to the SMTP server side (Bob's mail server) with `SMTP protocol` over TCP connection
- Bob's user agent retrieve the message from mail server using POP3

### 3. List and explain the two different approaches for DNS resolving.

- **iterated query**:
  - Local DNS server is majorly in charge of generating all the requests to different servers
  - Requesting host sends request to the local DNS server
  - Local DNS server keeps querying other level of servers to search for the IP address if it does not have the cache of the IP address of a certain layer following the sequence below:
    - root DNS server
    - TLD DNS server
    - authorative DNS server
  - more secured because when attacked by hacker, only one way of information is hacked

- **recursive query**:
  - The contacted name servers are in charge of looking for the IP address and sending queries to each other
  - Heavy load at upper level layers: root server, TLD DNS server
  - Not secure since when attacked by hacker, amplification effect will  work

### 4. Is it possible for an organization’s Web server and mail server to have exactly the same alias for a host name (e.g., foo.com)? What would be the types for the RRs that contain the hostnames of the web and the mail servers?

Yes, it is possible for organization’s Web server and mail server to have exactly the same alias for a host name. 
- type for RR that contains the host name of the **web server**:
  - `type=A`
- type for RR that contains the host name of the **mail server**:
  - `type=MX`

### 5. Suppose TCP operates over a 1 Gbps link.

**a. Assume TCP could utilize the full bandwidth continuously and the average packet size is 60 bytes, how long does it take for the TCP sequence numbers to wrap around completely?**


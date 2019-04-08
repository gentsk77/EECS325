#Assignment 2

EECS325 Spring 2019
Yue Shu
Due: Thursday, April 11, 2019

###1. Three types of switching fabric are discussed in Section 4.2. List and briefly describe each type. Which, if any, can send multiple packets across the fabric in parallel?

- **switching via memory**
  - traditional computers with switching under direct control of CPU
  - datagrams are copied to system's memory
  - switching rate is limited by memory bandwidth (2 bus crossings per datagram)
- **switching via a bus**
  - switch datagram from input port memory to output port memory via a shared bus
  - all output ports will receive the datagram, but nly the correct one will keep it
  - switching speed is limited by bus bandwidth
- **switching via interconnection network (crossbar)**
  - switch datagrams via 2N buses that connect N input ports to N output ports
  - each verticle bus intersects with each horizontal bus at an intersection that is controlled by the fabric controller
  - fragment datagram into cells of fixed length and switch these cells through the fabric 
  - datagrams to be forwarded will not be blocked by datagrams being forwarded to the other ports 
  - overcome bus bandwidth limitations 
- only **switching via interconnction network** can send multiple datagrams across the fabric in parallel

###2. What is HOL blocking? Does it occur in input ports or output ports?

HOL (Head of the Line) blocking: queued datagram at front of the queue prevents the following ones in the queue from moving forward. 
HOL occurs in input ports. 

###3. What fields in the IP header can be used to ensure that a packet is forwarded through no more than N routers?

The `time to live` field. It keeps track of the maximum number of remaining hops and decrements the value at each router.  

###4. When a large datagram is fragmented into multiple smaller datagrams, where are these smaller datagrams reassembled into a single larger datagram?

These smaller datagrams are only reassembled into a single larger datagram at their **final destination**, before it is passed to and processed by the transport layer.

###5. Consider the network below:

####a. Show the forwarding table in router A, such that all traffic destined to host H3 is forwarded through interface 3.



####*b. Can you write down a forwarding table in router A, such that all traffic from H1 destined to host H3 is forwarded through interface 3, while all traffic from H2 destined to host H3 is forwarded through interface 4?



###6. Consider a datagram network using 32-bit host addresses. Suppose a router has four links, numbered 0 through 3, and packets are to be forwarded to the link interfaces as follows:

![p6 image](/Images/p6.png)

####a. Provide a forwarding table that has five entries, uses longest prefix matching, and forwards packets to the correct link interfaces.



####b. Describe how your forwarding table determines the appropriate link interface for datagrams with destination addresses:

![p6b image](/Images/p6b.png)


###7. Consider a router that interconnects three subnets: Subnet 1, Subnet 2, and Subnet 3. Suppose all of the interfaces in each of these three subnets are required to have the prefix 223.1.17/24. Also suppose that Subnet 1 is required to support at least 60 interfaces, Subnet 2 is required to support at least 90 interfaces, and Subnet 3 is to support at least 12 interfaces. Provide three network addresses (of the form a.b.c.d/x) that satisfy these constraints.



###8. Consider sending a 2100-byte datagram into a link that has an MTU of 700 bytes. Suppose the original datagram is stamped with the identification number 422. How many fragments are generated? What are the values in the various fields in the IP datagram(s) generated related to fragmentation?



###9. Consider the network setup below. Suppose that the ISP instead assigns the router the address 24.34.112.236 and that the network address of the home network is 192.168.1/24.

####a. Assign addresses to all interfaces in the home network



####b. Suppose each host has two ongoing TCP connections, all to port 80 at host 128.119.41.85. Provide the six corresponding entries in the NAT translation table.

![p9b image](/Images/p9b.png)



###10. What is the difference between a forward table in the destination-based forwarding routers and OpenFlow’s flow table that we discussed in Section 4.4?
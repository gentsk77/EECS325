# Assignment 1
EECS325 Spring 2019
Yue Shu
Due: Tuesday, Feb 19, 2019

### 1. What are the advantages and disadvantages of circuit-switched network and packet-switched network?

**packet-switching**:
-  advantages:
   -  allow resources to be shared
   -  can accomodate more users to use network
   -  is simpler, no need for call setup 
-  disadvantages: 
   -  could have queueing delay if the arrival rate to link exceeds the transmission rate of link for a period of time
   -  if the buffer fills up, could have packets lost (dropped out)
   -  need reliable data transfer and congestion control 

**circuit-switching**: 
-  advantages:
   -  dedicated resources reserved for the user, thus can guarantee the performance 
-  disadvantages: 
   -  fewer users can be accomodated
   -  circuit segements remain idle if not in use, kind of a waste 

### 2. Consider sending a packet from a source host to a destination host over a fixed route. List the delay components in the end-to-end delay. Which of these delays are constant and which are variable?

There are four major delay components in one single nodal delay, which are processing delay, queueing delay, transmission delay, and propogation delay. Therefore, the end-to-end delay also consists of the four delays above, while the total length of delay would be accumulated according to the number of routers between the source host and the destination host. 

**constant delays**: 
- processing delay
  - very small amount of time, mostly less than microseconds, to examine the packet's header and check for bit errors
  - neglectable comparing to other sources of delays, thus I would classify it as a constant delay 
- propogation delay
  - $d_{prop} = d/s$, where $d$ is length of physical link and $s$ is propogation speed
  - since the packet follows a fixed route, $d$ remains constant
  - $s$ also remains constant since it is roughly $2\times 10^8 m/sec$ in most cases
  - thus propogation delay is also a constant delay 

**variable delays**:
- queueing delay
  - depends on the congestion level of the router, which can be expressed as $La/R$, where $L$ is the packet length in bits, $R$ is the link bandwidth, and $a$ is the average packet arrival rate
  - both $L$ and $a$ could be varied, thus this is a variable delay
- transmission delay
  - $d_{trans} = L/R$
  - depends on the total length of the packet in bits, which can be varied
  - thus is a variable delay 

### 3. What is the difference between transmission delay and propagation delay? Will the length of the packet affect the propagation delay and why?

**transmision delay**: 
- the time the router takes to push all of the packet's bits into the link 
- $d_{trans} = L/R$, where $L$ is the packet length in bits, and $R$ is the link bandwidth
- will be affected by the length of the packet

**propogation delay**:
- the time it takes the packets to travel in the link from one router to the next
- $d_{prop} = d/s$, where $d$ is length of physical link and $s$ is propogation speed
- thus will not be affected by the length of the packet

### 4. Consider the following institutional network system. Suppose the average object size is 100k bits and the average rate from browsers to origin servers is 15 requests/sec. (ignore the LAN delay in the following questions)

![problem 4 image](/Image/p4.png)

**Assume that $𝑅_{access} = 1.5 Mbps$ and the $RTT$ on the Internet side of the access link is $2 sec$.**

**1. Considering the queueing delay in the access router, the access delay could be calculated with the equation $D_{access} = \frac{R_{trans}}{(1-traffic~intensity)}$. $R_{trans}$ represents the transmission delay at the access link. Without web cache, what is the total average response time?**

The transmission delay of one request at the access link can be calculated as below:
$$R_{trans} = L/R_{access} = 100k~bits/1.5Mbps \approx 0.0651~sec$$
And the traffic intensity can be expressed as: 
$$traffic~intensity = La/R_{access} = \frac{100k~bits/request \times 15~request/sec}{1.5Mbps} \approx 0.977 $$
Thus the access delay for each request would be: 
$$D_{access} = \frac{R_{trans}}{(1-traffic~intensity)} = \frac{0.0651sec}{1 - 0.977} \approx 2.83 sec$$
And the total average response time for each request would be:
$$T_{response} = internet~delay + access~delay = 1RTT + D_{access} = 2sec + 2.83sec = 4.83sec$$

**2. Compare this result with the situation where $𝑅_{access}= 100 Mbps$.**

The new transmission delay of one request at the access link can be calculated as below:
$$R_{trans} = L/R_{access} = 100k~bits/100 Mbps \approx 9.77\times10^{-4}~sec$$
And the new traffic intensity can be expressed as: 
$$traffic~intensity = La/R_{access} = \frac{100k~bits/request \times 15~request/sec}{100Mbps} \approx 0.0146 $$
Thus the new access delay for each request would be: 
$$D_{access} = \frac{R_{trans}}{(1-traffic~intensity)} = \frac{9.77\times10^{-4}~sec}{1 - 0.0146} \approx 9.91\times10^{-4} sec$$
And the total average response time for each request would be:
$$T_{response} = internet~delay + access~delay = 1RTT + D_{access} = 2sec + 9.91\times10^{-4} sec \approx 2.00sec$$

### 5. Two hosts, A and B, are directly connected via a link 𝑅 = 1 Mbps. The distance between A and B is 10,000 kilometers and the propagation speed over the link is $2.5 × 10^8 m/s$.

**1. How long does it take to send a file of 20,000 bits from A to B?**


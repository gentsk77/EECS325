#Assignment 4

EECS325 Spring 2019
Yue Shu
Due: Tuesday, April 23, 2019

### 1. Compare and contrast link-state and distance-vector routing algorithms.



### 2. Consider the following network. With the indicated link costs, use Dijkstra’s shortest-path algorithm to compute the shortest path from x to all network nodes. Show the algorithm by computing a table similar to Slide 15 in Chapter 5.

![p2 image](/Images/p2.png)




### 3. Consider the network shown below, and assume that each node initially knows the costs to each of its neighbors. Consider the distance-vector algorithm and show the distance table entries at node z. (Show the distance table entries in each step)

![p3 image](/Images/p3.png)



### 4. Suppose we have the forwarding tables shown in the following table for nodes A and F, in a network where all links have cost 1. Give a diagram of the smallest network consistent with these tables.

![p4 image A](/Images/p4A.png)

![p4 image F](/Images/p4F.png)


### 5. Consider the network below. When the link cost c(x, y) increases from 4 to 60. Explain why it takes 44 iterations for the algorithm to stabilize. (Hint: show the distance table entries in each iteration)

![p5 image](/Images/p5.png)



### 6. Consider the network shown below. Suppose AS3 and AS2 are running OSPF for their intra-AS routing protocol. Suppose AS1 and AS4 are running RIP for their intra-AS routing protocol. Suppose eBGP and iBGP are used for the inter-AS routing protocol. Initially suppose there is no physical link between AS2 and AS4.

![p6 image](/Images/p6.png)

**a. Router 3c learns about prefix x from which routing protocol: OSPF, RIP, eBGP, or iBGP?**


**b. Router 3a learns about prefix x from which routing protocol?**


**c. Router 1c learns about x from which routing protocol?**


**d. Router 1d learns about x from which routing protocol?**


### 7. In the figure below, X, Y and Z are access ISPs and A, B and C are backbone provider networks. Suppose an ISP only wants to route traffic to/from its customer networks (does not want to carry transit traffic between other ISPs). Another BGP policy X wants to enforce is that X does not want to route from B to C via X.

![p7 image](/Images/p7.png)

**Following is the topology view at Y. Draw the topology views of W and X.**

![p7 image Y](/Images/p7Y.png)


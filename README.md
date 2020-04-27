# SocketRPCServer
A simple Socket-RPC Server for some simple maths : 

- calculating of an integer array
- finding the minimum and the maximum value in an integer array
- creating a float array with the same length as the original one with each element multiplied by a float number a

# How it Works
There are two clients, two servers and two types of communication. 

First we have the Socket Communication between the Socket Client and the Socket Server/RPC Client. The Socket Client gets a MENU with the OPTIONS above and sends the needed data in a socket to the Socket Server. The Socket Server receives the data and as an RPC Client now it communicates with the RPC Server to get results depending on the MENU OPTION the main client has chosen. The RPC Server sends the results to the RPC Client, and the RPC Client now acting as a Socket Server sends the results to the Socket Client. 
Works for more than 1 Socket Client. 
![Comminucation](https://github.com/beecadox/SocketRPCServer/blob/master/communication.jpg?raw=true)
**Auto-generating RPC files using .x RPC file**
 ```
 rpcgen sd1.x 
 ```
 **Compile files**
  ```
 makefile
 ```
**Socket Client**
 ```
./socket_client *hostname* *port number*
 ```
**Socket Server/ RPC Client**
 ```
./sd1_client *hostname* *port number*
 ```
**RPC Socket**
 ```
./sd1_server 
 ```
 
 Tested on Ubuntu

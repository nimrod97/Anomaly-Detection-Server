/*
 * Server.h
 *
 *  Author: Nimrod Gabbay 
 */

#ifndef SERVER_H_
#define SERVER_H_
#include "CLI.h"
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>


using namespace std;

class ClientHandler {
public:
  virtual void handle(int clientID) = 0;
};

class AnomalyDetectionHandler : public ClientHandler {
  thread *t1;

public:
  virtual void handle(int clientID) {
    SocketIO sio(clientID);
    CLI cli(&sio);
    cli.start();
  }
};

class Server {
  thread *t; // the thread to run the start() method in
  int sockfd;
  struct sockaddr_in server;
  struct sockaddr_in client;

public:
  Server(int port) throw(const char *);
  virtual ~Server();
  void start(ClientHandler &ch) throw(const char *);
  void stop();
};

#endif /* SERVER_H_ */

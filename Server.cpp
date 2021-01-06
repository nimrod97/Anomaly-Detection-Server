/*
 * Server.cpp
 *
 *  Author: Nimrod Gabbay 
 */


#include "Server.h"

void alarmCase(int signum);
volatile static bool stopFlag = false;

Server::Server(int port) throw(const char *) {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    throw "socket failed";
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);
  if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    throw "binding failed";
  if (listen(sockfd, 5) < 0)
    throw "listen failed";
}

void Server::start(ClientHandler &ch) throw(const char *) {
  t = new thread([&ch, this]() {
    while (!stopFlag) {
      socklen_t clientSize = sizeof(client);
      signal(SIGALRM, alarmCase);
      alarm(5);
      int clientFd;
      clientFd = accept(sockfd, (struct sockaddr *)&client, &clientSize);
      if (clientFd < 0) {
        throw "accept error";
      }
      alarm(0);
      ch.handle(clientFd);
      continue;
    }
    close(sockfd);
  });
}
void alarmCase(int signum) { stopFlag = true; }

void Server::stop() {
  stopFlag = true;
  t->join(); 
}

Server::~Server() {}

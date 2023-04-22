#include "HttpServer.hxx"
#include "Request.hxx"

#include <iostream>
#include <signal.h>
#include <thread>
#include <unistd.h>

namespace server {

HttpServer::HttpServer(port_t port)
    : listenerSock_{Socket(port, kHttpSockType)}, isActive_{false} {}

HttpServer::~HttpServer() {
  if (isActive_) {
    isActive_ = false;
    listenerThread_.join();
  }
}

void HttpServer::stop() {
  isActive_ = false;
  listenerThread_.join();
}

void HttpServer::start() {
  isActive_ = true;
  listenerThread_ = std::thread([&] {
    try {
      listenerSock_.setNonBlocking();
      listenerSock_.startListen();
    } catch (std::exception &e) {
      std::cerr << "error starting listen on listener socket, error: "
                << e.what() << "; exiting!\n";
      return;
    }

    while (isActive_) {
      try {
        int fd = listenerSock_.acceptOne();
        if (fd < 0) {
          continue;
        }

        auto sock = Socket::from(fd);
        sock.setBlocking();

        std::string requestString = sock.readMessage();
        auto request = http::Request::from(requestString);
        std::cout << "received: " << requestString << "\n";
        auto response = router_.generateResponse(request).build();
        std::cout << "response: " << response << "\n";
        sock.writeAll(response);

        // std::string res;
        // res += "HTTP/1.1 200 OK\r\n";
        // res += "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n";
        // res += "Server: Apache/2.2.14 (Win32)\r\n";
        // res += "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n";
        // res += "Content-Length: 12\r\n";
        // res += "Content-Type: text/html\r\n";
        // res += "Connection: Closed\r\n\r\n";
        // res += "hello world\n";

        // sock.writeAll(res);
      } catch (const std::exception &e) {
        std::cerr << "error in listener: " << e.what() << "\n";
        continue;
      }
    }
  });
}

}; // namespace server
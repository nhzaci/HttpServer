#pragma once

#include "HttpRouter.hxx"
#include "Socket.hxx"
#include "Types.hxx"

#include <thread>

namespace server {

class HttpServer {
public:
  using listener_sock_t = Socket;
  using router_t = http::HttpRouter;

  static constexpr Socket::sock_t kHttpSockType = SOCK_STREAM;

  HttpServer() = delete;
  explicit HttpServer(port_t);
  ~HttpServer();

  HttpServer(const HttpServer &) = delete;
  HttpServer &operator=(const HttpServer &) = delete;

  HttpServer(HttpServer &&) = delete;
  HttpServer &operator=(HttpServer &&) = delete;

  void start();
  void stop();

private:
  listener_sock_t listenerSock_;
  std::thread listenerThread_;
  router_t router_;
  bool isActive_;

  // void startListener_();
};

}; // namespace server
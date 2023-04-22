#pragma once

#include "Types.hxx"

#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

namespace server {

class Socket {
public:
  using sock_t = __socket_type;
  using fd_t = int;
  using message_t = std::string;
  using flags_t = int;

  static constexpr int kMaxBufferSize = 4096;
  static constexpr size_t kMaxConnections = 20;

  Socket() = delete;
  explicit Socket(const port_t &, sock_t);
  ~Socket();
  Socket(Socket &&);
  Socket &operator=(Socket &&);

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  static Socket from(fd_t fd);

  void startListen();
  void connectTo();
  message_t readMessage();
  void writeAll(const message_t &);

  fd_t acceptOne();

  void setNonBlocking();
  void setBlocking();

  fd_t fd() { return fd_; }

private:
  fd_t fd_;
  addrinfo *servInfo_;

  explicit Socket(fd_t);

  /**
   * Populates servInfo_ attribute with info from getaddrinfo syscall
   */
  void populateServInfo_(const port_t &, sock_t);

  /**
   * Makes syscall of socket to get an fd
   */
  fd_t initSocketFd_();
};

}; // namespace server
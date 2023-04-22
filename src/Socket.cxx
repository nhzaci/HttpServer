#include "Socket.hxx"

#include "errno.h"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>

namespace server {

Socket Socket::from(fd_t fd) { return Socket(fd); }

Socket::Socket(fd_t fd) : fd_{fd}, servInfo_{nullptr} {}

Socket::Socket(const port_t &port, sock_t sock) {
  populateServInfo_(port, sock);
  fd_ = initSocketFd_();
}

Socket::Socket(Socket &&other) {
  fd_ = other.fd_;
  other.fd_ = -1;

  servInfo_ = other.servInfo_;
  other.servInfo_ = nullptr;
}

Socket &Socket::operator=(Socket &&other) {
  fd_ = other.fd_;
  other.fd_ = -1;

  servInfo_ = other.servInfo_;
  other.servInfo_ = nullptr;
  return *this;
}

Socket::~Socket() {
  if (servInfo_)
    freeaddrinfo(servInfo_);
  if (fd_ >= 0)
    close(fd_);
}

void Socket::connectTo() {
  int err = connect(fd_, servInfo_->ai_addr, servInfo_->ai_addrlen);
  if (err < 0) {
    std::string error = "bind syscall failed in Socket::listen(); error: ";
    error += std::strerror(errno);
    throw std::runtime_error(error);
  }
}

void Socket::startListen() {
  int err;
  if ((err = bind(fd_, servInfo_->ai_addr, servInfo_->ai_addrlen)) < 0) {
    std::string error = "bind syscall failed in Socket::listen(); error: ";
    error += std::strerror(errno);
    throw std::runtime_error(error);
  }

  if ((err = listen(fd_, kMaxConnections)) < 0) {
    std::string error = "listen syscall failed in Socket::listen(); error: ";
    error += std::strerror(errno);
    throw std::runtime_error(error);
  }
}

void Socket::setBlocking() {
  if (int flags = fcntl(fd_, F_GETFL, 0); flags != -1) {
    fcntl(fd_, F_SETFL, flags & ~O_NONBLOCK);
    return;
  }

  std::string error = "fcntl syscall failed in Socket::setBlock; error: ";
  error += std::strerror(errno);
  throw std::runtime_error(error);
}

void Socket::setNonBlocking() {
  if (int flags = fcntl(fd_, F_GETFL, 0); flags != -1) {
    fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
    return;
  }

  std::string error = "fcntl syscall failed in Socket::setNonBlock; error: ";
  error += std::strerror(errno);
  throw std::runtime_error(error);
}

Socket::fd_t Socket::acceptOne() {
  sockaddr_storage incomingAddr;
  socklen_t sinSize = sizeof(incomingAddr);
  fd_t incomingFd =
      accept4(fd_, (sockaddr *)&incomingAddr, &sinSize, SOCK_NONBLOCK);
  if (incomingFd < 0) {
    // ignore EAGAIN or EWOULDBLOCK errors for accept
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      return -1;

    std::string error = "listen syscall failed in Socket::listen(); error: ";
    error += std::strerror(errno);
    throw std::runtime_error(error);
  }

  return incomingFd;
}

void Socket::populateServInfo_(const port_t &port, sock_t sock) {
  addrinfo hints;

  // ensure that hints is really zeroed-out, else we might
  // set an option that we don't actually want
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET; // IPv4
  hints.ai_socktype = sock;
  hints.ai_flags = AI_PASSIVE; // automatically fill in IP

  int status;

  if ((status = getaddrinfo(NULL, port.c_str(), &hints, &servInfo_)) != 0) {
    std::string error = "getaddrinfo failed in Socket::_get_serv_info; error: ";
    error += gai_strerror(status);
    throw std::runtime_error(error);
  }
}

Socket::fd_t Socket::initSocketFd_() {
  fd_t sockFd = socket(servInfo_->ai_family, servInfo_->ai_socktype,
                       servInfo_->ai_protocol);
  if (sockFd < 0) {
    std::string error =
        "socket syscall failed in Socket::get_socket_fd_; error: ";
    error += std::strerror(errno);
    throw std::runtime_error(error);
  }
  return sockFd;
}

Socket::message_t Socket::readMessage() {
  char buffer[kMaxBufferSize];
  ssize_t recvBytes = recv(fd_, &buffer, kMaxBufferSize, 0);

  if (recvBytes < 0) {
    // if try again and no data received yet, try again!
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      return "";

    std::string error = "recv syscall failed in Socket::readMessage; error: ";
    error += std::strerror(errno);
    throw std::runtime_error(error);
  }

  // TODO: Investigate cost of const char * to std::string conversion
  return message_t(buffer);
}

void Socket::writeAll(const message_t &data) {
  auto buffer = data.c_str();

  ssize_t offset = 0;
  size_t bytesLeft = data.size();
  ssize_t n;

  while (bytesLeft > 0) {
    n = send(fd_, buffer + offset, bytesLeft, 0);
    // std::cout << "n: " << n << std::endl;
    if (n < 0) {
      std::string error = "send syscall failed in Socket::writeAll; error: ";
      error += strerror(errno);
      throw std::runtime_error(error);
    }

    offset += n;
    bytesLeft -= n;
  }
}

}; // namespace server
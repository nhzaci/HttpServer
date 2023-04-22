#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace http {

class Request {
public:
  enum Method { Get, Put, Post, Delete, Head, Options, Trace, Connect };

  using method_t = Method;
  using version_t = std::string;
  using route_t = std::string;
  using header_t = std::unordered_map<std::string, std::string>;
  using body_t = std::string;

  method_t method() const { return method_; }
  route_t route() const { return route_; }
  version_t version() const { return version_; }
  header_t headers() const { return headers_; }
  body_t body() const { return body_; }

  static Request from(const std::string &);
  static Method methodFrom(const std::string &);

  friend std::ostream &operator<<(std::ostream &o, const Request &req);

private:
  template <typename method, typename route, typename version, typename header>
  Request(method &&m, route &&r, version &&v, header &&h)
      : method_{std::forward<method>(m)}, route_{std::forward<route>(r)},
        version_{std::forward<version>(v)}, headers_{std::forward<header>(h)} {}

  template <typename method, typename route, typename version, typename header,
            typename body>
  Request(method m, route r, version v, header h, body b)
      : method_{std::forward<method>(m)}, route_{std::forward<route>(r)},
        version_{std::forward<version>(v)}, headers_{std::forward<header>(h)},
        body_{std::forward<body>(b)} {}

  method_t method_;
  route_t route_;
  version_t version_;
  header_t headers_;
  body_t body_;
};

inline std::ostream &operator<<(std::ostream &os,
                                const Request::Method &method) {
  switch (method) {
  case Request::Method::Get:
    return os << "GET";
  case Request::Method::Put:
    return os << "PUT";
  case Request::Method::Post:
    return os << "POST";
  case Request::Method::Delete:
    return os << "DELETE";
  case Request::Method::Head:
    return os << "HEAD";
  case Request::Method::Options:
    return os << "OPTIONS";
  case Request::Method::Trace:
    return os << "TRACE";
  case Request::Method::Connect:
    return os << "CONNECT";
  default:
    return os << "UNKNOWN";
  }
}

inline std::ostream &operator<<(std::ostream &o, const Request &req) {
  o << "Request(method: " << req.method_ << "; route: " << req.route_
    << "; version: " << req.version_ << "; headers: \n";

  for (const auto &[key, val] : req.headers_) {
    o << "header: " << key << "; val: " << val << "\n";
  }

  return o << ")\n";
}

}; // namespace http

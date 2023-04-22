#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

namespace http {

namespace response {

enum class Status : uint16_t {
  Ok = 200,
  NotFound = 404,
};

enum class ContentType : uint8_t {
  TextHtml,
  ApplicationJson,
};

// From RFC 2616, HTTP/1.1
class Builder {
public:
  using version_t = std::string;
  using status_t = Status;
  using body_t = std::string;
  using header_key_t = std::string;
  using header_value_t = std::string;
  using header_t = std::unordered_map<header_key_t, header_value_t>;

  Builder();

  void setStatus(status_t);
  void setBody(body_t);
  void appendBody(body_t);
  void addHeader(header_key_t, header_value_t);
  std::string build();

private:
  version_t version_;
  status_t status_;
  body_t body_;
  header_t headers_;
};

inline std::ostream &operator<<(std::ostream &os, const Status &status) {
  switch (status) {
  case Status::Ok:
    os << "OK";
    break;
  case Status::NotFound:
    os << "Not Found";
    break;
  default:
    os << "Unknown";
    break;
  }

  return os;
}

} // namespace response
}; // namespace http
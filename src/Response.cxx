#include "Response.hxx"

#include <sstream>

namespace http {

namespace response {

Builder::Builder() : version_{"HTTP/1.1"}, status_{}, body_{}, headers_{} {}

void Builder::setStatus(status_t status) { status_ = status; }

void Builder::setBody(body_t body) { body_ = body; }

void Builder::appendBody(body_t body) { body_ += body; }

void Builder::addHeader(header_key_t key, header_value_t value) {
  headers_.emplace(key, value);
}

std::string Builder::build() {
  std::stringstream ss;

  addHeader("Content-Length", std::to_string(body_.size()));

  // request line
  ss << version_ << " " << static_cast<uint16_t>(status_) << " " << status_
     << "\r\n";
  // headers
  for (const auto &[key, value] : headers_) {
    ss << key << ": " << value << "\r\n";
  }
  // carriage return before body
  ss << "\r\n";
  // body
  ss << body_;

  return ss.str();
}

}; // namespace response

}; // namespace http
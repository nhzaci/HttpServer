#include "Request.hxx"
#include "Utils.hxx"
#include <sstream>
#include <stdexcept>

namespace http {

Request::Method Request::methodFrom(const std::string &input) {
  if (input == "GET")
    return Method::Get;
  if (input == "PUT")
    return Method::Put;
  if (input == "POST")
    return Method::Post;
  if (input == "DELETE")
    return Method::Delete;
  if (input == "HEAD")
    return Method::Head;

  throw std::invalid_argument("Unable to recognise input: " + input +
                              " as a valid HTTP Method");
}

Request Request::from(const std::string &input) {
  if (input.size() == 0) {
    throw std::invalid_argument("Input for HTTP request must not be empty");
  }

  auto inputSplit = splitStringByDelimiter(input, "\r\n");
  // we should at least have request line, else it's inavlid HTTP
  if (inputSplit.size() < 1) {
    auto error = "Request line not found in input: ";
    error + input;
    throw std::invalid_argument(error);
  }

  auto requestLine = inputSplit[0];
  auto requestSplit = splitStringByDelimiter(requestLine, " ");

  if (requestSplit.size() != 3) {
    std::stringstream error;
    error << "Invalid request line: `" << requestLine
          << "` must contain method, route and version, request split size: "
          << requestSplit.size() << "; ";

    for (const auto &item : requestSplit)
      error << item << "; ";

    throw std::invalid_argument(error.str());
  }

  auto method = methodFrom(requestSplit[0]);
  auto route = requestSplit[1];
  auto version = requestSplit[2];
  header_t headers;

  size_t i;
  for (i = 1; i < inputSplit.size(); i++) {
    // second carriage return line has empty stuff
    if (inputSplit[i].size() == 0)
      break;

    // split string by space, assume only space is between "header: value"
    // TODO: is this a safe assumption? check the rfc
    auto lineSplit = splitStringByDelimiter(inputSplit[i], " ");
    if (lineSplit.size() != 2) {
      throw std::invalid_argument(
          "Invalid line: `" + lineSplit[i] +
          "` must contain only 2 parts, a header and a value");
    }
    auto header = lineSplit[0].substr(0, lineSplit[0].size() - 1);
    auto value = lineSplit[1];
    headers[header] = value;
  }

  // TODO: potential bug if user somehow has carriage return lines inside the
  // request body; perhaps we should simply track number of characters skipped
  // and substr the part after everything into the reqeust body
  if (++i < inputSplit.size()) {
    return Request(method, route, version, headers, inputSplit[i]);
  }

  return Request(method, route, version, headers);
}

}; // namespace http
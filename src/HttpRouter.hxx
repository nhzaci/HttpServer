#pragma once

#include "Request.hxx"
#include "Response.hxx"

#include <string>
#include <unordered_map>

namespace http {

class HttpRouter {
public:
  using route_t = std::string;
  using data_t = std::string;
  using method_t = http::Request::Method;
  using request_t = http::Request;
  using response_t = http::response::Builder;

  void addRoute(route_t, method_t, data_t);
  void removeRoute(route_t);
  data_t getRoute(route_t, method_t);

  response_t generateResponse(request_t);

private:
  using route_map_t =
      std::unordered_map<route_t, std::unordered_map<method_t, data_t>>;
  route_map_t routesMap_;
};

}; // namespace http
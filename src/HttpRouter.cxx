#include "HttpRouter.hxx"

#include <stdexcept>

namespace http {

// TODO: Perfect forwarding
void HttpRouter::addRoute(route_t route, method_t method, data_t data) {
  if (auto search = routesMap_.find(route); search != routesMap_.end()) {
    search->second.emplace(method, data);
  } else {
    auto [mapItr, _] =
        routesMap_.emplace(route, std::unordered_map<method_t, data_t>());
    mapItr->second.emplace(method, data);
  }
}

void HttpRouter::removeRoute(route_t route) { routesMap_.erase(route); }

HttpRouter::data_t HttpRouter::getRoute(route_t route, method_t method) {
  return routesMap_.at(route).at(method);
}

HttpRouter::response_t HttpRouter::generateResponse(request_t req) {
  response_t responseBuilder;
  try {
    auto data = getRoute(req.route(), req.method());
    responseBuilder.setStatus(http::response::Status::Ok);
    responseBuilder.setBody(data);
  } catch (const std::out_of_range &e) {
    // return 404
    responseBuilder.setStatus(http::response::Status::NotFound);
    responseBuilder.setBody("We could not find what you are looking for...\n");
  }

  return responseBuilder;
}

}; // namespace http
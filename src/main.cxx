#include "HttpServer.hxx"

#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
  std::string port = argc > 1 ? argv[1] : "8080";

  auto server = server::HttpServer(port);
  try {
    std::cout << "starting http server...\n";
    server.start();
    std::cout << "started http server, listening on port 8080.\n";
    std::string command;
    while (std::cin >> command, command != "quit")
      ;
    std::cout << "stopping http server...\n";
    server.stop();
    std::cout << "successfully stopped.\n";
  } catch (const std::exception &e) {
    std::cerr << "error reached; error: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "some other error.." << std::endl;
  }
}
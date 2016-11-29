#include <iostream>

#include "app/App.hpp"
#include "minebombers/Minebombers.hpp"


int main(int argc, char* argv[]) {
  if(argc > 1 && std::string(argv[1]) == "testrun") {
    // Program is run with 'testrun' parameter.
    // This is used in automated testing to make sure that the program actually runs.
    std::cout << "Running windowless test run" << std::endl;
  } else {
    auto minebombers = std::make_shared<Minebombers>();

    App app(minebombers);
    app.init();
    app.run();
  }

  std::cout << "Exiting..." << std::endl;

  return 0;
}

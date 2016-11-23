#include <iostream>
#include <chrono>

// NOTE: Enables swizzleing of vectors, but may cause name conflicts
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include <SFML/Window.hpp>
#include <sol.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "service/Services.hpp"
#include "app/App.hpp"

int main(int argc, char* argv[]) {
  if(argc > 1 && std::string(argv[1]) == "testrun") {
    // Program is run with 'testrun' parameter.
    // This is used in automated testing to make sure that the program actually runs.
    std::cout << "Running windowless test run" << std::endl;
  } else {
    Game game;

    App app(game);
    app.init();
    app.run();
  }

  Services::logger()->info("Exiting");

  return 0;
}

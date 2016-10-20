#include <iostream>

#include <SFML/Window.hpp>
#include <sol.hpp>
#include <glm/vec3.hpp>

#include "kabanero/collection/mutable/KBVector.hpp"
#include "kabanero/collection/mutable/KBMap.hpp"
#include "kabanero/Option.hpp"
#include "util/PrintUtil.hpp"

int main(int argc, char* argv[]) {
  // Load configuration from config.lua file
  sol::state config;
  config.script_file("resources/config.lua");

  if(argc > 1 && std::string(argv[1]) == "testrun") {
    // Program is run with 'testrun' parameter.
    // This is used in automated testing to make sure that the program actually runs.
    std::cout << "Running windowless test run" << std::endl;

    auto test_string = config["test_string"].get_or<std::string>("notfound");
    std::cout << "test_string: " << test_string << std::endl;

  } else {
    std::cout << "Starting window" << std::endl;

    // Get window parameters from config file and create window
    auto window_w = config["window_width"].get_or(800);
    auto window_h = config["window_height"].get_or(800);
    auto window_name = config["window_name"].get_or<std::string>("window");

    sf::Window window(sf::VideoMode(window_w, window_h), window_name);

    while (window.isOpen()){
      // Main event loop
      sf::Event event;
      while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed)
          window.close();
      }
    }
  }
  std::cout << "Exiting" << std::endl;
  return 0;
}

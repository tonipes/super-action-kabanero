#include <iostream>

// NOTE: Enables swizzleing of vectors, but may cause name conflicts
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include <SFML/Window.hpp>
#include <sol.hpp>
#include <glm/vec3.hpp>

#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "collection/Option.hpp"
#include "util/PrintUtil.hpp"
#include "logger/Logger.hpp"

int main(int argc, char* argv[]) {
  // Load configuration from config.lua file
  Logger logger;

  logger.debug("Debug test");
  logger.info("Info test");
  logger.warn("Warn test");
  logger.error("Error test");
  logger.fatal("Fatal test");

  sol::state config;
  config.script_file("resources/config.lua");

  if(argc > 1 && std::string(argv[1]) == "testrun") {
    // Program is run with 'testrun' parameter.
    // This is used in automated testing to make sure that the program actually runs.
    logger.info("Running windowless test run" );

    auto test_string = config["test_string"].get_or<std::string>("notfound");
    logger.info( "test_string: " + test_string );

  } else {
    logger.info("Starting window");

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

  logger.info("Exiting");

  return 0;
}

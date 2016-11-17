#include <iostream>
#include <chrono>

// NOTE: Enables swizzleing of vectors, but may cause name conflicts
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include <SFML/Window.hpp>
#include <sol.hpp>
#include <glm/vec3.hpp>

#include "audio/AudioPlayer.hpp"
#include "resource/resourceManager/SyncResourceManager.hpp"
#include "message/messagePublisher/DefaultMessagePublisher.hpp"
#include "message/event/AudioEvent.hpp"
#include "resource/loader/TextLoader.hpp"
#include "resource/resource/Text.hpp"
#include "game/Game.hpp"
#include "resource/loader/TextureLoader.hpp"
#include "resource/loader/AudioLoader.hpp"
#include "collection/mutable/KBVector.hpp"
#include "collection/mutable/KBMap.hpp"
#include "collection/Option.hpp"
#include "util/PrintUtil.hpp"
#include "logger/DefaultLogger.hpp"
#include "service/Services.hpp"

int main(int argc, char* argv[]) {

  auto logger = std::make_shared<DefaultLogger>();
  auto resourceManager = std::make_shared<SyncResourceManager>();
  auto messagePublisher = std::make_shared<DefaultMessagePublisher>();

  Services::provideMessagePublisher(messagePublisher);
  Services::provideResourceManager(resourceManager);


  logger->debug("Debug test");
  logger->info("Info test");
  logger->warn("Warn test");
  logger->error("Error test");
  logger->fatal("Fatal test");

  // Load configuration from config.lua file
  sol::state config;
  config.script_file("resources/config.lua");

  if(argc > 1 && std::string(argv[1]) == "testrun") {
    // Program is run with 'testrun' parameter.
    // This is used in automated testing to make sure that the program actually runs.
    logger->info("Running windowless test run" );

  } else {
    logger->info("Loading resources");

    auto resources = config.get<sol::table>("resources");

    auto text_loader = std::make_shared<TextLoader>();
    std::regex text_regex("^.+\\.txt$");
    resourceManager->addLoader(text_regex, text_loader);

    auto audio_loader = std::make_shared<AudioLoader>();
    std::regex audio_regex("^.+\\.ogg$");
    resourceManager->addLoader(audio_regex, audio_loader);

    auto texture_loader = std::make_shared<TextureLoader>();
    std::regex texture_regex("^.+\\.png$");
    resourceManager->addLoader(texture_regex, texture_loader);

    for(auto i = 1; i <= resources.size(); i++){
      logger->debug(resources.get<std::string>(i));
      resourceManager->load(resources.get<std::string>(i));
    }

    // Intervals
    auto update_interval = config["update_interval"].get_or(32);
    auto draw_interval = config["draw_interval"].get_or(32);

    // Get window parameters from config file and create window
    auto window_w = config["window_width"].get_or(800);
    auto window_h = config["window_height"].get_or(800);
    auto window_name = config["window_name"].get_or<std::string>("window");

    sf::RenderWindow window(sf::VideoMode(window_w, window_h), window_name);

    logger->info("Creating game");

    Renderer renderer(window);

    auto audioFolderPath = config["audio_folder"].get_or<std::string>("resources/audio/");

    auto audioPlayer = std::make_shared<AudioPlayer>(audioFolderPath);
    Game game(renderer);

    messagePublisher->addSubscriber(audioPlayer);

    game.init();

    messagePublisher->sendMessage(
      Message(
        "audioPlayer:local_forecast.ogg",
        std::make_shared<AudioEvent>(PLAY)
      )
    );

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds ms;

    auto last_update_time = Clock::now();
    auto last_draw_time = Clock::now();

    while (window.isOpen()){
      auto current_time = Clock::now();

      auto update_delta_ms = std::chrono::duration_cast<ms>(current_time - last_update_time);
      double update_delta = update_delta_ms.count() / 1000.0;

      auto draw_delta_ms = std::chrono::duration_cast<ms>(current_time - last_draw_time);
      double draw_delta = draw_delta_ms.count() / 1000.0;

      if(update_delta_ms.count() > update_interval) {
        messagePublisher->publishMessages();
        game.update(update_delta);
        last_update_time = current_time;
      }
      if(draw_delta_ms.count() > draw_interval) {
        game.render();
        last_draw_time = current_time;
      }

      sf::Event event;
      while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed) {
          window.close();
        } else if (event.type == sf::Event::Resized) {

        }
      }
    }

  }

  logger->info("Exiting");

  return 0;
}

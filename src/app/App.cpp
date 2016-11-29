// NOTE: Enables swizzleing of vectors, but may cause name conflicts
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

#include <Box2D/Box2D.h>
#include <sol.hpp>

#include "app/App.hpp"
#include "service/Services.hpp"
#include "logger/DefaultLogger.hpp"
#include "resource/resourceManager/SyncResourceManager.hpp"
#include "message/messagePublisher/DefaultMessagePublisher.hpp"
#include "resource/loader/TextLoader.hpp"
#include "resource/loader/AudioLoader.hpp"
#include "resource/loader/TextureLoader.hpp"
#include "resource/loader/AudioLoader.hpp"
#include "resource/loader/AtlasLoader.hpp"
#include "audio/AudioPlayer.hpp"
#include "input/InputTranslator.hpp"

App::App(std::shared_ptr<Game> game) : _game(game) {
  auto logger = std::make_shared<DefaultLogger>();
  auto resourceManager = std::make_shared<SyncResourceManager>();
  auto messagePublisher = std::make_shared<DefaultMessagePublisher>();

  Services::provideMessagePublisher(messagePublisher);
  Services::provideResourceManager(resourceManager);
  Services::provideLogger(logger);

  logger->debug("Debug test");
  logger->info("Info test");
  logger->warn("Warn test");
  logger->error("Error test");
  logger->fatal("Fatal test");

  sol::state config;
  config.script_file("resources/config.lua");

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

  auto atlas_loader = std::make_shared<AtlasLoader>();
  std::regex atlas_regex("^.+\\.atlas$");
  resourceManager->addLoader(atlas_regex, atlas_loader);

  for(auto i = 1; i <= resources.size(); i++){
    logger->debug(resources.get<std::string>(i));
    resourceManager->load(resources.get<std::string>(i));
  }

  // Intervals
  _update_interval = config["update_interval"].get_or(32);
  _draw_interval = config["draw_interval"].get_or(32);

  // Get window parameters from config file and create window
  _window_w = config["window_width"].get_or(800);
  _window_h = config["window_height"].get_or(800);
  _window_name = config["window_name"].get_or<std::string>("window");


  _audioFolderPath = config["audio_folder"].get_or<std::string>("resources/audio/");
  _tilesize = config["tilesize"].get_or(32);
}

auto App::init() -> void {
  Services::logger()->info("Creating game");
  _game->init();
}

auto App::run() -> void {
  sf::RenderWindow window(sf::VideoMode(_window_w, _window_h), _window_name);
  auto audioPlayer = std::make_shared<AudioPlayer>(_audioFolderPath);
  Services::messagePublisher()->addSubscriber(audioPlayer);
  Renderer renderer(window, _tilesize);
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::milliseconds ms;

  auto last_update_time = Clock::now();
  auto last_draw_time = Clock::now();

  std::shared_ptr<InputTranslator> inputTranslator = std::make_shared<InputTranslator>();

  // Shameless box2d test
  b2Vec2 gravity(0.0f, -10.0f);
  b2World world(gravity);
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(0.0f, -10.0f);
  auto groundBody = world.CreateBody(&groundBodyDef);
  b2PolygonShape groundBox;
  groundBox.SetAsBox(50.0f, 10.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 4.0f);
  b2Body* body = world.CreateBody(&bodyDef);
  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  body->CreateFixture(&fixtureDef);

  while (window.isOpen()) {
    
    // b2d test
    world.Step(1.0f / 60.0f, 6, 2);
    auto position = body->GetPosition();
    auto angle = body->GetAngle();
    std::stringstream s;
    s << "pos: " << position.x << "," <<  position.y << ". ang: " << angle;
    std::string str = s.str();
    Services::logger()->debug(s.str());
    // b2d test end

    auto current_time = Clock::now();

    auto update_delta_ms = std::chrono::duration_cast<ms>(current_time - last_update_time);
    double update_delta = update_delta_ms.count() / 1000.0;

    auto draw_delta_ms = std::chrono::duration_cast<ms>(current_time - last_draw_time);
    double draw_delta = draw_delta_ms.count() / 1000.0;

    if (update_delta_ms.count() > _update_interval) {
      Services::messagePublisher()->publishMessages();
      _game->update(update_delta);
      last_update_time = current_time;
    }
    if (draw_delta_ms.count() > _draw_interval) {
      _game->render(renderer);
      last_draw_time = current_time;
    }

    sf::Event event;
    while (window.pollEvent(event)) {
      inputTranslator->processMessage(event);
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::Resized) {
        window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
      }
    }
  }
}

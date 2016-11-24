#include "resource/loader/AtlasLoader.hpp"

auto AtlasLoader::load(const std::string& filePath) -> const std::shared_ptr<Resource> {
  auto path = "this/is/a/test/path";

  std::ifstream in { filePath };
  if (!in.is_open()) {
    throw ResourceException("Couldn't load file: " + filePath);
  }
  std::string text = {
    std::istreambuf_iterator<char>(in),
    std::istreambuf_iterator<char>()
  };
  text.pop_back(); // Last line end

  std::stringstream ss(text);
  std::string line;
  std::string texID;
  std::string spriteID;
  std::smatch match;
  int w;
  int h;
  int x;
  int y;
  int sw;
  int sh;
  int ox;
  int oy;
  int offsetX;
  int offsetY;
  auto startSprite = false;
  KBMap<std::string, Sprite> sprites;
  while (std::getline(ss, line, '\n')) {
    if (std::regex_match(line, colonRegex)) {
      if (std::regex_search(line, match, sizeRegex) && match.size() == 2) {
        if (startSprite) {
          sw = std::stoi(match.str(1));
          sh = std::stoi(match.str(2));
        } else {
          w = std::stoi(match.str(1));
          h = std::stoi(match.str(2));
        }
      } else if (std::regex_search(line, match, xyRegex) && match.size() == 2) {
        x = std::stoi(match.str(1));
        y = std::stoi(match.str(2));
      } else if (std::regex_search(line, match, origRegex) && match.size() == 2) {
        ox = std::stoi(match.str(1));
        oy = std::stoi(match.str(2));
      } else if (std::regex_search(line, match, origRegex) && match.size() == 2) {
        offsetX = std::stoi(match.str(1));
        offsetY = std::stoi(match.str(2));
      } else if (std::regex_match(line, indexRegex)) {
        startSprite = false;
        Sprite sprite(
          path + texID,
          glm::vec2(x / (float)w, y / (float)h),
          glm::vec2(sw / (float)w, sh / (float)h),
          glm::vec2(ox / (float)w, oy / (float)h),
          glm::vec2(offsetX / (float)w, offsetY / (float)h)
        );
        std::cout << spriteID << std::endl;
        sprites.insert(spriteID, sprite);
      }
    } else {
      if (std::regex_match(line, imageRegex)) {
        texID = line;
      } else {
        spriteID = line;
        startSprite = true;
      }
    }
  }
  return std::make_shared<Atlas>(sprites);
}

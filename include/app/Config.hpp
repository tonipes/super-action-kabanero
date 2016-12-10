#pragma once

#include <iostream>
#include <fstream>

namespace cfg {

  auto _loadFile(std::string path) -> std::vector<std::string> {
    std::vector<std::string> lines;
    std::ifstream file(path);

    std::string line;
    while ( std::getline(file, line) ) {
        if ( !line.empty() )
            lines.push_back(line);
    }

    return lines;
  }

  auto _getDefaultConfig() ->  std::map<std::string, std::string> {
    return std::map<std::string, std::string> {
      {"window_width", "800"},
      {"window_height", "600"},
      {"window_name", "Kabanero"},
      {"audio_path", "resources/audio/"},
      {"update_fps", "60"},
      {"draw_fps", "120"},
      {"tilesize", "32"},
    };

  }

  auto loadConfig(std::string path) -> std::map<std::string, std::string> {
    auto config = cfg::_getDefaultConfig();

    auto lines = cfg::_loadFile(path);

    for(auto line : lines) {
      std::string::size_type pos;
      pos=line.find('=',0);
      auto first = line.substr(0,pos);
      auto second = line.substr(pos+1);
      config[first] = second;
    }

    return config;

  }

  auto loadResourceList(std::string path) -> std::vector<std::string> {
    auto res = _loadFile(path);
    return res;
  }
}

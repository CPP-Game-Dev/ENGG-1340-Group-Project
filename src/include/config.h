#include <iostream>

#pragma once

class Config {
  private:
    const std::string GAME_FOLDER = "game_name";
    const std::string CONFIG_FILE = "config.txt";

    std::string configDir;
    std::string homePath;

    Config();

  public:
    std::string getConfigDir();
    void addPair(std::string key, std::string value);
    std::string readPair(std::string key);
};

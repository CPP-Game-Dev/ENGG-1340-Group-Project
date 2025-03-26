#include <iostream>

const std::string GAME_FOLDER = "game_name";
const std::string CONFIG_FILE = "config.txt";

class Config {
  private:
    std::string configDir;

  public:
    std::string getConfigDir();
    void addPair(std::string key, std::string value);
    std::string readPair(std::string key);
    Config();
};

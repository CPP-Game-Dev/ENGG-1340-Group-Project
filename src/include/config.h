#include <iostream>
#include <map>
#include <string>

const std::string GAME_FOLDER = "game_name";
const std::string CONFIG_FILE = "config.txt";

class Config {
  private:
    std::string configDir;
    std::string getConfigDir();
    void createDefaultConfig();
    void addPair(std::string key, std::string value);
    std::string getPair(std::string key);
    std::map<int, std::string> getConfigKeys = {
        {KB_UP, "KB_UP"},
        {KB_DOWN, "KB_DOWN"},
        {KB_LEFT, "KB_LEFT"},
        {KB_RIGHT, "KB_RIGHT"},
        {KB_CONFIRM, "KB_CONFIRM"},
        {KB_CANCEL, "KB_CANCEL"},
        {KB_USE_PICKAXE, "KB_USE_PICKAXE"},
        {KB_USE_RATION, "KB_USE_RATION"},
        {KB_EXIT, "KB_EXIT"},
    };

  public:
    Config();
    enum CONFIG_KEYS {
        KB_UP = 0,
        KB_DOWN,
        KB_LEFT,
        KB_RIGHT,
        KB_CONFIRM,
        KB_CANCEL,
        KB_USE_PICKAXE,
        KB_USE_RATION,
        KB_EXIT,
    };
    std::string getConfig(CONFIG_KEYS);

    // Get the absolute path to the config file
    std::string getConfigFilePath() const { return configDir + CONFIG_FILE; }
};

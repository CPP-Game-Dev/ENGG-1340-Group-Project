#include "include/config.h"
#include <fstream>

/*
 * Config class constructor
 *
 * Assigns the value of the configuration directory based on environment
 * variables
 */
Config::Config() {
    // Config files adhere to XDG Base Directory Specification
    // See: https://specifications.freedesktop.org/basedir-spec/latest/
    homePath = getenv("XDG_DATA_HOME");
    if (homePath.empty()) // if XDG_DATA_HOME is not set, fallback to $HOME
        homePath = getenv("HOME");

    // Constructing base directory
    configDir = homePath + "/.config/" + GAME_FOLDER + "/";
}

/*
 * Function to get the configuration directory
 *
 * Returns a std::string of the configuration directory
 */
std::string Config::getConfigDir() { return configDir; }

/*
 * Function to add a key-value pair to the configuration file
 *
 * Writes a key value pair in the form of KEY=VALUE in the configuration file
 */
void Config::addPair(std::string key, std::string value) {
    std::ofstream file(configDir + "config.txt", std::ios::app);
    file << key << "=" << value << std::endl;
    file.close();
}

/*
 * Function to read a config pair from a given key
 *
 * Returns a string which contains the value of the key value pair
 *
 * Args: key
 */
std::string Config::readPair(std::string key) {
    std::ifstream file(configDir + CONFIG_FILE);

    for (std::string line; std::getline(file, line);) {
        if (line.find(key) != std::string::npos) {
            return line.substr(line.find("=") + 1);
            break;
        }
    }

    return std::string();
}

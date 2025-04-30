#include "include/config.h"
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>

/*
 * Config class constructor
 *
 * Assigns the value of the configuration directory based on environment
 * variables
 */
Config::Config() {
    // Config files adhere to XDG Base Directory Specification
    // See: https://specifications.freedesktop.org/basedir-spec/latest/
    char *cConfigDir = getenv("XDG_CONFIG_HOME");

    if (cConfigDir != NULL) {
        configDir = cConfigDir;
    } else {
        char *cHomePath = getenv("HOME");
        std::string homePath = cHomePath;
        configDir = homePath + "/.config/" + GAME_FOLDER + "/";
    }

    // Check if the directory exists
    struct stat info;
    if (stat(configDir.c_str(), &info) != 0) {
        // Directory does not exist, create it
        if (mkdir(configDir.c_str(), 0777) == -1) {
            std::cerr << "Error creating directory: " << configDir << std::endl;
        }
    } else if (!(info.st_mode & S_IFDIR)) {
        // Path exists but is not a directory
        std::cerr << configDir << " is not a directory" << std::endl;
    }

    // Check if the config file exists
    std::ifstream file(configDir + CONFIG_FILE);
    if (!file.good()) {
        // File does not exist, create it
        std::ofstream newFile(configDir + CONFIG_FILE); // Create the file
        newFile.close();                                // Close the file
        createDefaultConfig();                          // Create default config
    } else {
        // File exists, do nothing
        file.close();
    }
}

/*
 * Function to get the configuration directory
 *
 * @return std::string
 */
std::string Config::getConfigDir() { return configDir; }

/*
 * Function to add a key-value pair to the configuration file
 *
 * Writes a key value pair in the form of KEY=VALUE in the configuration file
 *
 * @param key
 * @param value
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
 * @param key
 */
std::string Config::getPair(std::string key) {
    std::ifstream file(configDir + CONFIG_FILE);

    for (std::string line; std::getline(file, line);) {
        if (line.find(key) != std::string::npos) {
            return line.substr(line.find("=") + 1);
            break;
        }
    }

    return std::string();
}

/*
 * Function to create the default configuration
 *
 * Initializes default key bindings for core actions (movement, tools, quit).
 * This is used when there is no existing config file and we need to set up
 * the game with a standard control scheme.
 *
 * Usage:
 * Config config;
 * config.createDefaultConfig(); // Sets keys like w/a/s/d, e, r, q
 *
 * Default bindings:
 * - Move Up       -> "w"
 * - Move Down     -> "s"
 * - Move Left     -> "a"
 * - Move Right    -> "d"
 * - Confirm       -> " "
 * - Cancel        -> "x"
 * - Use Pickaxe   -> "e"
 * - Use Ration    -> "r"
 * - Exit Game     -> "q"
 * 
 * @return void
 */
void Config::createDefaultConfig() {
    addPair(getConfigKeys.at(CONFIG_KEYS::KB_DOWN), "s");
    addPair(getConfigKeys.at(CONFIG_KEYS::KB_UP), "w");
    addPair(getConfigKeys.at(CONFIG_KEYS::KB_LEFT), "a");
    addPair(getConfigKeys.at(CONFIG_KEYS::KB_RIGHT), "d");
    addPair(getConfigKeys.at(CONFIG_KEYS::KB_CONFIRM), " ");
    addPair(getConfigKeys.at(CONFIG_KEYS::KB_CANCEL), "x");
    addPair(getConfigKeys.at(CONFIG_KEYS::KB_USE_PICKAXE), "e");
    addPair(getConfigKeys.at(CONFIG_KEYS::KB_USE_RATION), "r");
    addPair(getConfigKeys.at(CONFIG_KEYS::KB_EXIT), "q");
}

/*
 * Function to get the configuration
 *
 * Returns a string which contains the value of the key value pair
 *
 * Usage:
 * Config config;
 * std::string value = config.getConfig(Config::KB_UP); // value = "w"
 *
 * @param key
 * @return std::string
 */
std::string Config::getConfig(CONFIG_KEYS key) {
    return getPair(getConfigKeys.at(key));
}

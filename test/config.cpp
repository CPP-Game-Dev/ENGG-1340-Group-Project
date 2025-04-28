#include "../src/include/config.h"
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

// Helper function to check if a file exists
bool fileExists(const std::string &path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

// Helper function to set and restore environment variables
class EnvVarGuard {
  private:
    std::string name;
    char *oldValue;
    bool wasSet;

  public:
    EnvVarGuard(const std::string &name, const std::string &value)
        : name(name) {
        char *oldVal = getenv(name.c_str());
        wasSet = (oldVal != nullptr);
        oldValue = wasSet ? strdup(oldVal) : nullptr;
        setenv(name.c_str(), value.c_str(), 1);
    }

    ~EnvVarGuard() {
        if (wasSet) {
            setenv(name.c_str(), oldValue, 1);
            free(oldValue);
        } else {
            unsetenv(name.c_str());
        }
    }
};

namespace config_tests {

void test_constructor_with_xdg_config() {
    // Create a temporary directory for the test
    std::string testDir = "/tmp/config_test_xdg";
    mkdir(testDir.c_str(), 0777);

    // Set XDG_CONFIG_HOME to the temp directory
    EnvVarGuard guard("XDG_CONFIG_HOME", testDir);

    // Create Config object
    Config config;

    // Check if the config directory is set correctly
    assert(config.getConfigDir() == testDir);

    std::cout << "Constructor with XDG_CONFIG_HOME test passed!" << std::endl;
}

void test_constructor_with_home() {
    // Unset XDG_CONFIG_HOME and set HOME
    unsetenv("XDG_CONFIG_HOME");
    EnvVarGuard homeGuard("HOME", "/tmp");

    // Create the .config directory if it doesn't exist
    std::string configDir = "/tmp/.config";
    mkdir(configDir.c_str(), 0777);

    // Create Config object
    Config config;

    // Check if the config directory is set correctly
    std::string expectedDir = "/tmp/.config/" + GAME_FOLDER + "/";
    assert(config.getConfigDir() == expectedDir);

    // Check if directory was created
    struct stat info;
    assert(stat(expectedDir.c_str(), &info) == 0);
    assert(info.st_mode & S_IFDIR);

    std::cout << "Constructor with HOME test passed!" << std::endl;
}

void test_add_and_get_pair() {
    // Set up a test directory
    std::string testDir = "/tmp/config_test_pairs/";
    mkdir(testDir.c_str(), 0777);
    EnvVarGuard guard("XDG_CONFIG_HOME", testDir);

    Config config;

    // Test adding a key-value pair
    config.addPair("test_key", "test_value");

    // Check if config file was created
    std::string configFile = testDir + CONFIG_FILE;
    assert(fileExists(configFile));

    // Test getting the value back
    assert(config.getPair("test_key") == "test_value");

    std::cout << "Add and get pair test passed!" << std::endl;
}

void test_get_nonexistent_pair() {
    // Set up a test directory
    std::string testDir = "/tmp/config_test_empty/";
    mkdir(testDir.c_str(), 0777);
    EnvVarGuard guard("XDG_CONFIG_HOME", testDir);

    Config config;

    // Test getting a key that doesn't exist
    assert(config.getPair("nonexistent_key") == "");

    std::cout << "Get nonexistent pair test passed!" << std::endl;
}

void test_multiple_pairs() {
    // Set up a test directory
    std::string testDir = "/tmp/config_test_multiple/";
    mkdir(testDir.c_str(), 0777);
    EnvVarGuard guard("XDG_CONFIG_HOME", testDir);

    Config config;

    // Add multiple key-value pairs
    config.addPair("key1", "value1");
    config.addPair("key2", "value2");
    config.addPair("key3", "value3");

    // Check if all values can be retrieved
    assert(config.getPair("key1") == "value1");
    assert(config.getPair("key2") == "value2");
    assert(config.getPair("key3") == "value3");

    std::cout << "Multiple pairs test passed!" << std::endl;
}

void runAll() {
    std::cout << "Running Config tests..." << std::endl;
    test_constructor_with_xdg_config();
    test_constructor_with_home();
    test_add_and_get_pair();
    test_get_nonexistent_pair();
    test_multiple_pairs();
    std::cout << "All Config tests passed!" << std::endl;
}

} // namespace config_tests
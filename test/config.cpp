#include "../src/include/config.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

// Helper functions stay the same
bool fileExists(const std::string &path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool directoryExists(const std::string &path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && (buffer.st_mode & S_IFDIR));
}

// EnvVarGuard stays the same
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
    std::string testDir = "/tmp/config_test_xdg/";
    mkdir(testDir.c_str(), 0777);

    // Set XDG_CONFIG_HOME to the temp directory
    EnvVarGuard guard("XDG_CONFIG_HOME", testDir);

    // Create Config object
    Config config;

    // We can't check configDir directly anymore, but we can verify the config
    // file exists
    assert(fileExists(testDir + CONFIG_FILE));

    std::cout << "Constructor with XDG_CONFIG_HOME test passed!" << std::endl;
}

void test_constructor_with_home() {
    // Unset XDG_CONFIG_HOME and set HOME
    unsetenv("XDG_CONFIG_HOME");
    std::string testHome = "/tmp/config_home_test";
    mkdir(testHome.c_str(), 0777);
    EnvVarGuard homeGuard("HOME", testHome);

    // Expected config path
    std::string expectedDir = testHome + "/.config/" + GAME_FOLDER + "/";

    // Pre-create the .config directory to ensure permissions are correct
    std::string configDir = testHome + "/.config";
    mkdir(configDir.c_str(), 0777);

    // Create Config object
    Config config;

    // Check if directory was created
    assert(directoryExists(expectedDir));

    // Check if config file was created
    assert(fileExists(expectedDir + CONFIG_FILE));

    std::cout << "Constructor with HOME test passed!" << std::endl;
}

void test_default_config_creation() {
    // Set up a test directory
    std::string testDir = "/tmp/config_test_default/";
    mkdir(testDir.c_str(), 0777);
    EnvVarGuard guard("XDG_CONFIG_HOME", testDir);

    // Create a new config object, which should create default settings
    Config config;

    // Test that default keys exist using the public getConfig method
    assert(config.getConfig(Config::KB_UP) == "w");
    assert(config.getConfig(Config::KB_DOWN) == "s");
    assert(config.getConfig(Config::KB_LEFT) == "a");
    assert(config.getConfig(Config::KB_RIGHT) == "d");
    assert(config.getConfig(Config::KB_USE_PICKAXE) == "e");
    assert(config.getConfig(Config::KB_USE_RATION) == "r");
    assert(config.getConfig(Config::KB_EXIT) == "q");

    std::cout << "Default config creation test passed!" << std::endl;
}

void test_config_persistence() {
    // Set up a test directory
    std::string testDir = "/tmp/config_test_persistence/";
    mkdir(testDir.c_str(), 0777);
    EnvVarGuard guard("XDG_CONFIG_HOME", testDir);

    // First, create a config object which will set up defaults
    {
        Config config;
        // Default values should be created
    }

    // Now create a second config object which should load the existing values
    {
        Config config2;
        // Test that default keys exist
        assert(config2.getConfig(Config::KB_UP) == "w");
        assert(config2.getConfig(Config::KB_DOWN) == "s");
    }

    std::cout << "Config persistence test passed!" << std::endl;
}

void test_config_file_format() {
    // Set up a test directory
    std::string testDir = "/tmp/config_test_format/";
    mkdir(testDir.c_str(), 0777);
    EnvVarGuard guard("XDG_CONFIG_HOME", testDir);

    // Create Config object
    Config config;

    // Manually check the config file format
    std::string configFilePath = testDir + CONFIG_FILE;
    std::ifstream file(configFilePath);

    bool foundUp = false;
    bool foundDown = false;
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("KB_UP=w") != std::string::npos) {
            foundUp = true;
        }
        if (line.find("KB_DOWN=s") != std::string::npos) {
            foundDown = true;
        }
    }

    assert(foundUp);
    assert(foundDown);

    std::cout << "Config file format test passed!" << std::endl;
}

void runAll() {
    std::cout << "Running Config tests..." << std::endl;
    test_constructor_with_xdg_config();
    test_constructor_with_home();
    test_default_config_creation();
    test_config_persistence();
    test_config_file_format();
    std::cout << "All Config tests passed!" << std::endl;
}

} // namespace config_tests

#include <iostream>

namespace player_tests {
    void runAll();
}

namespace vector2d_tests {
    void runAll();
}

namespace utils_tests {
    void runAll();
}

namespace display_tests {
    void runAll();
}

int main() {
    std::cout << "Running all tests..." << std::endl;
    
    player_tests::runAll();
    vector2d_tests::runAll();
    utils_tests::runAll();
    display_tests::runAll();
    
    std::cout << "All tests completed successfully!" << std::endl;
    return 0;
}

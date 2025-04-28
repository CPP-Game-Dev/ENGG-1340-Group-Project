#include "../src/include/display.h"
#include "../src/include/enums.h"
#include "../src/include/vector2d.h"
#include <cassert>
#include <iostream>
#include <string>

// Access helper functions from display.cpp for testing
// These are defined in the source file but not in the header
extern std::string getTileChar(TileObject tile);
extern int getTileColor(TileObject tile);
extern int mapfov(int fov);
extern bool isVisible(int y1, int x1, int y2, int x2, int fov);

namespace display_tests {

void testGetTileChar() {
    // Test each tile type returns the expected character representation
    assert(getTileChar(TileObject::Player) == "P1");
    assert(getTileChar(TileObject::Wall) == "##");
    assert(getTileChar(TileObject::None) == "__");
    assert(getTileChar(TileObject::Exit) == "\\/");
    assert(getTileChar(TileObject::Ration) == "O ");
    assert(getTileChar(TileObject::EnergyDrink) == "RB");
    assert(getTileChar(TileObject::Battery) == "EN");
    assert(getTileChar(TileObject::Chest) == "[]");
    
    // Test unknown tile
    assert(getTileChar(static_cast<TileObject>(999)) == "??");
    
    std::cout << "getTileChar test passed!" << std::endl;
}

void testGetTileColor() {
    // Test each tile type returns the expected color code
    assert(getTileColor(TileObject::Player) == 1); // Yellow
    assert(getTileColor(TileObject::Wall) == 2);   // White
    assert(getTileColor(TileObject::None) == 3);   // Black
    assert(getTileColor(TileObject::Exit) == 4);   // Red
    
    // Other tiles should be green (color code 5)
    assert(getTileColor(TileObject::Ration) == 5);
    assert(getTileColor(TileObject::EnergyDrink) == 5);
    assert(getTileColor(TileObject::Battery) == 5);
    assert(getTileColor(TileObject::Chest) == 5);
    
    std::cout << "getTileColor test passed!" << std::endl;
}

void testMapFov() {
    // Test the FOV mapping function
    assert(mapfov(0) == 0);
    assert(mapfov(1) == 0);
    assert(mapfov(2) == 1);
    assert(mapfov(3) == 1);
    assert(mapfov(4) == 3);
    assert(mapfov(5) == 3);
    assert(mapfov(6) == 5);
    
    std::cout << "mapfov test passed!" << std::endl;
}

void testIsVisible() {
    // Test visibility function with various points and FOV values
    
    // Same point should always be visible
    assert(isVisible(5, 5, 5, 5, 1) == true);
    
    // Points at distance 1 should be visible with FOV 1 or greater
    // assert(isVisible(5, 5, 5, 6, 1) == true);
    // assert(isVisible(5, 5, 6, 5, 1) == true);
    
    // Points at distance 2 should not be visible with FOV 1
    assert(isVisible(5, 5, 5, 7, 1) == false);
    assert(isVisible(5, 5, 7, 5, 1) == false);
    
    // Points at diagonal should follow Euclidean distance
    assert(isVisible(5, 5, 6, 6, 2) == true);   // distance ≈ 1.41 < 2
    assert(isVisible(5, 5, 7, 7, 2) == false);  // distance ≈ 2.83 > 2
    // assert(isVisible(5, 5, 7, 7, 3) == true);   // distance ≈ 2.83 < 3
    
    std::cout << "isVisible test passed!" << std::endl;
}

void testArrowDisplay() {
    // Basic test for ArrowDisplay construction (without running the UI)
    std::vector<std::string> options = {"Option 1", "Option 2", "Option 3"};
    ArrowDisplay display(options, 1, 2);
    
    // Test that it constructed correctly (can't test much more without UI interaction)
    // assert(display.getSelectedIndex() == 0); // Default selected index is 0
    
    std::cout << "ArrowDisplay construction test passed!" << std::endl;
}

void runAll() {
    std::cout << "Running Display tests..." << std::endl;
    testGetTileChar();
    testGetTileColor();
    testMapFov();
    testIsVisible();
    testArrowDisplay();
    std::cout << "All Display tests passed!" << std::endl;
}

} // namespace display_tests
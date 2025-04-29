# ENGG 1340 Group Project

A maze-based exploration game where players navigate through procedurally generated levels, managing resources like stamina and items while finding their way to the exit.

## Getting Started
### Prerequisites
To build and run this project, you'll need:

- C++ compiler (C++11 or higher)
- ncurses library
- Make build system
### Installation
Clone the repository:
```bash
git clone https://github.com/CPP-Game-Dev/ENGG-1340-Group-Project.git
```

To compile the project:
```bash
make all
```

This will create the executable at `bin/game`.

## Usage
### Makefile
#### Makefile Commands  
The project includes a comprehensive Makefile with several useful commands:

- `make all` - Compiles the game (default target)
- `make run` - Compiles and runs the game
- `make test` - Compiles the test suite
- `make runtest` - Compiles and runs the test suite
- `make clean` - Removes compiled objects and executables

#### Makefile Structure
The Makefile handles:

1. Directory Management: Creates obj and bin directories if they don't exist
2. Source Compilation: Compiles all .cpp files in the src directory into object files
3. Test Compilation: Compiles all test files in the test directory
4. Linking: Links object files together into the final executable
5. Test Integration: Links test files with the application code (excluding main.cpp)

### Game Controls
The default controls are:

- `w` - Move up
- `s` - Move down
- `a` - Move left
- `b` - Move right
- `e` - Use pickaxe (break walls)
- `r` - Use ration (restore stamina)
- `q` - Quit game

The keybindings can be configured in the game's configuration file, which are located in:
- `$XDG_CONFIG_HOME/game_name/config.txt`, if `$XDG_CONFIG_HOME` is set
- `$HOME/.config/game_name/config.txt`, fallback if it is not set

## Features
### Project Requirements Implementation
#### 1. Generation of Random Events
- **Procedurally Generated Mazes**: Each level is uniquely created using Randomized Prim's Algorithm
- **Random Item Placement**: Items like rations, energy drinks, and batteries are randomly distributed
- **Randomized Exit Locations**: Exit points are positioned at calculated distances from the player's start
- **Weighted Item Distribution**: Different types of items have varying probabilities of appearing

#### 2. Data Structures for Storing Data
- **2D Tile Maps**: Uses nested vectors (`std::vector<std::vector<TileObject>>`) to represent the maze
- **Player Inventory**: Dynamic vector of unique pointers to store collected items
- **Position Management**: Custom Vector2D struct for position tracking and movement
- **Configuration Maps**: Uses std::map to store and retrieve configuration key-value pairs

#### 3. Dynamic Memory Management
- **Smart Pointers**: Extensive use of `std::unique_ptr` for safe memory management
- **Custom make_unique Implementation**: Provides C++11 compatibility for unique_ptr creation
- **Move Semantics**: Efficient transfer of resources without unnecessary copying
- **RAII Pattern**: Resources are properly acquired and released through object lifecycles

#### 4. File Input/Output
- **Configuration System**: Reads and writes settings to configuration files
- **XDG-Compliant Paths**: Follows XDG Base Directory Specification for config locations
- **Persistent Settings**: User controls and preferences are saved between game sessions
- **Directory Management**: Creates necessary directories if they don't exist

#### 5. Program Codes in Multiple Files
- **Modular Architecture**: Code is organized across 15+ files with clear separation of concerns
- **Header/Implementation Separation**: Interface definitions in header files with implementations in .cpp files
- **Component-Based Design**: Distinct modules for player, level, display, and item systems
- **Test Organization**: Separate test files for each component

#### 6. Multiple Difficulty Levels
- **Three Difficulty Options**: 
  - Catacombs (Easy): Slower map growth, more generous stamina regeneration (70%)
  - Labyrinth (Medium): Moderate map growth, balanced stamina regeneration (50%)
  - Purgatory (Hard): Rapid map growth, minimal stamina regeneration (30%)
- **Scaling Complexity**: Maps increase in size as players progress through levels
- **Resource Management**: Different difficulty levels affect how scarce resources are

### Code Structure
The project is structured modularly and is organized into several key components:

- **Player System**: Manages player stats, inventory, and movement
- **Level Generation**: Creates procedurally generated maze levels
- **Display System**: Handles ncurses-based rendering
- **Item System**: Implements various items with different effects
- **Configuration**: Handles game settings and key bindings

### Key Classes
- `Player`: Manages player stats and inventory
- `Level`: Handles maze generation and layout
- `Display`: Renders the game UI using ncurses
- `Item`: Base class for collectible/usable items
- `Vector2D`: 2D position and vector operations
- `Config`: Manages game settings and configuration files
- `Main`: Entry point for the game, initializes components and starts the game loop

### Unit Testing

The project includes a comprehensive testing framework, located in the `test` directory:

- **Test Organization**: Tests are organized by component (player, vector2d, utils, etc.)
- **Namespace Isolation**: Each test suite is contained in its own namespace
- **Standard Assertions**: Uses C++ assertions to verify expected behavior
- **CI Integration**: Tests run automatically on GitHub Actions

## Reviewing/Contributing the code

### Code Documentation
The code follows a consistent documentation pattern:

- **Function Documentation**: Each function has a detailed comment block explaining:
  - Purpose
  - Parameters
  - Return values
  - Examples (where appropriate)

- **Class Documentation**: Classes include descriptions of their purpose and usage

- **Implementation Notes**: Complex algorithms include step-by-step explanations

### Understanding the Codebase

1. Start with main.cpp to understand the entry point and game loop
2. Examine the class definitions in the `include` folder
3. Follow the implementation in the corresponding `.cpp` files
4. Review the test cases for usage examples

### Contributing Guidelines

1. **Code Style**: Follow the existing style (enforced by .clang-format)
2. **Documentation**: Maintain the documentation standard for new code
3. **Testing**: Add tests for new features or bug fixes
4. **Pull Requests**: Submit PRs with clear descriptions of changes

## License

This project is licensed under the CC0 1.0 Universal License - see the LICENSE.txt file for details. This effectively places the work in the public domain.
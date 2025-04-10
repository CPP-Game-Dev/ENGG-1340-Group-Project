#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>
#include <cmath> // For distance calculation
#include <iostream>
#include <ncurses.h>
struct Vector2D {
    int x;
    int y;
    Vector2D(int y, int x) {
        this->y = y;
        this->x = x;
    }
    Vector2D() {}

    bool operator==(const Vector2D &rhs) const {
        return this->x == rhs.x && this->y == rhs.y;
    }

    bool operator!=(const Vector2D &rhs) const { return !(*this == rhs); }
};

enum class TileObject {
    Player,
    Wall,
    None,
    Exit,
    Ration,
    EnergyDrink,
    Battery,
    Chest
};

class Level {
  private:
    std::vector<std::vector<TileObject> > maze;
    std::vector<Vector2D> wallList;
    Vector2D startPos;
    Vector2D endPos;
    int size;

    void generateMaze(Vector2D pos) {
        this->maze[pos.y][pos.x] = TileObject::None;
        getAdjWalls(pos);

        while (this->wallList.size() > 0 || pos == this->startPos) {
            print();
            int index = int(rand() % this->wallList.size());
            pos = this->wallList[index];
            if (verifyWall(pos)) {
                this->maze[pos.y][pos.x] = TileObject::None;
                getAdjWalls(pos);
            }
            wallList.erase(wallList.begin() + index);
        }
    }

    void getAdjWalls(Vector2D pos) {
        if (pos.y > 0 && this->maze[pos.y - 1][pos.x] == TileObject::Wall)
            this->wallList.push_back(Vector2D(pos.y - 1, pos.x));

        if (pos.x > 0 && this->maze[pos.y][pos.x - 1] == TileObject::Wall)
            this->wallList.push_back(Vector2D(pos.y, pos.x - 1));

        if (pos.y < this->size - 1 &&
            this->maze[pos.y + 1][pos.x] == TileObject::Wall)
            this->wallList.push_back(Vector2D(pos.y + 1, pos.x));

        if (pos.x < this->size - 1 &&
            this->maze[pos.y][pos.x + 1] == TileObject::Wall)
            this->wallList.push_back(Vector2D(pos.y, pos.x + 1));
    }

    bool verifyWall(Vector2D wallPos) {
        int count = 0;
        if (wallPos.y > 0 &&
            this->maze[wallPos.y - 1][wallPos.x] == TileObject::None)
            count++;

        if (wallPos.x > 0 &&
            this->maze[wallPos.y][wallPos.x - 1] == TileObject::None)
            count++;

        if (wallPos.y < this->size - 1 &&
            this->maze[wallPos.y + 1][wallPos.x] == TileObject::None)
            count++;

        if (wallPos.x < this->size - 1 &&
            this->maze[wallPos.y][wallPos.x + 1] == TileObject::None)
            count++;

        return count == 1;
    }

    void setExit() {
        std::vector<Vector2D> tileList = std::vector<Vector2D>();
        Vector2D pos;
        for (int i = 0; i < this->size; i++) {
            for (int j = 0; j < this->size; j++) {
                pos = Vector2D(i, j);
                if (this->maze[i][j] == TileObject::None) {
                    if (std::abs(startPos.y - i) + std::abs(startPos.x - j) <
                        this->size)
                        continue;
                    tileList.push_back(pos);
                }
            }
        }
        pos = tileList[int(rand() % tileList.size())];
        this->maze[pos.y][pos.x] = TileObject::Exit;
        this->endPos = pos;
        print();
    }

    void placeItems(int count) {
        TileObject items[] = {TileObject::Ration, TileObject::EnergyDrink,
                              TileObject::Battery, TileObject::Chest};
        int weights[] = {66, 22, 2, 10};

        std::vector<Vector2D> pathList = std::vector<Vector2D>();
        for (int i = 0; i < this->size; i++)
            for (int j = 0; j < this->size; j++)
                if (this->maze[i][j] == TileObject::None)
                    pathList.push_back(Vector2D(i, j));

        std::random_device rng;
        std::mt19937 eng(rng());
        std::shuffle(pathList.begin(), pathList.end(), eng);
        std::vector<Vector2D> selectedTiles =
            std::vector<Vector2D>(pathList.begin(), pathList.begin() + count);
        for (auto pos : selectedTiles) {
            TileObject item = TileObject::None;
            int rnd = rand() % 100;
            for (int i = 0; i < 4; i++) {
                rnd -= weights[i];
                if (rnd <= 0) {
                    item = items[i];
                    break;
                }
            }
            this->maze[pos.y][pos.x] = item;
            print();
        }
    }

  public:
    Level(int size, Vector2D startPos, int itemCount) {
        this->size = size;
        this->startPos = startPos;
        this->maze = std::vector<std::vector<TileObject> >(
            size, std::vector<TileObject>(size, TileObject::Wall));
        this->wallList = std::vector<Vector2D>();
        generateMaze(this->startPos);
        this->maze[startPos.y][startPos.x] = TileObject::Player;
        print();
        setExit();
        placeItems(itemCount);
    }
    std::vector<std::vector<TileObject> > getMaze() { return this->maze; }
    void print() {
        system("clear");

        TileObject tile;
        char x;
        std::string row = "##";
        for (int i = 0; i < this->size; i++) {
            row.append("#");
        }
        std::cout << row << "\n";
        for (int i = 0; i < this->size; i++) {
            std::cout << "#";
            for (int j = 0; j < this->size; j++) {
                tile = this->maze[i][j];
                x = ' ';
                switch (tile) {
                case TileObject::Player:
                    x = '@';
                    break;
                case TileObject::Wall:
                    x = '#';
                    break;
                case TileObject::Exit:
                    x = 'O';
                    break;
                case TileObject::Chest:
                    x = '?';
                    break;
                case TileObject::Ration:
                    x = '8';
                    break;
                case TileObject::EnergyDrink:
                    x = '0';
                    break;
                case TileObject::Battery:
                    x = '$';
                    break;
                default:
                    break;
                }
                std::cout << x;
            }
            std::cout << "#\n";
        }
        std::cout << row << "\n";

        usleep(1000000 * 0.02);
    }

    Vector2D getEndPos() { return this->endPos; }
};


#define MEGAPIXEL "  "
char getTileChar(TileObject tile) {
    switch (tile) {
    case TileObject::Player:
        return 'P';
    case TileObject::Wall:
        return '#';
    case TileObject::None:
        return '.';
    case TileObject::Exit:
        return 'E';
    case TileObject::Ration:
        return 'R';
    case TileObject::EnergyDrink:
        return 'D';
    case TileObject::Battery:
        return 'B';
    case TileObject::Chest:
        return 'C';
    default:
        return '?'; // Unknown tile
    }
}

int getTileColor(TileObject tile) {
    switch (tile) {
    case TileObject::Player:
        return 1; // Player as Yellow
    case TileObject::Wall:
        return 2; // Wall as White
    case TileObject::None:
        return 3; // None as Black
    case TileObject::Exit:
        return 4; // Exit as Red
    default:
        return 5; // Other tiles as Green
    }
}
int mapfov(int fov) {
    if (fov == 0 || fov == 1) {
        return 0;
    } else {
        if (fov % 2 == 0) {
            return fov - 1;
        } else {
            return fov - 2;
        }
    }
}

bool checkValid(int playerX, int playerY, int fov) {
    return playerX >= 0 && playerY >= 0 && fov >= 0;
}

void printMap(const std::vector<std::vector<TileObject> > &map, int playerX,
              int playerY, int fov) {
    for (int i = 0; i < (int)map.size(); i++) {
        for (int j = 0; j < (int)map[i].size(); j++) {
            if (i >= playerY - fov && i <= playerY + fov &&
                j >= playerX - fov && j <= playerX + fov) {
                // Tile is within the field of view
                int tile_color = getTileColor(map[i][j]);

                attron(COLOR_PAIR(tile_color));
                
                if (map[i][j] == TileObject::Wall || map[i][j] == TileObject::Player ||
                    map[i][j] == TileObject::None || map[i][j] == TileObject::Exit) {
                    addstr(MEGAPIXEL);; // Print two spaces as a "block"
                } else {
                    char t = getTileChar(map[i][j]);
                    printw("%c ", t);
                }

                attroff(COLOR_PAIR(tile_color));
            } else {
                // Tile Outside Field of view
                attron(COLOR_PAIR(6));
                addstr(MEGAPIXEL);
                attroff(COLOR_PAIR(6));
            }
        }
        printw("\n");
    }
}

void TesterProgram() {
    Level level1 = Level(9, Vector2D(0, 0), 3);
    std::vector<std::vector<TileObject> > map = 
    {
        {TileObject::Wall, TileObject::Wall, TileObject::Wall, TileObject::Wall, TileObject::Wall},
        {TileObject::Wall, TileObject::None, TileObject::None, TileObject::None, TileObject::Wall},
        {TileObject::Wall, TileObject::None, TileObject::Player, TileObject::None, TileObject::Wall},
        {TileObject::Wall, TileObject::None, TileObject::None, TileObject::Exit, TileObject::Wall},
        {TileObject::Wall, TileObject::Wall, TileObject::Wall, TileObject::Wall, TileObject::Wall}
    };
    std::vector<std::vector<TileObject> > map2 = level1.getMaze();

    int playerX = 2; // Player's X position in the map
    int playerY = 2; // Player's Y position in the map
    int fieldOfView = 10; // Field of view (player as center, near n blocks displayed)

    // Initialize ncurses
    initscr();
    start_color(); // Enable color functionality

    // Define color pairs
    init_pair(1, COLOR_BLACK, COLOR_YELLOW); // Player
    init_pair(2, COLOR_WHITE, COLOR_WHITE);  // Wall
    init_pair(3, COLOR_BLACK, COLOR_BLACK);  // None
    init_pair(4, COLOR_BLACK, COLOR_RED);    // Exit
    init_pair(5, COLOR_GREEN, COLOR_BLACK);  // Extras
    init_pair(6, COLOR_BLACK, COLOR_CYAN);   // Hidden
    // Print the map with the player's field of view
    printMap(map, playerX, playerY, mapfov(fieldOfView));

    // Refresh the screen to show the map
    refresh();

    // Sample proof-of-concept to get input
    while (true) {
        char ch = getch();

        if (ch == 'w') {
            printw("Forward");
        } else if (ch == 'a') {
            printw("left");
        } else if (ch == 's') {
            printw("back");
        } else if (ch == 'd') {
            printw("right");
        } else if (ch == 'q') {
            break;
        } else {
            printw("Invalid input");
        }
    }
    //
    // Deallocate memory and end ncurses
    endwin();
}

int main() {
    TesterProgram();
    /*
        Gamemethod: printmap -> getch -> move(modification of tiles) ->get
       player pos -> printmap -> REPEAT
    */
    return 0;
}

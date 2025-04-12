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

int main() {
    srand(time(NULL));
    Level level1 = Level(9, Vector2D(0, 0), 3);
    level1.print();
    sleep(3);
    Level level2 = Level(12, level1.getEndPos(), 5);
    level2.print();
}
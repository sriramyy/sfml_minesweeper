#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_set>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Tile.cpp"

class GameState {

public:
    // Tracks the play status of the game, which is reflected in the behavior of the user interface and visualizations.
    enum PlayStatus{ WIN, LOSS, PLAYING };
    friend class Toolbox;

    // Constructs a new random game state with specified tile _dimensions and the specified _numberOfMines
    GameState(sf::Vector2i _dim = sf::Vector2i(25,16), int _numberOfMines = 50) {
        dimensions = _dim;
        mineCount = _numberOfMines;
        playStatus = PLAYING;
        board.resize(dimensions.x, std::vector<Tile*>(dimensions.y, nullptr));

        for (int y = 0; y < dimensions.y; y++) {
            for (int x = 0; x < dimensions.x; x++) {
                board[x][y] = new Tile(sf::Vector2f(x*32.f,y*32.f)); // *32.f for tile width/height
            }
        }

        // placing mines randomly
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> xDist(0, dimensions.x - 1);
        std::uniform_int_distribution<> yDist(0, dimensions.y - 1);

        int placedMines = 0;
        std::unordered_set<int> occupied; // to track positions

        while (placedMines < mineCount) {
            int x = xDist(gen);
            int y = yDist(gen);

            // unique position (x + y * width)
            int pos = x + y * dimensions.x;

            if (occupied.find(pos) == occupied.end()) {
                // this places mine
                occupied.insert(pos);
                board[x][y]->isMine = true;
                board[x][y]->setState(Tile::HIDDEN);
                placedMines++;
            }
        }

        flagCount = 0;

    }

    // Constructs a game state based on the size, mine placement, and mine number specified at filepath.
    GameState(const char* filepath) {

        // open file
        std::ifstream file(filepath);
        if (!file.is_open()) { std::cout<< "can't open file (" << filepath << ")" << std::endl; }

        std::string line;
        std::vector<std::vector<int>> boardNum;

        while (std::getline(file, line)) {
            std::vector<int> row;
            for (int i = 0; i < line.size(); i++) {
                char c = line[i];
                if (c == '1') {
                    row.push_back(1);
                } else if (c == '0') {
                    row.push_back(0);
                } else {
                    std::cout << "invalid character in file" << std::endl;
                }
            }
            boardNum.push_back(row);
        }
        file.close();

        // PRINT OUT HERE
        // std::cout << "Contents of boardNum loaded from file (" << filepath << "):" << std::endl;
        // for (int y = 0; y < boardNum.size(); y++) {
        //     for (int x = 0; x < boardNum[y].size(); x++) {
        //         std::cout << boardNum[y][x];
        //     }
        //     std::cout << std::endl;
        // }

        // set variables
        dimensions = sf::Vector2i(boardNum[0].size(), boardNum.size());
        mineCount = 0;
        playStatus = PLAYING;
        flagCount = 0;

        //std::cout << "MADE HERE 3" << std::endl;
        // make board
        board.resize(dimensions.x, std::vector<Tile*>(dimensions.y, nullptr));

        for (int y = 0; y < dimensions.y; y++) {
            for (int x = 0; x < dimensions.x; x++) {
                board[x][y] = new Tile(sf::Vector2f(x*32.f,y*32.f)); // *32.f for tile width/height
                board[x][y]->isMine = false;
            }
        }


        for (int y = 0; y < boardNum.size(); y++) {
            for (int x = 0; x < boardNum[y].size(); x++) {
                if (boardNum[y][x] == 1) {
                    board[x][y]->isMine = true;
                    mineCount++;
                }
            }
        }


    }

    // Current count of the number of flags placed on the screen.
    int getFlagCount() {
        return flagCount;
    }

    // Current count of the number of mines actually on the board.
    int getMineCount() {
        return mineCount;
    }

    // Returns a pointer to the Tile at the specified coordinates, or nullptr if out of bounds.
    Tile* getTile(int x, int y) {
        return board[x][y];
    }

    // Returns the play status of the game.
    PlayStatus getPlayStatus() {
        return playStatus;
    }

    // Sets the play status of the game.
    void setPlayStatus(PlayStatus _status) {
        playStatus = _status;
    }


private:
    sf::Vector2i dimensions;
    int mineCount;
    int flagCount = 0;
    PlayStatus playStatus;
    std::vector<std::vector<Tile*>> board;

};
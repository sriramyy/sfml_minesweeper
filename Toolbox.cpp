#include <iostream>
#include <string>
#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include "GameState.cpp"
#include "Button.cpp"
#include <SFML/Graphics/Texture.hpp>
#include "minesweeper.h"

class Toolbox {
public:

    sf::RenderWindow window;
    GameState* gameState; // primary game state representation
    Button* debugButton; // reveals mines
    Button* newGameButton; // resets/starts new game
    Button* testButton1; // loads test board 1
    Button* testButton2; // loads test board 2
    Button* testButton3; // loads test board 3

    // for digits
    sf::Texture digitsTexture;
    std::vector<sf::Sprite> digitSprites;

    // for mines around
    std::vector<sf::Texture> numberTextures;


    // Returns a reference to the singular Toolbox instance.
    static Toolbox& getInstance() {
        static Toolbox instance; // static makes sure only single instance
        return instance;
    };

    // custom method to return and load reference to a texture
    sf::Texture& getTexture(const std::string& filename) {
        auto texture = textures.find(filename);
        if (texture == textures.end()) {
            sf::Texture tex;
            if (!tex.loadFromFile(filename)) {
                std::cout << "FAILED TO LOAD TEXTURE";
            }
            textures[filename] = std::move(tex);
        }
    }

private:

    // map for textures
    std::map<std::string, sf::Texture> textures;

    // Default constructor; should be accessible only from within the class itself. This method initializes the buttons,
    // window, game board, and any other elements necessary to play the game
    Toolbox() {
        window.create(sf::VideoMode(800,600), "P4 - Minesweeper, Sriram Yerramsetty");

        // create gamestatr
        gameState = new GameState({25,16}, 50);

        // create all the instances for buttons

        debugButton = new Button({500,512}, toggleDebugMode);
        newGameButton = new Button({370,512}, restart);
        testButton1 = new Button({562,512}, []() {
            Toolbox& instance = Toolbox::getInstance();
            delete instance.gameState;
            instance.gameState = new GameState("boards/testboard1.brd");
            instance.calcAdjacentMines();
        });
        testButton2 = new Button ({624,512}, []() {
            Toolbox& instance = Toolbox::getInstance();
            delete instance.gameState;
            instance.gameState = new GameState("boards/testboard2.brd");
            instance.calcAdjacentMines();
        });
        testButton3 = new Button({686, 512}, []() {
            Toolbox& instance = Toolbox::getInstance();
            delete instance.gameState;
            instance.gameState = new GameState("boards/testboard3.brd");
            instance.calcAdjacentMines();
        });

        // create the digits
        digitsTexture.loadFromFile("images/digits.png");

        for (int i =0; i<3; i++) {
            sf::Sprite digit;
            digit.setTexture(digitsTexture);
            digit.setPosition(0.f + i * 21.f, 512.f);
            digitSprites.push_back(digit);
        }

        for (int i = 1; i <= 8; i++) {
            sf::Texture texture;
            texture.loadFromFile("images/number_" + std::to_string(i) + ".png");
            numberTextures.push_back(texture);
        }

        calcAdjacentMines();

    }

    // added for flag counter (digits)
public:

    // increases flag count
    void incFlag() {
        this->gameState->flagCount += 1;
    }

    // decreases flag count
    void decFlag() {
        this->gameState->flagCount -= 1;
    }

    // update digits sprites
    void updateCounter(int flagsLeft) {
        int pos_flagsLeft = std::abs(flagsLeft);
        for (int i = 2; i >= 0; i--) {
            int digit = pos_flagsLeft % 10;
            digitSprites[i].setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
            pos_flagsLeft /= 10;
        }
        if (flagsLeft < 0) { // for negative
            digitSprites[0].setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
        }
    }


    // added for debugMode
private:
    bool debugMode = false;

public:
    bool getDebugModeTool() {
        return debugMode;
    }

    void setDebugMode(bool debug) {
        debugMode = debug;
    }

    // added for mine detection
public:
    bool checkMine(int x, int y) {
        return this->gameState->getTile(x,y)->isMine;
    }

    // added for numbers (neighbors)
public:
    std::array<Tile*, 8> findNeighbors(Tile* tile) {
        std::array<Tile*, 8> neighbors{};
        int count = 0;

        GameState* gameState = this->gameState;
        sf::Vector2f position = tile->getLocation();
        int x = static_cast<int>(position.x/32.f);
        int y = static_cast<int>(position.y/32.f);

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) {
                    continue;
                }
                int nx = x + i;
                int ny = y + j;

                if (nx >= 0 && nx < gameState->dimensions.x && ny >= 0 && ny < gameState->dimensions.y) {
                    neighbors[count++] = gameState->board[nx][ny];
                }
            }
        }
        tile->setNeighbors(neighbors);
        return neighbors;
    }

    void calcAdjacentMines() {
        // std::cout << "should be " << gameState->dimensions.x << " " << gameState->dimensions.y << std::endl;
        for (int y = 0; y < gameState->dimensions.y; y++) {
            for (int x = 0; x < gameState->dimensions.x; x++) {
                Tile* tile = gameState->board[x][y];
                if (tile && !tile->isMine) {
                    auto neighbors = findNeighbors(tile);
                    int count = 0;


                    // check if each neighbor a mine
                    for (int i = 0; i <neighbors.size(); i++){
                        Tile* neighbor = neighbors[i];
                        if (neighbor && neighbor->isMine) {
                            count++;
                        }
                    }

                    tile->adjacentMineCount = count;
                }
            }
        }
    }

    // get the number of adjacent mines
    int getAdjacentMines(Tile* tile) {
        // std::cout<< "there are " << tile->adjacentMineCount << " adjacent mines " << std::endl;
        return tile->adjacentMineCount;

    }

    void revealNeighTool(Tile* tile) {
        tile->revealNeighbors();
    }

    // added for adding flags in debugMode
    bool checkFlag(int x, int y) {
        return this->gameState->getTile(x,y)->hasFlag;
    }

    bool checkFlag(Tile* tile) {
        return tile->hasFlag;
    }

    // added for custom dimension boards
    int getWidth() {
        return this->gameState->dimensions.x;
    }

    int getHeight() {
        return this->gameState->dimensions.y;
    }

    int setWidth(int width) {
        this->gameState->dimensions.x = width;
    }

    void setHeight(int height) {
        this->gameState->dimensions.y = height;
    }
};

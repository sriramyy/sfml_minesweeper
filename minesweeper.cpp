#include <iostream>
#include <SFML/Graphics.hpp>
#include "Toolbox.cpp"
#include "minesweeper.h"

// Sriram Yerramsetty
// COP 3504C
// Project 4
// (note) for some reason my SFML files weren't being recognized in my path so instead I just moved the .dll
//        files to my cmake-build-debug file in the project file and it worked fine.

// Global Functions

// Draws the all UI elements according to the current gameState and debug mode.
void render() {
    Toolbox& instance = Toolbox::getInstance();
    instance.window.clear(sf::Color::White);


    // draw the tiles
    for (int y = 0; y < instance.getHeight(); y++) {
        for (int x = 0; x < instance.getWidth(); x++) {
            Tile* tile = instance.gameState->getTile(x, y);
            if (tile) {
                tile->draw(instance.window);

                if (tile->getState() == Tile::REVEALED) {
                    int adjacentMines = instance.getAdjacentMines(tile);
                    if (adjacentMines > 0) {
                        sf::Sprite numberSprite;
                        numberSprite.setTexture(instance.numberTextures[adjacentMines-1]);
                        numberSprite.setPosition(tile->getLocation().x, tile->getLocation().y);
                        instance.window.draw(numberSprite);

                    }

                }
            }
        }
    }

    // set sprite to buttons
    sf::Texture debugTexture;
    debugTexture.loadFromFile("images/debug.png");
    instance.debugButton->setSprite(new sf::Sprite(debugTexture));
    sf::Texture newGameTexture;
    newGameTexture.loadFromFile("images/face_happy.png");
    instance.newGameButton->setSprite(new sf::Sprite(newGameTexture));
    sf::Texture test1Texture;
    test1Texture.loadFromFile("images/test_1.png");
    instance.testButton1->setSprite(new sf::Sprite(test1Texture));
    sf::Texture test2Texture;
    test2Texture.loadFromFile("images/test_2.png");
    instance.testButton2->setSprite(new sf::Sprite(test2Texture));
    sf::Texture test3Texture;
    test3Texture.loadFromFile("images/Test_3.png");
    instance.testButton3->setSprite(new sf::Sprite(test3Texture));


    // set position of the buttons
    instance.debugButton->getSprite()->setPosition(instance.debugButton->getPosition());
    instance.newGameButton->getSprite()->setPosition(instance.newGameButton->getPosition());
    instance.testButton1->getSprite()->setPosition(instance.testButton1->getPosition());
    instance.testButton2->getSprite()->setPosition(instance.testButton2->getPosition());
    instance.testButton3->getSprite()->setPosition(instance.testButton3->getPosition());


    // draw all the buttons
    if (instance.debugButton->getSprite()) {
        instance.window.draw(*instance.debugButton->getSprite());
        instance.window.draw(*instance.debugButton->getSprite());
    }
    if (instance.newGameButton->getSprite()) {
        instance.window.draw(*instance.newGameButton->getSprite());
        // instance.window.draw(*instance.newGameButton->getSprite());
    }
    if (instance.testButton1->getSprite()) {
        instance.window.draw(*instance.testButton1->getSprite());
        // instance.window.draw(*instance.testButton1->getSprite());
    }
    if (instance.testButton2->getSprite()) {
        instance.window.draw(*instance.testButton2->getSprite());
        // instance.window.draw(*instance.testButton2->getSprite());
    }
    if (instance.testButton3->getSprite()) {
        instance.window.draw(*instance.testButton3->getSprite());
    }


    // draw the counter
    for (int i = 0; i < static_cast<int>(instance.digitSprites.size()); i++) {
        instance.window.draw(instance.digitSprites[i]);
    }

    // if the game is loss then:
    if (instance.gameState->getPlayStatus() == GameState::LOSS) {
        newGameTexture.loadFromFile("images/face_lose.png");
        instance.window.draw(*instance.newGameButton->getSprite());
        for (int y = 0; y < instance.getHeight(); y++) {
            for (int x = 0; x < instance.getWidth(); x++) {
                if (instance.checkMine(x,y)) {
                    instance.gameState->getTile(x, y)->setState(Tile::EXPLODED);
                }
            }
        }
    }

    // if the gmae is won then:
    if (instance.gameState->getPlayStatus() == GameState::WIN) {
        // std::cout<<"HELLO U OWN!";
        newGameTexture.loadFromFile("images/face_win.png");
        instance.window.draw(*instance.newGameButton->getSprite());
    }

    instance.window.display();
}

// Flips the debug mode on/off. (Debug mode should initially be off/false.)
void toggleDebugMode() {
    Toolbox& instance = Toolbox::getInstance();

    instance.setDebugMode(!instance.getDebugModeTool());

    // std::cout<<"ok now debug mode is " <<instance.getDebugModeTool()<<std::endl;

    for (int y = 0; y < instance.getHeight(); y++) {
        for (int x = 0; x < instance.getWidth(); x++) {
            Tile* tile = instance.gameState->getTile(x,y);
            if (tile) {
                if (instance.getDebugModeTool()) {// debugMode ON
                    if (instance.checkMine(x,y)) {
                        tile->setState(Tile::State::EXPLODED);
                    }
                } else { // debugMode OFF
                    if (instance.checkMine(x,y)) {
                        tile->setState(Tile::HIDDEN);
                    }
                    if (instance.checkFlag(x,y)) {
                        tile->setState(Tile::FLAGGED);
                    }

                }
            }
        }
    }


}

// Returns the true if debug mode is active, and false otherwise.
bool getDebugMode() {
    Toolbox& instance = Toolbox::getInstance();
    return instance.getDebugModeTool();
}


// This method is invoked directly by main() and is responsible for the game’s launch. It should be possible to
// directly invoke this function after including the submitted source and header files in the test suite.
int launch() {

    Toolbox& instance = Toolbox::getInstance();

    while (instance.window.isOpen()) {
        sf::Event event;
        while (instance.window.pollEvent(event)) {
            // close when x pressed
            if (event.type == sf::Event::Closed)
                instance.window.close();
        }


        if (event.type == sf::Event::MouseButtonPressed) {

            // this is to make sure the mouse press does not spam
            event.type = sf::Event::Count;

            sf::Vector2i mousePos = sf::Mouse::getPosition(instance.window);

            // check if debug button clicked
            if (instance.debugButton->getSprite()->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                // std::cout<<"DEBUG CLICKED";
                instance.debugButton->onClickFunction();
            }

            // check if new game button clicked
            if (instance.newGameButton->getSprite()->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                instance.newGameButton->onClickFunction();
            }

            // check if test button 1 clicked
            if (instance.testButton1->getSprite()->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                instance.testButton1->onClickFunction();
            }

            // check if test button 2 clicked
            if (instance.testButton2->getSprite()->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                instance.testButton2->onClickFunction();
            }

            // check if test button 3 clicked
            if (instance.testButton3->getSprite()->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                instance.testButton3->onClickFunction();
            }

            // check if we are playing (or else lock controls)
            if (instance.gameState->getPlayStatus() == GameState::PLAYING) {
                // check if a tile is clicked
                for(int y = 0; y < instance.getHeight(); y++) {
                    for (int x = 0; x < instance.getWidth(); x++) {
                        Tile* tile = instance.gameState->getTile(x,y);
                        if (tile) {
                            sf::Vector2f tilePos = tile->getLocation();
                            sf::Vector2f tileSize(32.f,32.f); // size

                            if (mousePos.x >= tilePos.x && mousePos.x < tilePos.x + tileSize.x && mousePos.y >= tilePos.y && mousePos.y < tilePos.y + tileSize.y) {
                                //std::cout<< "CLICKED IN TILE";

                                // click to place flag
                                if (event.mouseButton.button == sf::Mouse::Right) {
                                    if (tile->getState() != Tile::REVEALED) {
                                        if (!instance.checkFlag(tile)) {
                                            instance.incFlag();
                                        } else if (instance.checkFlag(tile)) {
                                            instance.decFlag();
                                        }
                                        tile->onClickRight();
                                    }
                                }

                                // click to reveal tile
                                if (event.mouseButton.button == sf::Mouse::Left) {
                                    tile->onClickLeft();
                                    // instance.printAdjacent(tile);
                                    if (tile->getState() == Tile::EXPLODED) {
                                        instance.gameState->setPlayStatus(GameState::LOSS);
                                    }
                                    if (instance.getAdjacentMines(tile) == 0) {
                                        instance.revealNeighTool(tile);
                                    }
                                }
                            }
                        }
                    }
                }

                // check if game won
                if (instance.gameState->getFlagCount() == instance.gameState->getMineCount()) {
                    int count = 0;
                    for(int y = 0; y < instance.getHeight(); y++) {
                        for (int x = 0; x < instance.getWidth(); x++) {
                            if (instance.checkMine(x,y) && instance.checkFlag(x,y)) {
                                count++;
                            }
                        }
                    }
                    if (count == instance.gameState->getMineCount()) {
                        // std::cout<< "GAME HAS BEEN WON";
                        instance.gameState->setPlayStatus(GameState::WIN);
                    } else {
                        instance.gameState->setPlayStatus(GameState::PLAYING);
                    }
                }
            }

        }

        instance.updateCounter(50 - instance.gameState->getFlagCount());
        render();
    }

    return 0;
}

// Resets all states/objects and generates a default game state (random board) and turns off debug mode if active.
// The new state should be a default game state (25x16 with 50 randomly placed mines).
void restart() {
    Toolbox& instance = Toolbox::getInstance();
    delete instance.gameState;
    instance.gameState = new GameState({25, 16}, 50);
    instance.calcAdjacentMines();
    launch();


}


int main() {
    return launch();
}

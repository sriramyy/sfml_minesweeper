#include <functional>
#include <iostream>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "minesweeper.h"


class Tile {

public:
    // Represents tile’s current UI state (visualization).
    enum State { REVEALED, HIDDEN, FLAGGED, EXPLODED };
    friend class Toolbox;
    friend class GameState;

    // Constructs a new tile object at the designated _position.
    Tile(sf::Vector2f _position) {
        state = HIDDEN;
        position = _position;
        sprite.setPosition(position);
        if (!texture.loadFromFile("images/tile_hidden.png")) {
            std::cerr << "FAILED TO LOAD IMAGE";
        }
        sprite.setTexture(texture);

        if (!flagTexture.loadFromFile("images/flag.png")) {
            std::cerr << "FAILED TO LOAD IMAGE";
        }
        flagSprite.setTexture(flagTexture);
        flagSprite.setPosition(position);
    }

    // Returns the position of this tile
    sf::Vector2f getLocation() {
        return position;
    }

    // Returns the current state of this tile
    State getState() {
        return state;
    }

    // Returns pointer to array of Tile pointers
    std::array<Tile*, 8>& getNeighbors() {
        return neighbors;
    }

    // Sets the state of this tile
    // Should trigger other behaviors related to the state change (including visualization).
    void setState(State _state) {
        state = _state;
        if (state == HIDDEN) {
            if(!texture.loadFromFile("images/tile_hidden.png")) {
                std::cerr << "FAILED TO LOAD HIDDEN IMAGE";
            }
        } else if (state == REVEALED) {
            if (!texture.loadFromFile("images/tile_revealed.png")) {
                std::cerr << "FAILED TO LOAD REVEALED IMAGE";
            }
        } else if (state == FLAGGED) {
            hasFlag = true;
            if (!texture.loadFromFile("images/tile_hidden.png")) {
                std::cerr << "FAILED TO LOAD FLAGGED IMAGE";
            }
        } else if (state == EXPLODED) {
            if (!texture.loadFromFile("images/mine.png")) {
                std::cerr << "FAILED TO LOAD MINE IMAGE";
            }
        }

        sprite.setTexture(texture);
    }

    // Populates/replaces the neighboring tile container
    void setNeighbors(std::array<Tile*, 8> _neighbors) {
        neighbors = _neighbors;
    }

    // Defines the reveal behavior for a tile when left mouse button is clicked
    void onClickLeft() {
        if (isMine && state == HIDDEN) {
            setState(EXPLODED);
        } else if (!isMine && state == HIDDEN) {
            setState(REVEALED);
        }

    }

    // Toggles this tile's state between FLAGGED and HIDDEN
    void onClickRight() {
        if (state != REVEALED) {
            if (state == HIDDEN) { // if tile hidden, make flag
                setState(FLAGGED);
                hasFlag = true;
            } else if (state == FLAGGED) { // if tile flag, make hidden
                setState(HIDDEN);
                hasFlag = false;
            } else if (!hasFlag && getDebugMode()) { // if state not flag, and debugMode on, set state to flag
                hasFlag = true;
            } else if (hasFlag && getDebugMode()) { // if state flag, and debugMode on, set state to not flag
                hasFlag = false;
            } else if (hasFlag) { // if state flag set state to flag
                setState(FLAGGED);
                hasFlag = true;
            }
        }
    }

    // Render this tile to the screen according to is state
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
        if (state == FLAGGED) {
            window.draw(flagSprite);
        }
    }

protected:
    // Based on State and mine content of the tile neighbors, set their state to REVEALED
    void revealNeighbors() {
        for (Tile* neighbor : neighbors) {
            if (neighbor && neighbor->getState() == HIDDEN && !neighbor->isMine) {
                neighbor->onClickLeft(); // go through and reveal all neighbors

                // check if the neighbor tile has no adjacenemt mines then reveal all its neighbors
                if (neighbor->adjacentMineCount == 0) {
                    neighbor->revealNeighbors();
                }
            }
        }
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Sprite flagSprite;
    sf::Texture flagTexture;
    State state; // state
    sf::Vector2f position; // position
    std::array<Tile*, 8> neighbors{}; // array for neighboring tiles (pointers)

    bool isMine;
    int adjacentMineCount;
    bool hasFlag = false;


};

#include <functional>
#include <iostream>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Button {

public:
    sf::Vector2f position;
    std::function<void()> onClickFunction;
    sf::Sprite* sprite;


    // Constructs a new object at the specified _position which invokes the _onClick callback when clicked.
    Button(sf::Vector2f _position, std::function<void()> _onClick) {
        position = _position;

        onClickFunction = _onClick;
        sprite = nullptr;
    }

    // Returns the position of the button.
    sf::Vector2f getPosition() {
        return position;
    }

    // Returns the current sprite of the button.
    sf::Sprite* getSprite() {
        return sprite;
    }

    // Sets this button’s visualization to the specified _sprite
    void setSprite(sf::Sprite* _sprite) {
        sprite = _sprite;
    }

    // Invokes the button’s callback method (usually called when clicked).
    void onClick() {

    }




};

#include "Button.h"

Button::Button() {

}

void Button::setBackground(sf::Color background, sf::Color hoverBackground = sf::Color(-1,-1,-1)) {
    this->background = background;
    this->activeBackground = background;
    if (hoverBackground.r < 0 || hoverBackground.g < 0 || hoverBackground.b < 0) {
        this->hoverBackground = background;
    }
    else {
        this->hoverBackground = hoverBackground;
    }
}

void Button::setLabel(std::string label) {
    this->label = label;
}

void Button::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void Button::setSize(double width, double height) {
    this->width = width;
    this->height = height;
}


void Button::setColor(sf::Color color) {
    this->color = color;
}


bool Button::checkBorders(sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (mousePos.x > this->x && mousePos.x < this->x + this->width) {
        if (mousePos.y > this->y && mousePos.y < this->y + this->height) {
            return true;
        }
    }
    return false;
}

bool Button::checkClick(sf::RenderWindow& window)
{
    if (checkBorders(window)) {
        return true;
    }

    return false;
}


void Button::show(sf::RenderWindow& window, sf::Font *font, bool isHover) {
    // Button
    this->button.setPosition(this->x, this->y);
    this->button.setFillColor(this->background);
    this->button.setSize(sf::Vector2f(this->width, this->height));

    window.draw(this->button);

    // Label
    sf::Text labelText;
    labelText.setFont(*font);

    labelText.setString(this->label);
    labelText.setCharacterSize(this->fontSize);

    float labelWidth = labelText.findCharacterPos(labelText.getString().getSize()).x;
    float posX = this->x + (this->width - labelWidth) / 2;
    float posY = this->y + (this->height - this->fontSize) / 2;
    labelText.setPosition(posX, posY);

    labelText.setFillColor(this->color);

    window.draw(labelText);
    
    if (isHover) {
        addHover(window);
    }
}

void Button::addHover(sf::RenderWindow& window) {
    if (checkBorders(window)) {
        sf::Cursor cursor;
        if (cursor.loadFromSystem(sf::Cursor::Hand))
            window.setMouseCursor(cursor);
    
        this->background = this->hoverBackground;
    }

    else {
        sf::Cursor cursor;
        if (cursor.loadFromSystem(sf::Cursor::Arrow))
            window.setMouseCursor(cursor);

        this->background = this->activeBackground;
    }
}


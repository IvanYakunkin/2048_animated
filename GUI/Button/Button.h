#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <filesystem>

class Button {
public:

    double height = 40;
    double width = 150;

    double x = 100;
    double y = 100;

    int fontSize = 19;

    sf::RectangleShape button;

    sf::Color background = sf::Color(112, 112, 112);
    sf::Color color = sf::Color(255, 255, 255);
    sf::Color hoverBackground;

    std::string label = "Click";

    Button();
    void show(sf::RenderWindow& window, sf::Font *font, bool isHover);
    void setPosition(int x, int y);
    void setBackground(sf::Color background, sf::Color hoverBackground);
    void setSize(double width, double height);
    void setLabel(std::string label);
    void setColor(sf::Color color);
    bool checkClick(sf::RenderWindow& window);

private:
    // Needed for hover color
    sf::Color activeBackground;
    void addHover(sf::RenderWindow& window);
    bool checkBorders(sf::RenderWindow& window);
};
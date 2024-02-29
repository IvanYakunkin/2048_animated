#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>     
#include <time.h>      
#include <filesystem>
#include <array>
#include <cmath>
#include <vector>
#include <map>

const std::string TITLE = "2048";

const int WIN_WIDTH = 900;
const int WIN_HEIGHT = 800;

const int FIELD_WIDTH = 500;
const int FIELD_HEIGHT = 500;

// Color for empty square
const sf::Color EMPTY_SQUARE_COLOR = sf::Color(205, 193, 180);
// Color between squares
const sf::Color FIELD_COLOR = sf::Color(187, 173, 160);
const sf::Color BACKGROUND_COLOR = sf::Color(217, 217, 217);

const double FIELD_POS_X = WIN_WIDTH / 2 - FIELD_WIDTH / 2;
const double FIELD_POS_Y = WIN_HEIGHT / 2 - FIELD_HEIGHT / 2;

// Delay after arrow-click 
const int RESTART_TIMER = 240;
double DELAY_SPEED = 0.8;

const int SPACE_BETWEEN_TILES = 10;
const int BORDER_SIZE = 10;
const sf::Color BORDER_COLOR = sf::Color(187, 173, 160);

const int CODE_LABEL_SIZE = 30;
const sf::Color CODE_LABEL_COLOR = sf::Color(255, 255, 255);

bool IS_LOSE = false;

std::string REPLAY_TEXT_LABEL = "Press ENTER to continue!";
std::string LOSE_TEXT_LABEL = "You lose!";
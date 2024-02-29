
#include "2048.h"
#include "GUI/Button/Button.h"

class Square
{
public:
    int code = 0;
    double size;
    int spaceBetweenTiles = SPACE_BETWEEN_TILES;

    double defaultSpeed = 50;
    double speed = defaultSpeed;
    // Modifier for speed
    double speedModify = 0.015;

    // Animation flags
    bool isAdding = false;
    bool wasAdding = false;
    bool isStop = false;

    // Delay for new square appearance
    bool isDelay = false;
    double delayValue = 255;

    // Color of the filled cell
    sf::Color filledColor;

    // Animation destination. -1 by default
    double animationEnd = -1;

    // Coords of the cell (x,y)
    double coords[2];
    double gridSquareSize = static_cast<double> (FIELD_WIDTH - spaceBetweenTiles * 3) / 4;


    // Colors
    std::map<int, sf::Color> squareColors = {
        {2, sf::Color(238,228,218)},
        {4, sf::Color(238,225,201)},
        {8, sf::Color(243,178,122)},
        {16, sf::Color(246,150,100)},
        {32, sf::Color(247,124,95)},
        {64, sf::Color(247,95,59)},
        {128, sf::Color(237,208,115)},
        {256, sf::Color(237,204,98)},
        {512, sf::Color(237,201,80)},
        {1024, sf::Color(237,197,63)},
        {2048, sf::Color(237,194,46)}
    };

    Square() {
        size = static_cast<double> (FIELD_WIDTH - spaceBetweenTiles * 3) / 4;
    }

    void updateColor() {
        // Update color by code
        for (auto color = squareColors.begin(); color != squareColors.end(); color++) {
            if (code != 0) {
                if (color->first == this->code) {
                    filledColor = sf::Color(color->second.a, color->second.g, color->second.b, this->delayValue);
                }
            }
        }
    }

    void updateFilledColor() {
        filledColor.a = static_cast<int>(delayValue);
    }

    // Sete coords by position x and y
    void setCoordsByPos(int y, int x) {
        coords[0] = FIELD_POS_X + (gridSquareSize + spaceBetweenTiles) * x;
        coords[1] = FIELD_POS_Y + (gridSquareSize + spaceBetweenTiles) * y;
    }

    // Get coords by position
    double getXCoordByPos(int pos) {
        double coord = FIELD_POS_X + (gridSquareSize + spaceBetweenTiles) * pos;

        return coord;
    }

    double getYCoordByPos(int pos) {
        double coord = FIELD_POS_Y + (gridSquareSize + spaceBetweenTiles) * pos;

        return coord;
    }
};

void drawBorder(sf::RenderWindow& window) {
    sf::RectangleShape border(sf::Vector2f(FIELD_WIDTH, FIELD_HEIGHT));
    border.setPosition(FIELD_POS_X, FIELD_POS_Y);
    border.setFillColor(sf::Color(0, 0, 0, 0));
    border.setOutlineThickness(BORDER_SIZE);
    border.setOutlineColor(BORDER_COLOR);
    window.draw(border);
}

void drawCells(sf::RenderWindow& window) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            double gridSquareSize = static_cast<double> (FIELD_WIDTH - SPACE_BETWEEN_TILES * 3) / 4;
            sf::RectangleShape tile(sf::Vector2f(gridSquareSize, gridSquareSize));
            tile.setPosition(FIELD_POS_X + (gridSquareSize + SPACE_BETWEEN_TILES) * x, FIELD_POS_Y + (gridSquareSize + SPACE_BETWEEN_TILES) * y);
            tile.setFillColor(EMPTY_SQUARE_COLOR);
            window.draw(tile);
        }
    }
}

void drawField(sf::RenderWindow& window) {
    sf::RectangleShape fieldRect(sf::Vector2f(FIELD_WIDTH, FIELD_HEIGHT));
    fieldRect.setPosition(FIELD_POS_X, FIELD_POS_Y);
    fieldRect.setFillColor(FIELD_COLOR);
    window.draw(fieldRect);

    // Draw Border
    drawBorder(window);
    drawCells(window);

}


void processMovementAndAnimation(Square& currentSquare, Square& nextSquare, int x, int y, int nextX, int animationEndXModifier, int coordId, int* movementCounter) {
    if (currentSquare.code == 0 || currentSquare.isStop) {
        return;
    }
    if (coordId == 0 && x == nextX) {
        return;
    }
    if (coordId == 1 && y == nextX) {
        return;
    }

    // Set animation destination
    if (nextSquare.code == 0 && currentSquare.animationEnd == -1 || nextSquare.animationEnd != -1) {
        if (coordId == 0) {
            currentSquare.animationEnd = currentSquare.getXCoordByPos(nextX);
        }
        else {
            currentSquare.animationEnd = currentSquare.getYCoordByPos(nextX);
        }
    }

    else if (currentSquare.code == nextSquare.code && currentSquare.code != 0 && currentSquare.animationEnd == -1 && !currentSquare.wasAdding && !nextSquare.wasAdding) {
        if (coordId == 0) {
            currentSquare.animationEnd = currentSquare.getXCoordByPos(nextX);
        }
        else {
            currentSquare.animationEnd = currentSquare.getYCoordByPos(nextX);
        }
        currentSquare.isAdding = true;
    }

    // Movement
    if (currentSquare.animationEnd != -1) {
        currentSquare.coords[coordId] += currentSquare.speed * currentSquare.speedModify * animationEndXModifier;
        *movementCounter += 1;

        // Stop animation if element reached destination 
        if ((animationEndXModifier == 1 && currentSquare.animationEnd <= currentSquare.coords[coordId]) ||
            (animationEndXModifier == -1 && currentSquare.animationEnd >= currentSquare.coords[coordId])) {
            currentSquare.animationEnd = -1;
            // Cell union
            if (currentSquare.isAdding) {
                currentSquare.code = 0;
                nextSquare.code *= 2;
                nextSquare.wasAdding = true;
                currentSquare.isAdding = false;
                nextSquare.updateColor();
                return;
            }
            // Move to empty cells
            if (nextSquare.code == 0) {
                Square temp = nextSquare;
                nextSquare = currentSquare;
                currentSquare = temp;
            }
        }
    }
}

void speed(Square field[4][4]) {
    for (int y = 0; y < 4; y++) {
        int speedCounter = 0;
        for (int x = 0; x < 4; x++) {
            if (field[y][x].code != 0) {
                speedCounter++;
            }
        }
        for (int x = 0; x < 4; x++) {
            if (field[y][x].code != 0) {
                field[y][x].speed = field[y][x].defaultSpeed / speedCounter;
            }
        }
    }
}

void speedY(Square field[4][4]) {
    for (int y = 0; y < 4; y++) {
        int speedCounter = 0;
        for (int x = 0; x < 4; x++) {
            if (field[x][y].code != 0) {
                speedCounter++;
            }
        }
        for (int x = 0; x < 4; x++) {
            if (field[x][y].code != 0) {
                field[x][y].speed = field[x][y].defaultSpeed / speedCounter;
            }
        }
    }
}

void drawSquares(sf::RenderWindow& window, Square field[4][4], sf::Text text, std::string* moveDirection, int* movementCounter) {

    if (*moveDirection == "right") {
        for (int y = 3; y > -1; y--) {
            for (int x = 3; x > -1; x--) {
                if (x < 3) {
                    processMovementAndAnimation(field[y][x], field[y][x + 1], x, y, x + 1, 1, 0, movementCounter);
                }
            }
        }
    }
    else if (*moveDirection == "left") {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (x > 0) {
                    processMovementAndAnimation(field[y][x], field[y][x - 1], x, y, x - 1, -1, 0, movementCounter);
                }
            }
        }
    }
    else if (*moveDirection == "down") {
        for (int x = 3; x > -1; x--) {
            for (int y = 3; y > -1; y--) {
                if (y < 3) {
                    processMovementAndAnimation(field[y][x], field[y + 1][x], x, y, y + 1, 1, 1, movementCounter);
                }
            }
        }
    }
    else if (*moveDirection == "top") {
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (y > 0) {
                    processMovementAndAnimation(field[y][x], field[y - 1][x], x, y, y - 1, -1, 1, movementCounter);
                }
            }
        }
    }

    // Draw squares and labels for them 
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {

            // SQUARE
            if (field[y][x].code != 0) {

                // Delay for new square
                if (field[y][x].isDelay) {
                    field[y][x].delayValue += DELAY_SPEED;
                    field[y][x].updateFilledColor();
                    if (field[y][x].delayValue >= 255) {
                        field[y][x].isDelay = false;
                    }
                }

                sf::RectangleShape tile(sf::Vector2f(field[y][x].size, field[y][x].size));
                int spaceBetweenTiles = 10;
                double gridSquareSize = static_cast<double> (FIELD_WIDTH - spaceBetweenTiles * 3) / 4;

                // if there are undefined coords than define them
                if (std::isnan(field[y][x].coords[0])) {
                    field[y][x].setCoordsByPos(y, x);
                }

                tile.setPosition(field[y][x].coords[0], field[y][x].coords[1]);
                tile.setFillColor(field[y][x].filledColor);
                window.draw(tile);

                // LABEL
                text.setString(std::to_string(field[y][x].code));
                sf::FloatRect textBounds = text.getLocalBounds();
                text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                text.setPosition(tile.getPosition() + sf::Vector2f(tile.getSize().x / 2, tile.getSize().y / 2));
                text.setCharacterSize(CODE_LABEL_SIZE);

                if (field[y][x].code == 2 || field[y][x].code == 4) {
                    text.setFillColor(sf::Color(0, 0, 0));
                }
                else {
                    text.setFillColor(CODE_LABEL_COLOR);
                }

                window.draw(text);
            }
        }
    }
}

void refreshAnimationBlock(Square field[4][4]) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            field[y][x].wasAdding = false;
            field[y][x].isStop = false;
        }
    }
}

// Return true if lose
bool isLose(Square(&field)[4][4], std::vector<std::array<int, 2>> freeCells) {
    if (freeCells.size() - 1 == 0) {
        bool isLose = true;

        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                // Check for horizontal
                if (field[y][x].code == field[y][x + 1].code && field[y][x].code != 0) {
                    isLose = false;
                }
                // Check for vertical
                if (field[x][y].code == field[x + 1][y].code && field[x][y].code != 0) {
                    isLose = false;
                }
            }
        }

        if (isLose) {
            return true;
        }
    }
    return false;
}

void setSquare(Square(&field)[4][4]) {
    // Get free squares position 
    std::vector<std::array<int, 2>> freeCells;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (field[y][x].code == 0) {
                freeCells.push_back({ y, x });
            }
        }
    }

    // Get random position for a new square
    int range = freeCells.size();
    int randEl = rand() % range;

    // Set a new element
    // Define the code of the new element
    int elRange = 100;
    int elRand = rand() % elRange + 1;
    int otherElementPercent = 15; // code "4" will be generated with a 15% probability
    int newCode = 0;

    if (elRand <= otherElementPercent) {
        newCode = 4;
    }
    else {
        newCode = 2;
    }

    std::array<int, 2> freePos = freeCells[randEl];
    field[freePos[0]][freePos[1]].code = newCode;
    field[freePos[0]][freePos[1]].coords[0] = std::nan("1");
    field[freePos[0]][freePos[1]].isDelay = true;
    field[freePos[0]][freePos[1]].isDelay = true;
    field[freePos[0]][freePos[1]].delayValue = 0;
    field[freePos[0]][freePos[1]].isStop = true;
    field[freePos[0]][freePos[1]].updateColor();

    // Check lose
    if (isLose(field, freeCells)) {
        IS_LOSE = true;
    }

}

void showLoseWindow(sf::RenderWindow& window) {
    sf::RectangleShape aboveLayer(sf::Vector2f(FIELD_WIDTH, FIELD_HEIGHT));
    aboveLayer.setPosition(FIELD_POS_X - 10, FIELD_POS_Y - 10);
    aboveLayer.setFillColor(sf::Color(238, 228, 218, 203));
    aboveLayer.setSize(sf::Vector2f(520, 520));
    window.draw(aboveLayer);
}

void showLoseMessage(sf::RenderWindow& window, sf::Text loseText, std::string loseTextLabel) {
    loseText.setString(loseTextLabel);
    loseText.setCharacterSize(100);
    loseText.setPosition(FIELD_POS_X + FIELD_WIDTH / 2 - 200, FIELD_POS_Y + FIELD_HEIGHT / 2 - 75);
    loseText.setStyle(sf::Text::Bold);
    loseText.setFillColor(sf::Color(119, 110, 101));

    window.draw(loseText);

}

void showContinueMessage(sf::RenderWindow& window, sf::Text replayText, std::string replayTextLabel) {
    replayText.setString(replayTextLabel);

    replayText.setCharacterSize(30);
    replayText.setPosition(FIELD_POS_X + FIELD_WIDTH / 2 - 160, FIELD_POS_Y + FIELD_HEIGHT / 2 + 50);
    replayText.setFillColor(sf::Color(119, 110, 101));

    window.draw(replayText);
}


void reloadGame(Square(&field)[4][4]) {
    IS_LOSE = false;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            field[y][x].code = 0;
        }
    }
}

int main()
{

    srand(time(NULL));

    std::string executablePath = std::filesystem::absolute(".").string();
    std::filesystem::current_path(executablePath);

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), TITLE);

    // Font 
    std::string fontFolder = "/fonts/";
    std::string fontName = "Roboto-Regular.ttf";
    std::string fullFontPath = executablePath + fontFolder + fontName;
    sf::Font font;

    if (!font.loadFromFile(fullFontPath))
    {
        std::cout << "Unable to load the font";
        return 1;
    }

    sf::Text labelText, loseText, replayText;

    labelText.setFont(font);
    loseText.setFont(font);
    replayText.setFont(font);

    // Game field 4x4
    Square field[4][4];

    // Set default squares
    //setSquare(field);
    field[0][0].code = 2;
    field[0][0].coords[0] = std::nan("1");
    field[0][0].updateColor();

    field[0][1].code = 2;
    field[0][1].coords[0] = std::nan("1");
    field[0][1].updateColor();

    field[0][2].code = 4;
    field[0][2].coords[0] = std::nan("1");
    field[0][2].updateColor();

    // timer for delay after a key click
    sf::Clock timer;
    bool keyPressed = false;

    // Movement direction: top | down | right | left
    std::string moveDirection = "";

    // If there is no movement then there is no need to generate new square
    int movementCounter = 0;

    Button button = Button();
    button.setBackground(EMPTY_SQUARE_COLOR, sf::Color(217, 197, 176));
    button.setPosition(20, 20);
    button.setLabel("Replay");
    button.setColor(sf::Color::Black);
    
    while (window.isOpen())
    {
        window.clear(BACKGROUND_COLOR);

        sf::Event event;

        button.show(window, &font, true);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Button handler
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (button.checkClick(window)) {
                    reloadGame(field);
                    setSquare(field);
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !keyPressed && !IS_LOSE)
            {
                keyPressed = true;
                timer.restart();

                refreshAnimationBlock(field);
                movementCounter = 0;
                moveDirection = "left";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !keyPressed && !IS_LOSE)
            {
                keyPressed = true;
                timer.restart();

                refreshAnimationBlock(field);
                movementCounter = 0;
                moveDirection = "right";

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !keyPressed && !IS_LOSE)
            {
                keyPressed = true;
                timer.restart();

                refreshAnimationBlock(field);
                moveDirection = "down";
                movementCounter = 0;

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !keyPressed && !IS_LOSE)
            {
                keyPressed = true;
                timer.restart();

                refreshAnimationBlock(field);
                movementCounter = 0;
                moveDirection = "top";
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && IS_LOSE) {
                reloadGame(field);
                setSquare(field);
            }
        }

        // restart timer 
        if (keyPressed && timer.getElapsedTime().asMilliseconds() > RESTART_TIMER)
        {
            keyPressed = false;

            if (movementCounter != 0) {
                setSquare(field);
            }
        }

        drawField(window);

        drawSquares(window, field, labelText, &moveDirection, &movementCounter);
        std::cout << field[0][0].wasAdding << std::endl;
        // Check lose 
        if (IS_LOSE) {
            showLoseWindow(window);
            showLoseMessage(window, loseText, LOSE_TEXT_LABEL);
            showContinueMessage(window, replayText, REPLAY_TEXT_LABEL);
        }

        window.display();
    }

    return 0;
}

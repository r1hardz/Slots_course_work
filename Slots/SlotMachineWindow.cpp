#include "SlotMachineWindow.h"
#include <iomanip>
#include <sstream>
#include <iostream>

SlotMachineWindow::SlotMachineWindow(
    sf::RenderWindow& win,
    Database& database,
    const std::string& user
) :
    window(win),
    db(database),
    username(user),
    currentBet(1.0),
    isAnimating(false)
{
    initializeGUI();
}

void SlotMachineWindow::initializeGUI() {

    // load font
    if (!font.loadFromFile("Roboto-Black.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    // Load win sound
    if (!winSoundBuffer.loadFromFile("sounds/win.wav")) {
        std::cerr << "Error loading win sound!" << std::endl;
    }

    // sound volume
    winSound.setBuffer(winSoundBuffer);
    winSound.setVolume(5.0f);

    // Load background
    if (!backgroundTexture.loadFromFile("images/slot_background.jpg")) {
        std::cerr << "Error loading slot_background.jpg" << std::endl;
    }

    //background position and scaling
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );


    // load the symbols 
    if (!textures[SEVEN].loadFromFile("images/fruit.png")) {
        std::cout << "Error loading fruit.png" << std::endl;
    }

    if (!textures[LEMON].loadFromFile("images/lemon.png")) {
        std::cout << "Error loading lemon.png" << std::endl;
    }

    if (!textures[CHERRY].loadFromFile("images/cherries.png")) {
        std::cout << "Error loading cherries.png" << std::endl;
    }

    if (!textures[ORANGE].loadFromFile("images/orange.png")) {
        std::cout << "Error loading orange.png" << std::endl;
    }

    if (!textures[PLUM].loadFromFile("images/plum.png")) {
        std::cout << "Error loading plum.png" << std::endl;
    }

    if (!textures[BELL].loadFromFile("images/bell.png")) {
        std::cout << "Error loading bell.png" << std::endl;
    }

    if (!textures[DIAMOND].loadFromFile("images/diamond.png")) {
        std::cout << "Error loading diamond.png" << std::endl;
    }

    if (!textures[BAR].loadFromFile("images/bar.png")) {
        std::cout << "Error loading bar.png" << std::endl;
    }

    // Initialize panels 
    
    // Credits panel setup (show player's credits)
    creditsPanel.setSize(sf::Vector2f(300, 60));
    creditsPanel.setFillColor(sf::Color(0, 0, 0, 180));
    creditsPanel.setOutlineThickness(2);
    creditsPanel.setOutlineColor(sf::Color(255, 215, 0));
    creditsPanel.setPosition(30, window.getSize().y - 90);

    // Bet panel setup (shows current bet)
    betPanel.setSize(sf::Vector2f(200, 60));
    betPanel.setFillColor(sf::Color(0, 0, 0, 180));
    betPanel.setOutlineThickness(2);
    betPanel.setOutlineColor(sf::Color(255, 215, 0));
    betPanel.setPosition(window.getSize().x - 270, window.getSize().y - 90);

    // Initialize text elements

    // Credits text setup
    creditsText.setFont(font);
    creditsText.setCharacterSize(25);
    creditsText.setFillColor(sf::Color::White);
    creditsText.setOutlineThickness(3);
    creditsText.setOutlineColor(sf::Color::Black);

    // Bet amount text setup
    betText.setFont(font);
    betText.setCharacterSize(25);
    betText.setFillColor(sf::Color::White);
    betText.setOutlineThickness(3);
    betText.setOutlineColor(sf::Color::Black);

    // Winning message text setup
    winningsText.setFont(font);
    winningsText.setCharacterSize(36);
    winningsText.setFillColor(sf::Color::Yellow);
    winningsText.setOutlineThickness(2);
    winningsText.setOutlineColor(sf::Color::Black);
    winningsText.setPosition((window.getSize().x - winningsText.getLocalBounds().width) / 2, 80);

    // Initialize buttons

    // Main spin button setup 
    spinButton.setSize(sf::Vector2f(200, 60));
    spinButton.setFillColor(sf::Color(180, 0, 0));
    spinButton.setOutlineThickness(3);
    spinButton.setOutlineColor(sf::Color(255, 215, 0));
    spinButton.setPosition((window.getSize().x - 200) / 2, window.getSize().y - 150);

    // Spin button text setup
    spinButtonText.setFont(font);
    spinButtonText.setString("SPIN");
    spinButtonText.setCharacterSize(35);
    spinButtonText.setFillColor(sf::Color::White);
    spinButtonText.setOutlineThickness(2);
    spinButtonText.setOutlineColor(sf::Color::Black);
    centerText(spinButtonText, spinButton);

    // Bet adjustment buttons
    // Increase bet button (+)
    increaseBetButton.setSize(sf::Vector2f(40, 40));
    increaseBetButton.setFillColor(sf::Color(180, 0, 0));
    increaseBetButton.setOutlineThickness(2);
    increaseBetButton.setOutlineColor(sf::Color(255, 215, 0));
    increaseBetButton.setPosition(betPanel.getPosition().x + betPanel.getSize().x + 5, betPanel.getPosition().y + 10);

    // Decrease bet button (-)
    decreaseBetButton.setSize(sf::Vector2f(40, 40));
    decreaseBetButton.setFillColor(sf::Color(180, 0, 0));
    decreaseBetButton.setOutlineThickness(2);
    decreaseBetButton.setOutlineColor(sf::Color(255, 215, 0));
    decreaseBetButton.setPosition(betPanel.getPosition().x - 45, betPanel.getPosition().y + 10);

    // + text setup
    increaseBetText.setFont(font);
    increaseBetText.setString("+");
    increaseBetText.setCharacterSize(35);
    increaseBetText.setFillColor(sf::Color::White);
    increaseBetText.setOutlineThickness(2);
    increaseBetText.setOutlineColor(sf::Color::Black);
    increaseBetText.setPosition(
        // Center + in button
        increaseBetButton.getPosition().x + (increaseBetButton.getSize().x - increaseBetText.getLocalBounds().width) / 2,
        increaseBetButton.getPosition().y + (increaseBetButton.getSize().y - increaseBetText.getLocalBounds().height) / 2 - 12
    );

    // -  text setup
    decreaseBetText.setFont(font);
    decreaseBetText.setString("-");
    decreaseBetText.setCharacterSize(35);
    decreaseBetText.setFillColor(sf::Color::White);
    decreaseBetText.setOutlineThickness(2);
    decreaseBetText.setOutlineColor(sf::Color::Black);
    decreaseBetText.setPosition(

        // Center - in button
        decreaseBetButton.getPosition().x + (decreaseBetButton.getSize().x - decreaseBetText.getLocalBounds().width) / 2,
        decreaseBetButton.getPosition().y + (decreaseBetButton.getSize().y - decreaseBetText.getLocalBounds().height) / 2 - 18
    );

    initializeReels();
}

void SlotMachineWindow::initializeReels() {
    const float slotWidth = 150;
    const float slotHeight = 150;
    const float spacing = 10;

    const float totalWidth = REEL_COLS * slotWidth + (REEL_COLS - 1) * spacing;
    const float totalHeight = REEL_ROWS * slotHeight + (REEL_ROWS - 1) * spacing;
    const float gridX = (window.getSize().x - totalWidth) / 2;
    const float gridY = (window.getSize().y - totalHeight) / 2 - 50;

    // Create sprites for the symbols
    for (int row = 0; row < REEL_ROWS; ++row) {
        for (int col = 0; col < REEL_COLS; ++col) {
            sf::Sprite sprite;
            float x = gridX + col * (slotWidth + spacing);
            float y = gridY + row * (slotHeight + spacing);

            sprite.setTexture(textures[SEVEN]);
            sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

            float scaleX = (slotWidth * 0.8f) / sprite.getLocalBounds().width;
            float scaleY = (slotHeight * 0.8f) / sprite.getLocalBounds().height;
            float scale = std::min(scaleX, scaleY);
            sprite.setScale(scale, scale);

            sprite.setPosition(x + slotWidth / 2, y + slotHeight / 2);
            reelSprites.push_back(sprite);
        }
    }

    // Create grid lines
    sf::Color lineColor(50, 50, 50);
    for (int col = 1; col < REEL_COLS; ++col) {
        sf::RectangleShape line(sf::Vector2f(4, totalHeight));
        line.setPosition(gridX + col * (slotWidth + spacing) - spacing / 2, gridY);
        line.setFillColor(lineColor);
        gridLines.push_back(line);
    }

    for (int row = 1; row < REEL_ROWS; ++row) {
        sf::RectangleShape line(sf::Vector2f(totalWidth, 4));
        line.setPosition(gridX, gridY + row * (slotHeight + spacing) - spacing / 2);
        line.setFillColor(lineColor);
        gridLines.push_back(line);
    }

    reelsBackground.setSize(sf::Vector2f(totalWidth + 40, totalHeight + 40));
    reelsBackground.setPosition(gridX - 20, gridY - 20);
    reelsBackground.setFillColor(sf::Color(0, 0, 0, 180));
    reelsBackground.setOutlineThickness(5);
    reelsBackground.setOutlineColor(sf::Color(255, 165, 0));

    highlightedSprites.resize(reelSprites.size(), false);
    highlightIntensity = 0.0f;
}

void SlotMachineWindow::updateReels(const std::vector<std::vector<Symbol>>& results) {
    int index = 0;

    // go through each position in the 3x5 grid
    for (int row = 0; row < REEL_ROWS; ++row) {
        for (int col = 0; col < REEL_COLS; ++col) {
            sf::Vector2f pos = reelSprites[index].getPosition();
            sf::Vector2f scale = reelSprites[index].getScale();

            reelSprites[index].setTexture(textures[results[col][row]], true);
            reelSprites[index].setOrigin(
                reelSprites[index].getLocalBounds().width / 2,
                reelSprites[index].getLocalBounds().height / 2
            );
            reelSprites[index].setPosition(pos);
            reelSprites[index].setScale(scale);
            index++;
        }
    }
}

void SlotMachineWindow::handleButtonHover(sf::Vector2i mousePos)
{
    // change spin button color on hover
    bool spinHovered = spinButton.getGlobalBounds().contains(mousePos.x, mousePos.y);
    sf::Color spinColor = spinHovered ? sf::Color(220, 0, 0) : sf::Color(180, 0, 0);
    spinButton.setFillColor(spinColor);

    // change increase bet button color on hover
    bool increaseHovered = increaseBetButton.getGlobalBounds().contains(mousePos.x, mousePos.y);
    sf::Color increaseColor = increaseHovered ? sf::Color(220, 0, 0) : sf::Color(180, 0, 0);
    increaseBetButton.setFillColor(increaseColor);

    // change decrease bet button color on hover
    bool decreaseHovered = decreaseBetButton.getGlobalBounds().contains(mousePos.x, mousePos.y);
    sf::Color decreaseColor = decreaseHovered ? sf::Color(220, 0, 0) : sf::Color(180, 0, 0);
    decreaseBetButton.setFillColor(decreaseColor);
}

void SlotMachineWindow::updateHighlights() {
    if (!isAnimating) {
        for (auto& sprite : reelSprites) {
            sprite.setColor(sf::Color::White);
        }
        return;
    }

    float time = highlightClock.getElapsedTime().asSeconds();
    highlightIntensity = (std::sin(time * 5) + 1.0f) / 2.0f;

    for (size_t i = 0; i < reelSprites.size(); i++) {
        if (highlightedSprites[i]) {
            sf::Color glowColor(255, 255, 100, static_cast<sf::Uint8>(255 * highlightIntensity));
            reelSprites[i].setColor(glowColor);
        }
        else {
            reelSprites[i].setColor(sf::Color::White);
        }
    }
}

void SlotMachineWindow::highlightWinningSymbols(const std::vector<std::vector<Symbol>>& results, double payout) {
    std::fill(highlightedSprites.begin(), highlightedSprites.end(), false);

    if (payout > 0) {
        for (const auto& payline : gameLogic.getPaylines()) {
            Symbol firstSymbol = results[payline[0] % 5][payline[0] / 5];
            int consecutiveMatches = 1;
            std::vector<int> matchingPositions = { payline[0] };

            for (size_t i = 1; i < payline.size(); i++) {
                int pos = payline[i];
                int row = pos / 5;
                int col = pos % 5;
                if (results[col][row] == firstSymbol) {
                    consecutiveMatches++;
                    matchingPositions.push_back(pos);
                }
                else {
                    break;
                }
            }

            if (consecutiveMatches >= 2) {
                for (int pos : matchingPositions) {
                    highlightedSprites[pos] = true;
                }
            }
        }

        isAnimating = true;
        highlightClock.restart();
    }
    else {
        isAnimating = false;
    }
}

void SlotMachineWindow::handleSpin() {
    double currentCredits = db.getUserCredits(username);
    if (currentCredits >= currentBet) {
        auto results = gameLogic.spinReels();
        updateReels(results);
        double payout = gameLogic.calculatePayout(results, currentBet);
        db.updateUserCredits(username, currentCredits - currentBet + payout);
        highlightWinningSymbols(results, payout);

        if (payout > 0) {
            std::stringstream winningsStream;
            winningsStream << std::fixed << std::setprecision(2) << payout;
            winningsText.setString("You won " + winningsStream.str() + " credits!");
            winningsText.setPosition((window.getSize().x - winningsText.getLocalBounds().width) / 2, 25);
            winSound.play();
        }
        else {
            winningsText.setString("");
        }
    }
    else {
        winningsText.setString("Not enough credits!");
        winningsText.setPosition((window.getSize().x - winningsText.getLocalBounds().width) / 2, 25);
    }
}

void SlotMachineWindow::updateBetAmount(bool increase) {
    if (increase && currentBet < MAX_BET) {
        currentBet = std::min(MAX_BET, currentBet + BET_INCREMENT);
    }
    else if (!increase && currentBet > MIN_BET) {
        currentBet = std::max(MIN_BET, currentBet - BET_INCREMENT);
    }
}

void SlotMachineWindow::centerText(sf::Text& text, const sf::RectangleShape& container) {
    text.setPosition(
        container.getPosition().x + (container.getSize().x - text.getLocalBounds().width) / 2,
        container.getPosition().y + (container.getSize().y - text.getLocalBounds().height) / 2
    );
}

void SlotMachineWindow::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseMoved) {
                handleButtonHover(sf::Mouse::getPosition(window));
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (spinButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    handleSpin();
                }
                else if (decreaseBetButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    updateBetAmount(false);
                }
                else if (increaseBetButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    updateBetAmount(true);
                }
            }
        }

        updateHighlights();

        std::stringstream creditsStream, betStream;
        creditsStream << std::fixed << std::setprecision(2) << db.getUserCredits(username);
        betStream << std::fixed << std::setprecision(2) << currentBet;

        creditsText.setString("Credits: " + creditsStream.str());
        betText.setString("Bet: " + betStream.str());

        centerText(creditsText, creditsPanel);
        centerText(betText, betPanel);

        window.clear(sf::Color(50, 50, 50));
        window.draw(backgroundSprite);
        window.draw(reelsBackground);

        for (const auto& line : gridLines) {
            window.draw(line);
        }

        for (const auto& sprite : reelSprites) {
            window.draw(sprite);
        }

        window.draw(creditsPanel);
        window.draw(betPanel);
        window.draw(creditsText);
        window.draw(betText);
        window.draw(spinButton);
        window.draw(spinButtonText);
        window.draw(decreaseBetButton);
        window.draw(increaseBetButton);
        window.draw(decreaseBetText);
        window.draw(increaseBetText);
        window.draw(winningsText);
        window.display();
    }
}
#ifndef SLOT_MACHINE_WINDOW_H
#define SLOT_MACHINE_WINDOW_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include "Game.h"
#include "Database.h"

enum Symbol; 

class SlotMachineWindow {
public:
    // constructor takes window reference, database reference and username
    SlotMachineWindow(
        sf::RenderWindow& win,
        Database& database,
        const std::string& user
    );

    void run();  // the main game loop

private:
    void initializeGUI();    // sets up all GUI elements
    void initializeReels();  // sets up the slot machine reels

    // updating and handling functions
    void updateReels(const std::vector<std::vector<Symbol>>& results);
    void handleButtonHover(sf::Vector2i mousePos);
    void updateHighlights();
    void highlightWinningSymbols(
        const std::vector<std::vector<Symbol>>& results,
        double payout
    );
    void centerText(sf::Text& text, const sf::RectangleShape& container);
    void handleSpin();
    void updateBetAmount(bool increase);

    sf::RenderWindow& window;
    Game gameLogic;
    Database& db;
    std::string username;

    // game state variables
    double currentBet;
    std::vector<bool> highlightedSprites;
    sf::Clock highlightClock;
    float highlightIntensity;
    bool isAnimating;

    // resource handles
    sf::Font font;
    std::unordered_map<Symbol, sf::Texture> textures;
    sf::Texture backgroundTexture;
    sf::SoundBuffer winSoundBuffer;

    // games GUI elements
    sf::Sprite backgroundSprite;
    std::vector<sf::Sprite> reelSprites;
    std::vector<sf::RectangleShape> gridLines;
    sf::RectangleShape reelsBackground;
    sf::RectangleShape creditsPanel;
    sf::RectangleShape betPanel;
    sf::RectangleShape spinButton;
    sf::RectangleShape increaseBetButton;
    sf::RectangleShape decreaseBetButton;

    // games text display elements
    sf::Text creditsText;
    sf::Text betText;
    sf::Text winningsText;
    sf::Text spinButtonText;
    sf::Text increaseBetText;
    sf::Text decreaseBetText;

    // sound elements
    sf::Sound winSound;

    // games constants
    const double MIN_BET = 1.0;
    const double MAX_BET = 10.0;
    const double BET_INCREMENT = 0.5;
    const int REEL_ROWS = 3;
    const int REEL_COLS = 5;
};

#endif
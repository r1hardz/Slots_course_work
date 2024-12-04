#include "Game.h"
#include <iostream>
#include <iomanip>

Game::Game() 
{
    initializePayouts();
    initializePaylines();
}

void Game::initializePayouts() 
{
    symbolPayouts = {
        // format - symbol, {minimum matches, payout multiplier}
        {DIAMOND, {{2, 5.0},  {3, 25.0}, {4, 100.0}, {5, 500.0}}},  // Super rare 
        {SEVEN,   {{2, 3.0},  {3, 15.0}, {4, 75.0},  {5, 300.0}}},  // Very rare
        {BAR,     {{2, 0.1},  {3, 3.0},  {4, 15.0},  {5, 50.0}}},   
        {BELL,    {{2, 0.5},  {3, 2.0},  {4, 10.0},  {5, 30.0}}},   
        {PLUM,    {{2, 0.3},  {3, 1.0},  {4, 5.0},   {5, 15.0}}},   
        {ORANGE,  {{2, 0.2},  {3, 0.8},  {4, 4.0},   {5, 12.0}}},   
        {CHERRY,  {{2, 0.2},  {3, 0.6},  {4, 3.0},   {5, 10.0}}},   
        {LEMON,   {{2, 0.1},  {3, 0.5},  {4, 2.0},   {5, 8.0}}}     // Least valuable
    };
}

// winning line patterns
void Game::initializePaylines() 
{
    paylines = {
        {0, 1, 2, 3, 4},      // Horizontal top
        {5, 6, 7, 8, 9},      // Horizontal middle
        {10, 11, 12, 13, 14}, // Horizontal bottom
        {0, 6, 12, 8, 4},     // V shape
        {10, 6, 2, 8, 14},    // Inverted V shape
        {0, 6, 7, 8, 4},      // Zigzag top
        {10, 6, 7, 8, 14},    // Zigzag bottom
        {0, 1, 7, 3, 4},      // Wave top
        {10, 11, 7, 13, 14},  // Wave bottom
        {5, 1, 7, 13, 9}      // Diamond shape
    };
}

// convert symbol enum to display name
std::string Game::getSymbolName(Symbol symbol) const {
    switch (symbol) {
    case SEVEN:
        return "Seven";
    case LEMON:
        return "Lemon";
    case CHERRY:
        return "Cherry";
    case ORANGE:
        return "Orange";
    case PLUM:
        return "Plum";
    case BELL:
        return "Bell";
    case DIAMOND:
        return "Diamond";
    case BAR:
        return "Bar";
    default:
        return "Unknown";
    }
}

std::vector<std::vector<Symbol>> Game::spinReels() 
{
    // make 5x3 grid for results
    std::vector<std::vector<Symbol>> results(5, std::vector<Symbol>(3));
    
    // set up random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // set symbol probabilities (lower number = rarer)
    std::discrete_distribution<> dist({ 3, 15, 15, 15, 8, 15, 2, 8 });

    // filling grid with random symbols
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            results[i][j] = static_cast<Symbol>(dist(gen));
        }
    }
    return results;
}


double Game::calculatePayout(const std::vector<std::vector<Symbol>>& results, double betAmount) 
{
    double totalPayout = 0.0;

    // check each possible winning line
    for (size_t lineIndex = 0; lineIndex < paylines.size(); ++lineIndex) {
        const auto& payline = paylines[lineIndex];

        // get the first symbol in payline
        int firstPos = payline[0];
        int firstRow = firstPos / 5;
        int firstCol = firstPos % 5;
        Symbol currentSymbol = results[firstCol][firstRow];

        // count matching symbols from the left side
        int consecutiveMatches = 1;
        
        // check remaining positions in payline
        for (size_t i = 1; i < payline.size(); i++) {
            int pos = payline[i];
            int row = pos / 5;
            int col = pos % 5;
            Symbol nextSymbol = results[col][row];

            if (nextSymbol == currentSymbol) {
                consecutiveMatches++;
            }
            else {
                break;
            }
        }

        if (consecutiveMatches >= 2) {
            double highestPayout = 0.0;
            
            // find higest usable payout for this symbol count
            for (const PayoutInfo& payout : symbolPayouts[currentSymbol]) {
                if (consecutiveMatches >= payout.minCount) {
                    highestPayout = std::max(highestPayout, payout.payout);
                }
            }

            // Calculate and add line payout
            double linePayout = highestPayout * betAmount;
            totalPayout += linePayout;

            // show winning line info
            std::cout << "Payline " << (lineIndex + 1) << " won with "
                << consecutiveMatches << " " << getSymbolName(currentSymbol)
                << "(s), payout: " << std::fixed << std::setprecision(2)
                << linePayout << " euros (bet: " << betAmount << ")" << std::endl;
        }
    }

    if (totalPayout > 0.0) {
        std::cout << "Total win: " << std::fixed << std::setprecision(2)
            << totalPayout << " euros!" << std::endl;
    }

    return totalPayout;
}
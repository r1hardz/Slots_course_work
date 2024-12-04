#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <random>

enum Symbol { SEVEN, LEMON, CHERRY, ORANGE, PLUM, BELL, DIAMOND, BAR, NONE };

struct PayoutInfo {
    int minCount;      // the minimal number of symbols needed for a payout
    double payout;     // payout multiplier
};

class Game {
public:
    Game();
    std::vector<std::vector<Symbol>> spinReels();
    double calculatePayout(const std::vector<std::vector<Symbol>>& results, double betAmount);

    // getter for paylines
    const std::vector<std::vector<int>>& getPaylines() const { 
        return paylines; 
    }

private:
    // game data
    std::map<Symbol, std::vector<PayoutInfo>> symbolPayouts;
    std::vector<std::vector<int>> paylines;

    void initializePayouts();
    void initializePaylines();
    std::string getSymbolName(Symbol symbol) const;
};

#endif
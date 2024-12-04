#include <SFML/Graphics.hpp>
#include "Database.h"
#include "LoginSystem.h"
#include "SlotMachineWindow.h"

// full project available in  - https://github.com/r1hardz/Slots_course_work


int main() {
    // create window
    sf::RenderWindow window({ 1024, 768 }, "Slot Machine",
        sf::Style::Titlebar | sf::Style::Close);

    // center the window on screen
    window.setPosition({
        static_cast<int>((sf::VideoMode::getDesktopMode().width - 1024) / 2),
        static_cast<int>((sf::VideoMode::getDesktopMode().height - 768) / 2)
        });

    Database db("user_database.db");
    LoginSystem loginSystem(window, db);
    GameState currentState = GameState::LOGIN;

    // login window screen loop
    while (window.isOpen() && currentState != GameState::GAME) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }

            if (loginSystem.handleEvent(event, currentState)) {
                break;
            }
        }

        window.clear({ 50, 50, 50 });
        loginSystem.update();
        loginSystem.draw();
        window.display();
    }

    // change state to game if login successful
    if (currentState == GameState::GAME) {
        SlotMachineWindow slotMachine(window, db, loginSystem.getUsername());
        slotMachine.run();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include "Database.h"
#include "LoginSystem.h"
#include "SlotMachineWindow.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Titlebar | sf::Style::Close);
    window.setSize(sf::Vector2u(1024, 768));  // Ensure fixed size
    window.setPosition(sf::Vector2i(
        (sf::VideoMode::getDesktopMode().width - 1024) / 2,
        (sf::VideoMode::getDesktopMode().height - 768) / 2
    ));  // Center the window on screen

    Database db("user_database.db");
    LoginSystem loginSystem(window, db);
    GameState currentState = GameState::LOGIN;

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

        window.clear(sf::Color(50, 50, 50));
        loginSystem.update();
        loginSystem.draw();
        window.display();
    }

    if (currentState == GameState::GAME) {
        SlotMachineWindow slotMachine(window, db, loginSystem.getUsername());
        slotMachine.run();
    }

    return 0;
}
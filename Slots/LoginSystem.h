#ifndef LOGIN_SYSTEM_H
#define LOGIN_SYSTEM_H

#include <SFML/Graphics.hpp>
#include "Database.h"
#include <string>

enum class GameState {
	LOGIN,
	REGISTER,
	GAME
};

class LoginSystem {
public:
	LoginSystem(sf::RenderWindow& window, Database& database);

	// Input handling
	void handleTextInput(char input);
	bool handleEvent(sf::Event& event, GameState& currentState);
	void handleMouseMove(const sf::Vector2i& mousePos);
	void handleMouseClick(const sf::Vector2i& mousePos);

	// Field management
	void toggleField();
	void setPasswordField(bool value);
	bool isOnPasswordField() const {
		return isPasswordField; 
	}

	// User actions
	bool tryLogin();
	bool tryRegister();
	void clearInputs();

	// Getters
	const std::string& getUsername() const { 
		return inputUsername; 

	}

	std::string getDisplayPassword() const {
		return std::string(inputPassword.length(), '*'); 

	}

	// UI updates
	void draw();
	void update();

private:
	void initializeGUI();
	void updateButtonText();
	void updateInputTexts();
	void updateCursor();
	void centerText(sf::Text& text, const sf::RectangleShape& container);
	void setErrorMessage(const std::string& message, const sf::Color& color = sf::Color::Red);

	// Core components
	sf::RenderWindow& window;
	Database& db;
	std::string inputUsername;
	std::string inputPassword;
	bool isPasswordField;

	// GUI elements
	sf::Font font;
	sf::Text titleText;
	sf::Text errorText;
	sf::RectangleShape usernameBox;
	sf::RectangleShape passwordBox;
	sf::Text usernameText;
	sf::Text passwordText;
	sf::Text inputUsernameText;
	sf::Text inputPasswordText;
	sf::RectangleShape loginButton;
	sf::RectangleShape registerButton;
	sf::Text loginButtonText;
	sf::Text registerButtonText;
	sf::Text switchText;
	sf::RectangleShape cursor;

	// Animation and state
	float cursorBlinkTime;
	bool cursorVisible;
	sf::Clock clock;
	GameState currentState;

	// Constants
	const float CURSOR_BLINK_INTERVAL = 0.5f;
	const unsigned int MAX_INPUT_LENGTH = 20;
	const float BUTTON_WIDTH = 200.0f;
	const float BUTTON_HEIGHT = 50.0f;
	const float INPUT_BOX_WIDTH = 300.0f;
	const float INPUT_BOX_HEIGHT = 40.0f;
};

#endif
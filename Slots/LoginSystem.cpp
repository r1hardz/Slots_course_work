#include "LoginSystem.h"
#include <iostream>

LoginSystem::LoginSystem(sf::RenderWindow& win, Database& database) : 
    window(win),
    db(database),
    isPasswordField(false),
    cursorBlinkTime(0),
    cursorVisible(true),
    currentState(GameState::LOGIN) 
{
    initializeGUI();
}

void LoginSystem::initializeGUI() {
    if (!font.loadFromFile("Roboto-Black.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    // Title
    titleText.setFont(font);
    titleText.setString("Slot Machine");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition((window.getSize().x - titleText.getGlobalBounds().width) / 2, 50);

    // Error text
    errorText.setFont(font);
    errorText.setCharacterSize(24);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition((window.getSize().x - errorText.getLocalBounds().width) / 2, 120);

    // Username box
    usernameBox.setSize(sf::Vector2f(INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT));
    usernameBox.setFillColor(sf::Color(240, 240, 240));
    usernameBox.setOutlineThickness(2);
    usernameBox.setOutlineColor(sf::Color::Blue);
    usernameBox.setPosition((window.getSize().x - usernameBox.getSize().x) / 2, 200);

    // Password box
    passwordBox.setSize(sf::Vector2f(INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT));
    passwordBox.setFillColor(sf::Color(240, 240, 240));
    passwordBox.setOutlineThickness(2);
    passwordBox.setOutlineColor(sf::Color::Blue);
    passwordBox.setPosition((window.getSize().x - passwordBox.getSize().x) / 2, 295);

    // Username label
    usernameText.setFont(font);
    usernameText.setString("Username:");
    usernameText.setCharacterSize(24);
    usernameText.setFillColor(sf::Color::Yellow);
    usernameText.setPosition(
        usernameBox.getPosition().x - usernameText.getGlobalBounds().width - 20,
        usernameBox.getPosition().y + (usernameBox.getSize().y - usernameText.getGlobalBounds().height) / 2
    );

    // Password label
    passwordText.setFont(font);
    passwordText.setString("Password:");
    passwordText.setCharacterSize(24);
    passwordText.setFillColor(sf::Color::White);
    passwordText.setPosition(
        passwordBox.getPosition().x - passwordText.getGlobalBounds().width - 10,
        passwordBox.getPosition().y + (passwordBox.getSize().y - passwordText.getGlobalBounds().height) / 2
    );

    // Input texts
    inputUsernameText.setFont(font);
    inputUsernameText.setCharacterSize(20);
    inputUsernameText.setFillColor(sf::Color::Black);
    inputUsernameText.setPosition(usernameBox.getPosition().x + 10, usernameBox.getPosition().y + 8);

    inputPasswordText.setFont(font);
    inputPasswordText.setCharacterSize(20);
    inputPasswordText.setFillColor(sf::Color::Black);
    inputPasswordText.setPosition(passwordBox.getPosition().x + 10, passwordBox.getPosition().y + 8);

    // Login button
    loginButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    loginButton.setFillColor(sf::Color(100, 100, 200));
    loginButton.setPosition(
        (window.getSize().x - (loginButton.getSize().x * 2 + 20)) / 2,
        380
    );

    // Register button
    registerButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    registerButton.setFillColor(sf::Color(100, 200, 100));
    registerButton.setPosition(
        loginButton.getPosition().x + loginButton.getSize().x + 20,
        380
    );

    // Button texts
    loginButtonText.setFont(font);
    loginButtonText.setCharacterSize(22);
    loginButtonText.setFillColor(sf::Color::White);
    loginButtonText.setOutlineThickness(1);
    loginButtonText.setOutlineColor(sf::Color::Black);

    registerButtonText.setFont(font);
    registerButtonText.setCharacterSize(22);
    registerButtonText.setFillColor(sf::Color::White);
    registerButtonText.setOutlineThickness(1);
    registerButtonText.setOutlineColor(sf::Color::Black);

    // Switch text
    switchText.setFont(font);
    switchText.setCharacterSize(20);
    switchText.setFillColor(sf::Color::White);

    // Cursor
    cursor.setSize(sf::Vector2f(2, 30));
    cursor.setFillColor(sf::Color::Black);

    updateButtonText();
}

void LoginSystem::handleTextInput(char input) {

    // check if the input is a backspace or delete key
    if (input == '\b' || input == 127) {
        if (!isPasswordField && !inputUsername.empty()) {
            inputUsername.pop_back();
        }
        else if (isPasswordField && !inputPassword.empty()) {
            // remove the last character from the username
            inputPassword.pop_back();
        }
        return;
    }
    // check if the input is an invalid character (outside the printable ASCI range)
    if (input < 32 || input > 126) {
        return;
    }

    if (!isPasswordField && inputUsername.length() < MAX_INPUT_LENGTH) {
        inputUsername += input;
    }
    else if (isPasswordField && inputPassword.length() < MAX_INPUT_LENGTH) {
        inputPassword += input;
    }
}

void LoginSystem::toggleField() {
    isPasswordField = !isPasswordField;
}

void LoginSystem::setPasswordField(bool value) {
    isPasswordField = value;
}

bool LoginSystem::tryLogin() {
    if (db.loginUser(inputUsername, inputPassword)) {
        setErrorMessage("Login successful!", sf::Color::Green);
        return true;
    }
    setErrorMessage("Login failed - incorrect username or password");
    return false;
}

bool LoginSystem::tryRegister() {

    // minimum length for password and username is 3 characters
    if (inputUsername.length() < 3 || inputPassword.length() < 3) {
        setErrorMessage("Username and password must be at least 3 characters long!");
        return false;
    }

    if (db.addUser(inputUsername, inputPassword)) {
        setErrorMessage("Registration successful! Please login", sf::Color::Green);
        return true;
    }

    setErrorMessage("Registration failed - username might already exist");
    return false;
}

void LoginSystem::clearInputs() {
    inputUsername.clear();
    inputPassword.clear();
    isPasswordField = false;
}

void LoginSystem::updateButtonText() {

    // Check the current state of the login system, if so change the text
    if (currentState == GameState::REGISTER) {
        loginButtonText.setString("Back to Login");
        registerButtonText.setString("Register");
        switchText.setString("Have an account? Click Login");
    }
    else {
        loginButtonText.setString("Login");
        registerButtonText.setString("Go to Register");
        switchText.setString("Need an account? Click Register");
    }

    // center the login button text
    centerText(loginButtonText, loginButton);
    centerText(registerButtonText, registerButton);
    switchText.setPosition((window.getSize().x - switchText.getGlobalBounds().width) / 2, 450);
}

void LoginSystem::updateInputTexts() {
    inputUsernameText.setString(inputUsername);
    inputPasswordText.setString(getDisplayPassword());

}


// responsible for updating the position and visibility of the cursor in the login/registration UI.
void LoginSystem::updateCursor() {
    cursorBlinkTime += clock.restart().asSeconds();
    if (cursorBlinkTime >= CURSOR_BLINK_INTERVAL) {
        cursorVisible = !cursorVisible;
        cursorBlinkTime = 0;
    }

    float textX = !isPasswordField ? inputUsernameText.getPosition().x : inputPasswordText.getPosition().x;
    float textWidth = !isPasswordField ? inputUsernameText.getLocalBounds().width : inputPasswordText.getLocalBounds().width;
    float boxY = !isPasswordField ? usernameBox.getPosition().y : passwordBox.getPosition().y;

    cursor.setPosition(textX + textWidth, boxY + 5);
}

void LoginSystem::handleMouseMove(const sf::Vector2i& mousePos) {

    // check if the mouse position is within the global bounds of the login button
    loginButton.setFillColor(loginButton.getGlobalBounds().contains(mousePos.x, mousePos.y)
        // set the fill color to a lighter shade of blue
        ? sf::Color(120, 120, 240) : sf::Color(100, 100, 200));

    //same with register button
    registerButton.setFillColor(registerButton.getGlobalBounds().contains(mousePos.x, mousePos.y)
        ? sf::Color(120, 240, 120) : sf::Color(100, 200, 100));
}

void LoginSystem::handleMouseClick(const sf::Vector2i& mousePos) {

    // mouse click was within the bounds of the username input box
    if (usernameBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        // active input field to the username field
        setPasswordField(false);
    }
    // click was within the bounds of the password input box
    else if (passwordBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        // active input field to the password field
        setPasswordField(true);
    }
    //// iif the mouse click was within the bounds of the login button
    else if (loginButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        if (currentState == GameState::LOGIN) {
            // try to log the user in
            if (tryLogin()) {
                currentState = GameState::GAME;
            }
        }

        // if the current state is not LOGIN
        else {
            // change the state to LOGIN
            currentState = GameState::LOGIN;
            clearInputs(); 
            errorText.setString("");
        }
    }

    // click was within the bounds of the register button
    else if (registerButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        if (currentState == GameState::REGISTER) {
            if (tryRegister()) {
                //if the registration was successful, change the state to LOGIN
                currentState = GameState::LOGIN;
                clearInputs();
            }
        }
        else {
            currentState = GameState::REGISTER;
            clearInputs();
            errorText.setString("");
        }
    }
}

 //processing various user input events within the login and registration system.
bool LoginSystem::handleEvent(sf::Event& event, GameState& currentState) {
    if (event.type == sf::Event::TextEntered) {
        handleTextInput(static_cast<char>(event.text.unicode));
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Tab) {
            toggleField();
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            if (this->currentState == GameState::LOGIN) {
                if (tryLogin()) {
                    currentState = GameState::GAME;
                    return true;
                }
            }
            else if (this->currentState == GameState::REGISTER) {
                if (tryRegister()) {
                    this->currentState = GameState::LOGIN;
                    clearInputs();
                }
            }
        }
    }
    else if (event.type == sf::Event::MouseMoved) {
        handleMouseMove(sf::Mouse::getPosition(window));
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        handleMouseClick(sf::Mouse::getPosition(window));
        if (this->currentState == GameState::GAME) {
            currentState = GameState::GAME;
            return true;
        }
    }
    return false;
}

void LoginSystem::update() {
    updateInputTexts();
    updateCursor();

    usernameBox.setOutlineColor(!isPasswordField ? sf::Color::Yellow : sf::Color::Blue);
    passwordBox.setOutlineColor(isPasswordField ? sf::Color::Yellow : sf::Color::Blue);

    updateButtonText();
}

void LoginSystem::draw() {

    window.draw(titleText);
    window.draw(errorText);
    window.draw(usernameText);
    window.draw(passwordText);
    window.draw(usernameBox);
    window.draw(passwordBox);
    window.draw(inputUsernameText);
    window.draw(inputPasswordText);
    window.draw(loginButton);
    window.draw(registerButton);
    window.draw(loginButtonText);
    window.draw(registerButtonText);
    window.draw(switchText);

    if (cursorVisible) {
        window.draw(cursor);
    }
}

// positions the provided text element centered within the provided container
void LoginSystem::centerText(sf::Text& text, const sf::RectangleShape& container) {
    text.setPosition(
        container.getPosition().x + (container.getSize().x - text.getLocalBounds().width) / 2,
        container.getPosition().y + (container.getSize().y - text.getLocalBounds().height) / 2
    );
}
// sets the text, color, and centered horizontal position of the error text element
void LoginSystem::setErrorMessage(const std::string& message, const sf::Color& color) {
    errorText.setString(message);
    errorText.setFillColor(color);
    errorText.setPosition((window.getSize().x - errorText.getLocalBounds().width) / 2, 120);
}
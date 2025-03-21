//

#include <iostream>
#include <fstream>
#include "game.h"
#include "States/gameState.h"

//
// Created by Logan on 3/11/2025.
void game::initWindow() {

    std::ifstream ifs("config/window.ini");

    std::string title = "None";
    sf::VideoMode window_bounds({720,380});
    unsigned framerate_limit = 120;
    bool vertical_sync_enabled = false;

    if(ifs.is_open())
    {
        std::getline(ifs,title);
        ifs >> window_bounds.size.x >> window_bounds.size.y;
        ifs >> framerate_limit;
        ifs >> vertical_sync_enabled;
    }
    else {
        std::cout << "Failed to open config/window.ini" << "\n";
    }
    ifs.close();

    this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
    this->window->setFramerateLimit(60);
    this->window->setVerticalSyncEnabled(false);
}

void game::initStates()
{
    this->states.push(new gameState(this->window));
}

// Constructors
game::game() {
    this->initWindow();
    this->initStates();
}

game::~game() {
    delete this->window;

    while(!this->states.empty())
    {
        delete this->states.top();
        this->states.pop();
    }
}

void game::updateSFMLEvents()
{
    this->sfEvent = this->window->pollEvent();
    while(const auto event = this->window->pollEvent())
    {
        this->sfEvent = event;
        if(this->sfEvent->getIf<sf::Event::Closed>()) {
            this->window->close();
        }
        else if(const auto* mousePressed = this->sfEvent->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                // Left Click!
            } else if (mousePressed->button == sf::Mouse::Button::Right) {
                // Right Click!
            } else if (mousePressed->button == sf::Mouse::Button::Middle) {
                // Middle Click!
            }
        }
        else if(const auto* keyPressed = this->sfEvent->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
                // W!
            } else if (keyPressed->scancode == sf::Keyboard::Scancode::A) {
                // A!
            } else if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
                // S!
            } else if (keyPressed->scancode == sf::Keyboard::Scancode::D) {
                // D!
            }
        }
        else if(const auto* mouseMoved = this->sfEvent->getIf<sf::Event::MouseMoved>())
        {
            sf::Vector2<float> pos(mouseMoved->position.x, mouseMoved->position.y);
//            std::cout<< "Mouse Pos: " + std::to_string(mouseMoved->position.x) + " " + std::to_string(mouseMoved->position.y) +"\n";
//            mousePos = pos;
//            for (const sf::CircleShape &entity: entities) {
//                sf::FloatRect boundingBox = entity.getGlobalBounds();
//                std::cout << std::to_string(boundingBox.position.x) + " " + std::to_string(boundingBox.position.y) + "\n";
//
//                if (boundingBox.contains(pos)) {
//                    std::cout << "Overlap Detected!";
//                }
//            }
        }
        else if(const auto* onFocusLost = this->sfEvent->getIf<sf::Event::FocusLost>())
        {
            window->setFramerateLimit(10);
        }
        else if(const auto* onFocusGained = this->sfEvent->getIf<sf::Event::FocusLost>())
        {
            window->setFramerateLimit(60);
        }
    }
}

void game::update() {
    this->updateSFMLEvents();

    if(!this->states.empty()) {
        this->states.top()->update(this->dt);
    }
}

void game::render() {
    this->window->clear();

    // draw
    this->window->draw(clockText);

    if(!this->states.empty()) {
        this->states.top()->render(this->window);
    }

    this->window->display();
}

void game::run() {
    this->window->setFramerateLimit(60);

    while(this->window->isOpen()) {
        this->updateDt();
        this->update();
        this->render();
    }

    clock_t current_ticks, delta_ticks;
    clock_t fps = 0;

    sf::Font font("pixel-arial-14.otf"); // Throws sf::Exception if an error occurs

    sf::Text fps_text(font); // a font is required to make a text object
    fps_text.setCharacterSize(24); // in pixels, not points!
    fps_text.setFillColor(sf::Color::Red);
    fps_text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    clockText = sf::Text(font); // a font is required to make a text object
    clockText.setCharacterSize(24); // in pixels, not points!
    clockText.setFillColor(sf::Color::White);
    clockText.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::CircleShape circle(50.f);
    circle.setFillColor(sf::Color(100, 250, 50));
    circle.setPosition({250.f, 250.f});
//    entities.push_back(circle);

    sf::RectangleShape boundBox(circle.getGlobalBounds().size);
    boundBox.setPosition(circle.getGlobalBounds().position);
    boundBox.setFillColor(sf::Color::Transparent);
    boundBox.setOutlineColor(sf::Color::Red);
    boundBox.setOutlineThickness(2.f);

    sf::View view(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));

// activate it
//        view.zoom(0.01f);
    view.zoom(0.5f);
    window->setView(view);

    while (window->isOpen()) {

//        while (const std::optional event = window.pollEvent())
//        {
////            if (event->is<sf::Event::Closed>())
////            {
////                window.close();
////            }
//        }


        window->clear();
// let's define a view


        window->draw(fps_text);
        window->draw(circle);
        window->draw(boundBox);

        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; ++j) {
                sf::RectangleShape box({32, 32});
                box.setPosition({i * 32.f, j * 32.f});
                box.setFillColor(sf::Color::Black);
                box.setOutlineColor(sf::Color::White);
                box.setOutlineThickness(2.f);

                sf::Text number(font);
                number.setCharacterSize(12); // in pixels, not points!
                number.setString(std::to_string(i) + ", " + std::to_string(j));
                number.setPosition({i * 32.f, j * 32.f});

                window->draw(box);
                window->draw(number);
            }
        }

        window->display();

        delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
        if (delta_ticks > 0)
            fps = CLOCKS_PER_SEC / delta_ticks;
//        cout << fps << endl;
    }
}

void game::updateDt()
{
    this->dt = this->dtClock.getElapsedTime().asSeconds();
    system("cls");
    std::cout << this->dt << "\n";

    this->dtClock.restart();
}

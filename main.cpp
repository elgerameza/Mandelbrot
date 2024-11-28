#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"
#include <iostream>

int main() {
    unsigned int screenWidth = sf::VideoMode::getDesktopMode().width / 2;
    unsigned int screenHeight = sf::VideoMode::getDesktopMode().height / 2;

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Mandelbrot Set Viewer");

    
    ComplexPlane ComplexPlane(screenWidth, screenHeight);

    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }
    sf::Text text("", font, 16);
    text.setFillColor(sf::Color::White);

    // Main loop
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            // Handle window close event
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    ComplexPlane.zoomOut();
                    ComplexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                }
                else if (event.mouseButton.button == sf::Mouse::Left) {
                    ComplexPlane.zoomIn();
                    ComplexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                ComplexPlane.setMouseLocation({ event.mouseMove.x, event.mouseMove.y });
             }
        }

        // Check if the Escape key is pressed to close the window
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        // Update the scene
        ComplexPlane.updateRender();
        ComplexPlane.loadText(text);

        // Draw the scene
        window.clear();
        window.draw(ComplexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}

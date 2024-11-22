#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    int choix = 0;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Déplacer une Forme");

    // Création d'un cercle
    sf::CircleShape circle(50.f); // Rayon de 50 pixels
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition(100.f, 300.f); // Position initiale

    sf::CircleShape circle2(50.f); // Rayon de 50 pixels
    circle2.setFillColor(sf::Color::Green);
    circle2.setPosition(0.f, 0.f); // Position initiale

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) { choix = 1; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) { choix = 2; }
            else { choix = 0; }

            if (choix == 1) { std::cout << "bouger bleu" << std::endl; }

            if (choix == 2) { 0; std::cout << "bouger vert" << std::endl; }
        }

        // Vérification des touches
        if (choix == 1) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                circle.move(0.01f, 0.f); // Déplacer vers la droite
            }
        }
        if (choix == 2) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                circle2.move(0.01f, 0.f); // Déplacer vers la droite
            }
        }

        window.clear();
        window.draw(circle);
        window.draw(circle2);
        window.display();
    }

    return 0;
}
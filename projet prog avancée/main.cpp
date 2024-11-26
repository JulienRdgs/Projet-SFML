
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>


// Dimensions de la fenêtre
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Dimensions des raquettes
const float PADDLE_WIDTH = 10.f;
const float PADDLE_HEIGHT = 100.f;

// Rayon de la balle
const float BALL_RADIUS = 10.f;

// Vitesse de la balle
sf::Vector2f ballVelocity(3.0f, 3.0f);

const float POS_LIGNE = WINDOW_WIDTH * 0.15;

bool styloB = false;
bool pinceauB = false;

void activeButton(sf::RectangleShape button, sf::Mouse mouse, sf::RenderWindow& window, bool& selec) {
    if (button.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window))) && selec == false) {
        std::cout << "dessiner" << std::endl;
        selec = true;
    }
    else if (button.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window))) && selec == true) {
        std::cout << "ne plus dessiner" << std::endl;
        selec = false;
    }
}
int main() {

    sf::Image image;
    image.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::White);
    sf::Texture textImage;
    textImage.loadFromImage(image);
    sf::Sprite spriteImage;
    spriteImage.setTexture(textImage);

    std::vector<sf::CircleShape> dessin;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { std::cout << "erreur" << std::endl; }

    // Créer la fenêtre
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Paint SFML JRdgs");
    window.setFramerateLimit(60);

    sf::RectangleShape stylo(sf::Vector2f(50, 50));
    stylo.setPosition(0, 0);
    stylo.setFillColor(sf::Color::Red);

    sf::RectangleShape pinceau(sf::Vector2f(50, 50));
    pinceau.setPosition(100, 0);
    pinceau.setFillColor(sf::Color::Green);

    sf::VertexArray ligne(sf::Lines);
    ligne.append(sf::Vertex(sf::Vector2f(0, POS_LIGNE), sf::Color::Black));
    ligne.append(sf::Vertex(sf::Vector2f(WINDOW_WIDTH, POS_LIGNE), sf::Color::Black));


    while (window.isOpen()) {

        textImage.loadFromImage(image);
        spriteImage.setTexture(textImage);
        // Gérer les événements
        sf::Event event;
        sf::Mouse mouse;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) // Handling the closure of the renderwindow
                if (event.key.code == sf::Keyboard::Key::Escape)
                    window.close();
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //if (event.type == sf::Event::MouseMoved) 
            // std::cout << "X : " << mouse.getPosition(window).x << "  Y : " << mouse.getPosition(window).y << std::endl;

            if (event.type == sf::Event::MouseButtonReleased) {

                if (event.mouseButton.button == sf::Mouse::Left) {
                    activeButton(stylo, mouse, window, styloB); //répeter pour chaque bouton
                    if (styloB) pinceauB = false; // pas réussi a assembler les booléens des boutons dans un vecteur.
                    activeButton(pinceau, mouse, window, pinceauB);
                    if (pinceauB) styloB = false;
                }
            }
        }

        if (styloB == true && mouse.getPosition(window).y > POS_LIGNE) { // pour ne pas dessiner sur le menu
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                image.setPixel(mouse.getPosition(window).x, mouse.getPosition(window).y, sf::Color::Black);
            }
        }
        if (pinceauB == true && mouse.getPosition(window).y > POS_LIGNE) { // pour ne pas dessiner sur le menu
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::CircleShape shape = sf::CircleShape(5);
                shape.setPosition(mouse.getPosition(window).x, mouse.getPosition(window).y);
                shape.setFillColor(sf::Color::Green);
                dessin.push_back(shape);
                window.draw(shape);
            }
        }

        // Dessiner les éléments
        window.clear(sf::Color::White);
        window.draw(spriteImage);
        window.draw(stylo);
        window.draw(pinceau);
        window.draw(ligne);
        for (sf::CircleShape shape : dessin) {
            window.draw(shape);
        }

        window.display();

    }

    return 0;
}
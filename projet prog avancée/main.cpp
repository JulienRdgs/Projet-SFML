
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

const float BUTTON_SIZE = 30.f;

const float SET_ROW = 10.f;

const float POS_LIGNE = WINDOW_WIDTH * 0.15;

const float BUTTON_GAP = 15.f + BUTTON_SIZE;

const float OUTLINE_THICK = 1.0f;

sf::Color setColor = sf::Color::Black;

bool styloB = false;
bool pinceauB = false;
bool rougeB = false;
bool roseB = false;

void activeButton(sf::RectangleShape button, sf::Mouse mouse, sf::RenderWindow& window, bool& selec) {
    if (button.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window))) && selec == false) {
        std::cout << "dessiner" << std::endl;
        selec = true;
    }
    else if (button.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window))) && selec == true) {
        std::cout << "ne plus dessiner" << std::endl;
        selec = false;
        setColor = sf::Color::Black;
    }
}

void defaultParam(sf::RectangleShape &colorButton) { // actuellent ça ne save qu'une ligne, mais jsp si j'ajouterai des bails aux boutons
    colorButton.setOutlineThickness(OUTLINE_THICK);
    colorButton.setOutlineColor(sf::Color::Black);
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
    if (!font.loadFromFile("asset/arial.ttf")) { std::cout << "erreur chargement arial" << std::endl; }

    // Créer la fenêtre
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Paint SFML JRdgs");
    window.setFramerateLimit(60);

    sf::RectangleShape stylo(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    stylo.setPosition(SET_ROW, SET_ROW);
    stylo.setFillColor(sf::Color::White);
    stylo.setOutlineThickness(OUTLINE_THICK);
    stylo.setOutlineColor(sf::Color::Black);
    sf::Texture iconeStylo;
    if (!iconeStylo.loadFromFile("asset/icone stylo.png")) { std::cout << "erreur chargement icone stylo" << std::endl; }
    stylo.setTexture(&iconeStylo);

    sf::RectangleShape pinceau(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    pinceau.setPosition(SET_ROW + BUTTON_GAP, SET_ROW);
    pinceau.setFillColor(sf::Color::White);
    pinceau.setOutlineThickness(OUTLINE_THICK);
    pinceau.setOutlineColor(sf::Color::Black);
    sf::Texture iconePinceau;
    if (!iconePinceau.loadFromFile("asset/icone pinceau.png")) { std::cout << "erreur chargmenent icone pinceau" << std::endl; }
    pinceau.setTexture(&iconePinceau);

    sf::RectangleShape rouge(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    rouge.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE, SET_ROW);
    rouge.setFillColor(sf::Color::Red);
    defaultParam(rouge);
    /*rouge.setOutlineThickness(OUTLINE_THICK);
    rouge.setOutlineColor(sf::Color::Black);*/ //ces deux lignes sont faites dans le defaultParam()

    sf::RectangleShape bleu(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    bleu.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP, SET_ROW);
    bleu.setFillColor(sf::Color::Blue);
    defaultParam(bleu);

    sf::RectangleShape vert(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    vert.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 2, SET_ROW);
    vert.setFillColor(sf::Color::Green);
    defaultParam(vert);

    sf::RectangleShape rose(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    rose.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 3, SET_ROW);
    rose.setFillColor(sf::Color(255,0,255));
    defaultParam(rose);

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
                    activeButton(rouge, mouse, window, rougeB);
                    if (rougeB) {
                        roseB == false;
                        setColor = sf::Color::Red;
                    } //faire une fonction pour éviter de taper le meme activeButton pour chaque couleur ? ou jlaisse genre 4 couleur pour 4 copiés-collés ?
                    activeButton(rose, mouse, window, roseB);
                    if (roseB) { 
                        rougeB == false;
                        setColor = sf::Color(255,0,255); //jsp pk jpeux pas switch du rose au rouge directement.
                    }
                }
            }
        }

        if (styloB == true && mouse.getPosition(window).y > POS_LIGNE) { // pour ne pas dessiner sur le menu
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                image.setPixel(mouse.getPosition(window).x, mouse.getPosition(window).y, setColor);
            }
        }
        if (pinceauB == true && mouse.getPosition(window).y > POS_LIGNE) { // pour ne pas dessiner sur le menu
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::CircleShape shape = sf::CircleShape(5);
                shape.setPosition(mouse.getPosition(window).x, mouse.getPosition(window).y);
                shape.setFillColor(setColor);
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
        window.draw(rouge);
        window.draw(bleu);
        window.draw(vert);
        window.draw(rose);
        for (sf::CircleShape shape : dessin) {
            window.draw(shape);
        }

        window.display();

    }

    return 0;
}
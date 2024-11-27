
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

std::vector<bool> buttonState;
bool styloB = false;
bool pinceauB = false;
bool rougeB = false;
bool roseB = false;

void activeButton(sf::RectangleShape button, sf::Mouse mouse, sf::RenderWindow& window, bool& selec) {
    if (button.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
        std::cout << "dessiner" << std::endl;
        selec = true;
        for (bool bouton : buttonState) {
            if (bouton != selec) {
                bouton = false; //désactiver tous les autres boutons si après avoir cliqué sur un bouton
            }
        }
        // ALTERNATIVE : modifier manuellement tous les autres états dans un if(etat=true) après chaque activeButton

        for (bool bouton : buttonState) {
            std::cout << bouton << std::endl;
        }
        if (button.getFillColor() != sf::Color::White) { //si le bouton est une couleur, alors la couleur actuelle devient celle-ci
            setColor = button.getFillColor(); 
        }
    }
}

void defaultParam(sf::RectangleShape &colorButton) { // actuellent ça ne save qu'une ligne, mais jsp si j'ajouterai des bails aux boutons
    colorButton.setOutlineThickness(OUTLINE_THICK);
    colorButton.setOutlineColor(sf::Color::Black);
}

std::vector<sf::CircleShape> dessin;
std::vector<sf::RectangleShape> boutons;

int main() {

    buttonState.push_back(styloB);
    buttonState.push_back(pinceauB);
    buttonState.push_back(rougeB);
    buttonState.push_back(roseB);

    sf::Image image;
    image.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::White);
    sf::Texture textImage;
    textImage.loadFromImage(image);
    sf::Sprite spriteImage;
    spriteImage.setTexture(textImage);

    

    sf::Font font;
    if (!font.loadFromFile("asset/arial.ttf")) { std::cout << "erreur chargement arial" << std::endl; }

    // Créer la fenêtre
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Paint SFML JRdgs");
    window.setFramerateLimit(60);

    sf::RectangleShape souris(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    souris.setPosition(SET_ROW, SET_ROW);
    souris.setFillColor(sf::Color::White);
    defaultParam(souris);
    sf::Texture iconeSouris;
    if (!iconeSouris.loadFromFile("asset/icone souris.png")) { std::cout << "erreur chargement icone souris" << std::endl; }
    souris.setTexture(&iconeSouris);
    boutons.push_back(souris);

    sf::RectangleShape stylo(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    stylo.setPosition(SET_ROW + BUTTON_GAP, SET_ROW);
    stylo.setFillColor(sf::Color::White);
    defaultParam(stylo);
    sf::Texture iconeStylo;
    if (!iconeStylo.loadFromFile("asset/icone stylo.png")) { std::cout << "erreur chargement icone stylo" << std::endl; }
    stylo.setTexture(&iconeStylo);
    boutons.push_back(stylo);

    sf::RectangleShape pinceau(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    pinceau.setPosition(SET_ROW + BUTTON_GAP*2, SET_ROW);
    pinceau.setFillColor(sf::Color::White);
    defaultParam(pinceau);
    sf::Texture iconePinceau;
    if (!iconePinceau.loadFromFile("asset/icone pinceau.png")) { std::cout << "erreur chargmenent icone pinceau" << std::endl; }
    pinceau.setTexture(&iconePinceau);
    boutons.push_back(pinceau);

    sf::RectangleShape rouge(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    rouge.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE, SET_ROW);
    rouge.setFillColor(sf::Color::Red);
    defaultParam(rouge);
    boutons.push_back(rouge);

    sf::RectangleShape bleu(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    bleu.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP, SET_ROW);
    bleu.setFillColor(sf::Color::Blue);
    defaultParam(bleu);
    boutons.push_back(bleu);

    sf::RectangleShape vert(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    vert.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 2, SET_ROW);
    vert.setFillColor(sf::Color::Green);
    defaultParam(vert);
    boutons.push_back(vert);

    sf::RectangleShape rose(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    rose.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 3, SET_ROW);
    rose.setFillColor(sf::Color(255,0,255));
    defaultParam(rose);
    boutons.push_back(rose);

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
                    activeButton(stylo, mouse, window, styloB);
                    activeButton(pinceau, mouse, window, pinceauB);
                    activeButton(rouge, mouse, window, rougeB);
                    activeButton(rose, mouse, window, roseB);
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
        window.draw(spriteImage);//l'image pour le setPixel du stylo, a potentiellement changer
        window.draw(ligne); //la ligne qui sépare les boutons de l'espace de dessin
        for (sf::RectangleShape &bouton : boutons) {
            window.draw(bouton);//tous les boutons
        }
        for (sf::CircleShape &shape : dessin) {
            window.draw(shape);//les cercles créés par le pinceau, a potentiellement changer
        }

        window.display();

    }

    return 0;
}
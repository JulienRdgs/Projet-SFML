
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float BUTTON_SIZE = 30.f;

const float SET_ROW = 10.f;

const float POS_LIGNE = WINDOW_WIDTH * 0.15;

const float BUTTON_GAP = 15.f + BUTTON_SIZE;

const float OUTLINE_THICK = 1.0f;

//COULEUR ACTUELLE
sf::Color setColor = sf::Color::Black;

//VECTOR DES ETATS DES BOUTONS, ACTUELLEMENT INUTILE
std::vector<bool> buttonState;
//ETATS DES BOUTONS
bool sourisB = false;
bool styloB = false;
bool pinceauB = false;
bool noirB = false;
bool rougeB = false;
bool bleuB = false;
bool vertB = false;
bool roseB = false;

//DETECTION D'ACTIVATION DE BOUTON
void activeButton(sf::RectangleShape button, sf::Mouse mouse, sf::RenderWindow& window, bool& selec) {
    if (button.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
        std::cout << "dessiner" << std::endl;
        selec = true;
        //for (bool bouton : buttonState) {
        //    if (bouton != selec) {
        //        bouton = false; //désactiver tous les autres boutons si après avoir cliqué sur un bouton
        //    }
        //}
        // ALTERNATIVE ACTUELLE : modifier manuellement tous les autres états dans un if(etat=true) après chaque activeButton

        /*for (bool bouton : buttonState) {
            std::cout << bouton << std::endl; //debug pour vérif les états
        }*/
        if (button.getFillColor() != sf::Color::White) { //si le bouton est une couleur, alors la couleur actuelle devient celle-ci
            setColor = button.getFillColor();
        }
    }
} //ajouter une condition pour éviter de changer de couleur quand la souris est activée ? faudrait faire une deuxième boucle copiée-collée ici


void defaultParam(sf::RectangleShape& colorButton) { // actuellent ça ne save qu'une ligne, mais jsp si j'ajouterai des bails aux boutons
    colorButton.setOutlineThickness(OUTLINE_THICK);
    colorButton.setOutlineColor(sf::Color::Black);
}

std::vector<sf::CircleShape> dessin;
std::vector<sf::RectangleShape> boutons;

int main() {

    //PUSH_BACK DES ETATS DES BOUTONS, ACTUELLEMENT INUTILE
    buttonState.push_back(sourisB);
    buttonState.push_back(styloB);
    buttonState.push_back(pinceauB);
    buttonState.push_back(noirB);
    buttonState.push_back(rougeB);
    buttonState.push_back(bleuB);
    buttonState.push_back(vertB);
    buttonState.push_back(roseB);

    //IMAGE POUR LE BACKGROUND DU MENU
    sf::Image image;
    image.create(WINDOW_WIDTH, POS_LIGNE, sf::Color(222,222,222));
    sf::Texture textImage;
    textImage.loadFromImage(image);
    sf::Sprite spriteImage;
    spriteImage.setTexture(textImage);

    //AU CAS OU
    sf::Font font;
    if (!font.loadFromFile("asset/arial.ttf")) { std::cout << "erreur chargement arial" << std::endl; }

                        // CREER FENETRE
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Paint SFML JRdgs");
    window.setFramerateLimit(60);

                        //SOURIS
    sf::RectangleShape souris(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    souris.setPosition(SET_ROW, SET_ROW);
    souris.setFillColor(sf::Color::White);
    defaultParam(souris);
    sf::Texture iconeSouris;
    if (!iconeSouris.loadFromFile("asset/icone souris.png")) { std::cout << "erreur chargement icone souris" << std::endl; }
    souris.setTexture(&iconeSouris);
    boutons.push_back(souris);

                        //STYLO
    sf::RectangleShape stylo(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    stylo.setPosition(SET_ROW + BUTTON_GAP, SET_ROW);
    stylo.setFillColor(sf::Color::White);
    defaultParam(stylo);
    sf::Texture iconeStylo;
    if (!iconeStylo.loadFromFile("asset/icone stylo.png")) { std::cout << "erreur chargement icone stylo" << std::endl; }
    stylo.setTexture(&iconeStylo);
    boutons.push_back(stylo);

                        //PINCEAU
    sf::RectangleShape pinceau(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    pinceau.setPosition(SET_ROW + BUTTON_GAP * 2, SET_ROW);
    pinceau.setFillColor(sf::Color::White);
    defaultParam(pinceau);
    sf::Texture iconePinceau;
    if (!iconePinceau.loadFromFile("asset/icone pinceau.png")) { std::cout << "erreur chargmenent icone pinceau" << std::endl; }
    pinceau.setTexture(&iconePinceau);
    boutons.push_back(pinceau);

                        //TAILLE PINCEAU
    //DIMINUER
    unsigned int currentSizePinceau = 10;
    sf::RectangleShape diminuerTaille(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    //diminuerTaille.setPosition(400, 400);
    diminuerTaille.setPosition(SET_ROW + BUTTON_GAP * 3, SET_ROW);
    diminuerTaille.setFillColor(sf::Color::White);
    defaultParam(diminuerTaille);
    sf::Texture flecheBas;
    if (!flecheBas.loadFromFile("asset/icone down.png")) { std::cout << "erreur chargmenent icone pinceau" << std::endl; }
    diminuerTaille.setTexture(&flecheBas);
    boutons.push_back(diminuerTaille);

    //AUGMENTER
    sf::RectangleShape augmenterTaille(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    //augmenterTaille.setPosition(500, 400);
    augmenterTaille.setPosition(SET_ROW + BUTTON_GAP * 4, SET_ROW);
    augmenterTaille.setFillColor(sf::Color::White);
    defaultParam(augmenterTaille);
    sf::Texture flecheHaut;
    if (!flecheHaut.loadFromFile("asset/icone up.png")) { std::cout << "erreur chargmenent icone pinceau" << std::endl; }
    augmenterTaille.setTexture(&flecheHaut);
    boutons.push_back(augmenterTaille);

                        //COULEURS
    //NOIR
    sf::RectangleShape noir(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    noir.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE, SET_ROW);
    noir.setFillColor(sf::Color::Black);
    defaultParam(noir);
    boutons.push_back(noir);

    //ROUGE
    sf::RectangleShape rouge(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    rouge.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP, SET_ROW);
    rouge.setFillColor(sf::Color::Red);
    defaultParam(rouge);
    boutons.push_back(rouge);

    //BLEU
    sf::RectangleShape bleu(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    bleu.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 2, SET_ROW);
    bleu.setFillColor(sf::Color::Blue);
    defaultParam(bleu);
    boutons.push_back(bleu);

    //VERT
    sf::RectangleShape vert(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    vert.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 3, SET_ROW);
    vert.setFillColor(sf::Color::Green);
    defaultParam(vert);
    boutons.push_back(vert);

    //ROSE
    sf::RectangleShape rose(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    rose.setPosition(WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 4, SET_ROW);
    rose.setFillColor(sf::Color(255, 0, 255));
    defaultParam(rose);
    boutons.push_back(rose);

    //LIGNE SEPARATRICE
    sf::VertexArray ligne(sf::Lines);
    ligne.append(sf::Vertex(sf::Vector2f(0, POS_LIGNE), sf::Color::Black));
    ligne.append(sf::Vertex(sf::Vector2f(WINDOW_WIDTH, POS_LIGNE), sf::Color::Black));


    while (window.isOpen()) {

        textImage.loadFromImage(image);
        spriteImage.setTexture(textImage);
        
        sf::Event event;
        sf::Mouse mouse;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Key::Escape)
                    window.close();
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //if (event.type == sf::Event::MouseMoved) 
            // std::cout << "X : " << mouse.getPosition(window).x << "  Y : " << mouse.getPosition(window).y << std::endl;

            if (event.type == sf::Event::MouseButtonReleased) {

                if (event.mouseButton.button == sf::Mouse::Left) {

                    // DESSINER/SOURIS
                    activeButton(souris, mouse, window, sourisB);
                    if (sourisB) { styloB = false; pinceauB = false; }

                    activeButton(stylo, mouse, window, styloB);
                    if (styloB) { pinceauB = false; sourisB = false; }

                    activeButton(pinceau, mouse, window, pinceauB);
                    if (pinceauB) { styloB = false; sourisB = false; }

                    //TAILLE PINCEAU
                    if (diminuerTaille.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
                        std::cout << "taille pinceau : diminuer taille" << std::endl;
                        if (currentSizePinceau <= 3) { currentSizePinceau = 1; }
                        else { currentSizePinceau -= 3; }
                    }
                    if (augmenterTaille.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
                        std::cout << "taille pinceau : augmenter taille" << std::endl;
                        if (currentSizePinceau >= 37) { currentSizePinceau = 40; }
                        else { currentSizePinceau += 3; }
                    }

                    // COULEURS
                    activeButton(noir, mouse, window, noirB);
                    if (noirB) { rougeB = false;  roseB = false; }

                    activeButton(rouge, mouse, window, rougeB);
                    if (rougeB) { roseB = false; noirB = false; }

                    activeButton(bleu, mouse, window, bleuB);
                    if (bleuB) { rougeB = false; noirB = false; }

                    activeButton(vert, mouse, window, vertB);
                    if (vertB) { rougeB = false; noirB = false; }

                    activeButton(rose, mouse, window, roseB);
                    if (roseB) { rougeB = false; noirB = false; }

                    /*for (sf::RectangleShape bouton : boutons) {
                        activeButton(bouton,mouse,window,bouton.etat) //j'aurai pu faire un bail comme ça si les bools des boutons étaient dans leur classe
                    }*/
                }
            }
        }

        if (styloB == true && mouse.getPosition(window).y >= POS_LIGNE-currentSizePinceau) { // pour ne pas dessiner sur le menu
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                //image.setPixel(mouse.getPosition(window).x, mouse.getPosition(window).y, setColor);  anciennement pour setPixel sur une image
                sf::CircleShape shape = sf::CircleShape(0.5);
                shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius()); //centrer rond sur souris
                shape.setFillColor(setColor);
                dessin.push_back(shape);
                window.draw(shape);
            }
        }
        if (pinceauB == true) {
            if (mouse.getPosition(window).y >= POS_LIGNE-currentSizePinceau) { // pour ne pas dessiner sur le menu
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::CircleShape shape = sf::CircleShape(currentSizePinceau);
                    shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius());
                    shape.setFillColor(setColor);
                    dessin.push_back(shape);
                    window.draw(shape);
                }
            }
        }
        /*else {
            diminuerTaille.setPosition(400, 400);
            augmenterTaille.setPosition(500 , 400);
        }*/

        //DESSINER
        window.clear(sf::Color::White);
        for (sf::CircleShape& shape : dessin) {
            window.draw(shape);//les cercles créés par le pinceau, a potentiellement changer
        }
        window.draw(spriteImage);
        window.draw(ligne); //la ligne qui sépare les boutons de l'espace de dessin
        for (sf::RectangleShape& bouton : boutons) {
            window.draw(bouton);//tous les boutons
        }
        std::cout << currentSizePinceau << std::endl;
        window.display();

    }

    return 0;
}
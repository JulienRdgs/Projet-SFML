#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp" //entre guillemets pour automatiquement include les fichiers sfml
#include <iostream>
#include <vector>

                        //CONST
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float BUTTON_SIZE = 30.f;
const float SET_ROW = 10.f; //écart entre les lignes
const float POS_LIGNE = BUTTON_SIZE * 2 + SET_ROW * 3; // le menu a la place pour 2 lignes d'icones
const float BUTTON_GAP = SET_ROW + BUTTON_SIZE; //pour éviter les calculs trop longs
const float OUTLINE_THICK = 1.0f;
const float DEFAULT_RECT_SIZE = 125.f;
const float DEFAULT_CIRCLE_RADIUS = 50.f;
const float DEFAULT_TRIANGLE_SIZE = 50.f;

                        //VARIABLES
unsigned int currentSizePinceau = 10;
//bool ecrireActif = false;
bool lock = false;
sf::Color shapeColor = sf::Color::White;
int selecShapeCount = 0;
bool isDrawing = false;
//std::string cinUserText;
sf::Color currentColor = sf::Color::Black;
sf::Color backgroundChoice = sf::Color::White;

class Drawings; //pour pouvoir déclarer vectorDrawing

                        //VECTEURS
std::vector<Drawings> vectorDrawing; //les ronds qui servent de dessins
std::vector<sf::RectangleShape> vectorRectangle; //les rectangles placés par l'utilisateur
std::vector<sf::CircleShape> vectorRond; //les ronds placés par l'utilisateur
std::vector<sf::ConvexShape> vectorTriangle; //les triangles placés par l'utilisateur
//std::vector<allButtons> vectorButtons; //plusieurs problèmes avec

                        //CLASSE DES RONDS DESSINES
class Drawings : public sf::CircleShape {
public:
    bool gommeState = false; //pour différencier les cercles qui effacent de ceux qui dessinent
};

                        //CLASSE DES BOUTONS
class allButtons : public sf::RectangleShape {
public:

    bool buttonState = false;
    std::string id;

    allButtons(std::string id, float x, float y, sf::Color couleur) : id(id) {
        this->setPosition(x, y);
        this->setFillColor(couleur);
        this->setSize(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
        this->setOutlineThickness(OUTLINE_THICK);
        this->setOutlineColor(sf::Color::Black);
    };

                        //DETECTION D'ACTIVATION DE BOUTON
    bool turnOnCheck(allButtons& button, sf::Mouse mouse, sf::RenderWindow& window) {
        if (button.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
            if (button.id.substr(0, 3) != "Mod") { //pour séparer les outils des modificateurs et couleurs
                button.buttonState = true;
                if (button.id.substr(0, 3) == "Col") { currentColor = button.getFillColor(); }
            }
            if (button.id == "Mod diminuerTaille") {
                if (currentSizePinceau <= 4) { currentSizePinceau = 3; }
                else { currentSizePinceau -= 3; }
            }
            if (button.id == "Mod augmenterTaille") {
                if (currentSizePinceau >= 37) { currentSizePinceau = 40; }
                else { currentSizePinceau += 3; }
            }
            if (button.id == "Mod clearAll") { vectorDrawing.clear(); vectorRectangle.clear(); vectorRond.clear(); vectorTriangle.clear(); }

            return true;
        }
        else { return false; }
    } //ajouter une condition pour éviter de changer de couleur quand la souris est activée ? faudrait faire une deuxième boucle copiée-collée ici
};


                        //POUR ENTOURER L'OUTIL ACTIF
void showActiveButton(allButtons& bouton) {
    if (bouton.id.substr(0, 3) != "Col") {
        //ajouter ici un if substr == "Mod" avec la meme fonction qu'en dessous mais set à genre 100 millisecondes
        if (bouton.buttonState) {
            bouton.setOutlineColor(sf::Color::Red); bouton.setOutlineThickness(3);
        }
        else {
            bouton.setOutlineColor(sf::Color::Black); bouton.setOutlineThickness(OUTLINE_THICK);
        }
    }
    else {
        if (bouton.buttonState) {
            bouton.setOutlineColor(sf::Color::Yellow); bouton.setOutlineThickness(3);
        }
        else {
            bouton.setOutlineColor(sf::Color::Black); bouton.setOutlineThickness(OUTLINE_THICK);
        }
    }
}

                        //POUR TIMER L'EFFACAGE DU VECTEUR QUI GERE LA GOMME
bool backgroundCheck(allButtons& bouton) {
    if (bouton.buttonState) {
        backgroundChoice = bouton.getFillColor();
        return true;
    }
    else { return false; }
}

int main() {
                        //MODE D'EMPLOI DANS LA CONSOLE
    std::cout << "                     MODE D'EMPLOI" << std::endl << std::endl
        << "Stylo : Dessiner (taille fine)" << std::endl << std::endl
        << "Pinceau : Dessiner (taille ajustable) et changer couleur des formes" << std::endl << std::endl
        << "Gomme : Gommer le stylo et le pinceau (taille ajustable)" << std::endl << std::endl
        << "Souris : Bouger des formes" << std::endl << std::endl
        << "Seau : Changer la couleur de l'arriere plan" << std::endl << std::endl
        << "Formes : Placer un rectangle, un rond ou un triangle" << std::endl 
        << "         (pour supprimer une seule forme, glisser la forme en dehors de l'ecran (pas reellement supprimee)" << std::endl
        << "         Cliquez sur plusieurs forme pour faire une combinaise" << std::endl
        << "         NE PAS DRAG UNE FORME SUR UNE AUTRE" << std::endl
        << "Poubelle : Effacer tout (sans modifier l'arriere plan)" << std::endl << std::endl;
        

                        //IMAGE POUR LE BACKGROUND DU MENU
    sf::Image image;
    image.create((unsigned int)WINDOW_WIDTH, (unsigned int)POS_LIGNE, sf::Color(222, 222, 222)); //(unsigned int) c'est pour éviter les avertissements
    sf::Texture textImage;
    textImage.loadFromImage(image);
    sf::Sprite spriteImage;
    spriteImage.setTexture(textImage);

                        //FONT
    sf::Font font;
    if (!font.loadFromFile("asset/arial.ttf")) { std::cout << "erreur chargement arial" << std::endl; }

                        // CREER FENETRE
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Paint SFML JRdgs", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

                        //POUR MODIF L'ICONE DE L'APP
/*sf::Image iconeApp;
if(!iconeApp.loadFromFile("asset/icone pinceau.png")) { std::cout << "erreur chargement icone appli" << std::endl; }
window.setIcon(iconeApp.getSize().x, iconeApp.getSize().y, iconeApp.getPixelsPtr());*/

                        //STYLO
    allButtons stylo("stylo", SET_ROW, SET_ROW, sf::Color::White);
    sf::Texture iconeStylo;
    if (!iconeStylo.loadFromFile("asset/icone stylo.png")) { std::cout << "erreur chargement icone stylo" << std::endl; }
    stylo.setTexture(&iconeStylo);
    //vectorButtons.push_back(stylo);

                        //SOURIS
    allButtons souris("souris", SET_ROW, SET_ROW + BUTTON_GAP, sf::Color::White);
    sf::Texture iconeSouris;
    if (!iconeSouris.loadFromFile("asset/icone souris.png")) { std::cout << "erreur chargement icone souris" << std::endl; }
    souris.setTexture(&iconeSouris);
    //vectorButtons.push_back(souris);

                        //PINCEAU
    allButtons pinceau("pinceau", SET_ROW + BUTTON_GAP, SET_ROW, sf::Color::White);
    sf::Texture iconePinceau;
    //sf::Image iconeApp;
    if (!iconePinceau.loadFromFile("asset/icone pinceau.png")) { std::cout << "erreur chargement icone pinceau" << std::endl; }
    pinceau.setTexture(&iconePinceau);
    //vectorButtons.push_back(pinceau);

                        //DIMINUER TAILLE PINCEAU
    allButtons diminuerTaille("Mod diminuerTaille", SET_ROW + BUTTON_GAP * 3, SET_ROW, sf::Color::White);
    sf::Texture flecheBas;
    if (!flecheBas.loadFromFile("asset/icone down.png")) { std::cout << "erreur chargement icone down" << std::endl; }
    diminuerTaille.setTexture(&flecheBas);
    //vectorButtons.push_back(diminuerTaille);

                        //AUGMENTER TAILLE PINCEAU
    allButtons augmenterTaille("Mod augmenterTaille", SET_ROW + BUTTON_GAP * 4, SET_ROW, sf::Color::White);
    sf::Texture flecheHaut;
    if (!flecheHaut.loadFromFile("asset/icone up.png")) { std::cout << "erreur chargement icone up" << std::endl; }
    augmenterTaille.setTexture(&flecheHaut);
    //vectorButtons.push_back(augmenterTaille);

                        //AFFICHER TAILLE PINCEAU
    std::string stringShowSize = std::to_string(currentSizePinceau);
    sf::Text displaySize(stringShowSize, font, 15);
    displaySize.setPosition(SET_ROW + BUTTON_GAP * 5, augmenterTaille.getPosition().y / 2 + SET_ROW);
    displaySize.setFillColor(sf::Color::Black);

                        //TOUT EFFACER
    allButtons clearAll("Mod clearAll", (WINDOW_WIDTH - BUTTON_SIZE) / 2, (POS_LIGNE - BUTTON_SIZE) / 2, sf::Color::White);
    sf::Texture poubelle;
    if (!poubelle.loadFromFile("asset/icone poubelle.png")) { std::cout << "erreur chargement icone poubelle" << std::endl; }
    clearAll.setTexture(&poubelle);
    //vectorButtons.push_back(clearAll);

                        //GOMME
    allButtons gomme("gomme", SET_ROW + BUTTON_GAP * 2, SET_ROW, sf::Color::White);
    sf::Texture iconeGomme;
    if (!iconeGomme.loadFromFile("asset/icone gomme.png")) { std::cout << "erreur chargement icone gomme" << std::endl; }
    gomme.setTexture(&iconeGomme);
    //vectorButtons.push_back(gomme);

                        //ECRIRE
    allButtons ecrire("ecrire", (WINDOW_WIDTH - BUTTON_SIZE) / 2 + BUTTON_GAP, (POS_LIGNE - BUTTON_SIZE) / 2, sf::Color::White);
    sf::Texture iconeEcrire;
    if (!iconeEcrire.loadFromFile("asset/icone ecrire.png")) { std::cout << "erreur chargement icone ecrire" << std::endl; }
    ecrire.setTexture(&iconeEcrire);
    //vectorButtons.push_back(ecrire);

                        //CHANGER COULEUR BACKGROUND
    allButtons backgroundColor("backgroundColor", SET_ROW + BUTTON_GAP, SET_ROW + BUTTON_GAP, sf::Color::White);
    sf::Texture iconeBackgroundColor;
    if (!iconeBackgroundColor.loadFromFile("asset/icone background.png")) { std::cout << "erreur chargement icone background" << std::endl; }
    backgroundColor.setTexture(&iconeBackgroundColor);
    //vectorButtons.push_back(backgroundColor);

                        //PLACER FORMES
    allButtons placeShape("placeShapes", SET_ROW + BUTTON_GAP * 2, SET_ROW + BUTTON_GAP, sf::Color::White);
    sf::Texture iconePlaceShape;
    if (!iconePlaceShape.loadFromFile("asset/icone shapes.png")) { std::cout << "erreur chargement icone shapes" << std::endl; }
    placeShape.setTexture(&iconePlaceShape);
    //vectorButtons.push_back(backgroundColor);

                        //PLACER RECTANGLE
    allButtons placeRectangle("placeRectangle", SET_ROW + BUTTON_GAP * 3, SET_ROW + BUTTON_GAP, sf::Color::White);
    placeRectangle.setOutlineThickness(0);
    sf::Texture iconeRectangle;
    if (!iconeRectangle.loadFromFile("asset/icone rectangle.png")) { std::cout << "erreur chargement icone rectangle" << std::endl; }
    placeRectangle.setTexture(&iconeRectangle);
    //vectorButtons.push_back(backgroundColor);

                        //PLACER ROND
    allButtons placeRond("placeRond", SET_ROW + BUTTON_GAP * 4, SET_ROW + BUTTON_GAP, sf::Color::White);
    placeRond.setOutlineThickness(0);
    sf::Texture iconeRond;
    if (!iconeRond.loadFromFile("asset/icone rond.png")) { std::cout << "erreur chargement icone rond" << std::endl; }
    placeRond.setTexture(&iconeRond);
    //vectorButtons.push_back(backgroundColor);

                        //PLACER TRIANGE
    allButtons placeTriangle("placeTriangle", SET_ROW + BUTTON_GAP * 5, SET_ROW + BUTTON_GAP, sf::Color::White);
    placeTriangle.setOutlineThickness(0);
    sf::Texture iconeTriangle;
    if (!iconeTriangle.loadFromFile("asset/icone triangle.png")) { std::cout << "erreur chargement icone triangle" << std::endl; }
    placeTriangle.setTexture(&iconeTriangle);
    //vectorButtons.push_back(backgroundColor);

                        //COULEURS
    //NOIR
    allButtons noir("Col noir", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE, SET_ROW, sf::Color::Black);
    //vectorButtons.push_back(noir);

    //ROUGE
    allButtons rouge("Col rouge", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP, SET_ROW, sf::Color::Red);
    //vectorButtons.push_back(rouge);

    //BLEU
    allButtons bleu("Col bleu", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 2, SET_ROW, sf::Color::Blue);
    //vectorButtons.push_back(bleu);

    //VERT
    allButtons vert("Col vert", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP, SET_ROW + BUTTON_GAP, sf::Color::Green);
    //vectorButtons.push_back(vert);

    //ROSE
    allButtons rose("Col rose", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 2, SET_ROW + BUTTON_GAP, sf::Color(255, 0, 255));
    //vectorButtons.push_back(rose);

    //BLANC
    allButtons blanc("Col blanc", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE, SET_ROW + BUTTON_GAP, sf::Color::White);
    //vectorButtons.push_back(blanc);

                        //LIGNE SEPARATRICE
    sf::VertexArray ligne(sf::Lines);
    ligne.append(sf::Vertex(sf::Vector2f(0, POS_LIGNE), sf::Color::Black));
    ligne.append(sf::Vertex(sf::Vector2f(WINDOW_WIDTH, POS_LIGNE), sf::Color::Black));

    //BOUCLE PRINCIPALE
    while (window.isOpen()) {

        stringShowSize = "(" + std::to_string(currentSizePinceau) + ")"; //actualiser l'affichage de la taille du pinceau
        displaySize.setString(stringShowSize);
        textImage.loadFromImage(image);
        spriteImage.setTexture(textImage);

        sf::Event event;
        sf::Mouse mouse;

                    //BOUCLE D'EVENEMENTS
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::Escape) { window.close(); }
                //if (ecrireActif) {
                //    std::cout << "debut input text" << std::endl;
                //    cinUserText += sf::Keyboard::getDescription(event.key.scancode).toAnsiString();
                //    std::cout << cinUserText << std::endl;
                //    sf::Text userText;
                //    userText.setString(cinUserText);
                //    userText.setFont(font);
                //    userText.setPosition(mouse.getPosition(window).x, mouse.getPosition(window).y); //  /////////PAS FINI, MARCHE PAS
                //    window.draw(userText);
                //    if (event.key.code == sf::Keyboard::Key::Enter) {
                //        ecrireActif = false;
                //        std::cout << "fin input text" << std::endl;
                //    }
                //}  début fonction pour écrire du texte
            }

            if (event.type == sf::Event::Closed) {
                window.close();
            }

                    //DETECTION DES CLIQUES SUR ICONES
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (souris.turnOnCheck(souris, mouse, window)) {
                        stylo.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false; 
                        backgroundColor.buttonState = false; placeShape.buttonState = false;
                    }
                    if (stylo.turnOnCheck(stylo, mouse, window)) {
                        souris.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false; 
                        backgroundColor.buttonState = false; placeShape.buttonState = false;
                    }
                    if (pinceau.turnOnCheck(pinceau, mouse, window)) {
                        souris.buttonState = false; stylo.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false; 
                        backgroundColor.buttonState = false; placeShape.buttonState = false;
                    }
                    if (gomme.turnOnCheck(gomme, mouse, window)) {
                        souris.buttonState = false; stylo.buttonState = false; pinceau.buttonState = false; ecrire.buttonState = false; 
                        backgroundColor.buttonState = false; placeShape.buttonState = false;
                    }
                    /*if (ecrire.turnOnCheck(ecrire, mouse, window)) {
                        souris.buttonState = false; stylo.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; 
                        backgroundColor.buttonState = false; placeShape.buttonState = false;
                    }*/
                    if (placeShape.turnOnCheck(placeShape, mouse, window)) {
                        souris.buttonState = false; stylo.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; 
                        backgroundColor.buttonState = false; ecrire.buttonState = false;
                    }
                    if (backgroundColor.turnOnCheck(backgroundColor, mouse, window)) {
                        souris.buttonState = false; stylo.buttonState = false; pinceau.buttonState = false; 
                        gomme.buttonState = false; ecrire.buttonState = false; placeShape.buttonState = false;
                        //désactiver les couleurs en plus pour ne pas appliquer une couleur au background dès qu'on clique sur le seau
                        rouge.buttonState = false; bleu.buttonState = false; vert.buttonState = false; 
                        noir.buttonState = false; rose.buttonState = false; blanc.buttonState = false;
                    }

                    clearAll.turnOnCheck(clearAll, mouse, window);

                    if (pinceau.buttonState || gomme.buttonState) { //pour que le choix de la taille soit exclusif au pinceau et à la gomme
                        diminuerTaille.turnOnCheck(diminuerTaille, mouse, window);
                        augmenterTaille.turnOnCheck(augmenterTaille, mouse, window);
                    }
                    if (placeShape.buttonState) {
                        placeRectangle.turnOnCheck(placeRectangle, mouse, window);
                        placeRond.turnOnCheck(placeRond, mouse, window);
                        placeTriangle.turnOnCheck(placeTriangle, mouse, window);
                    }
                    if (pinceau.buttonState || stylo.buttonState || backgroundColor.buttonState) {
                        if (noir.turnOnCheck(noir, mouse, window)) {
                            rouge.buttonState = false; bleu.buttonState = false; vert.buttonState = false; rose.buttonState = false; blanc.buttonState = false;
                        }
                        if (rouge.turnOnCheck(rouge, mouse, window)) {
                            noir.buttonState = false; bleu.buttonState = false; vert.buttonState = false; rose.buttonState = false; blanc.buttonState = false;
                        }
                        if (bleu.turnOnCheck(bleu, mouse, window)) {
                            rouge.buttonState = false; noir.buttonState = false; vert.buttonState = false; rose.buttonState = false; blanc.buttonState = false;
                        }
                        if (vert.turnOnCheck(vert, mouse, window)) {
                            rouge.buttonState = false; bleu.buttonState = false; noir.buttonState = false; rose.buttonState = false; blanc.buttonState = false;
                        }
                        if (rose.turnOnCheck(rose, mouse, window)) {
                            rouge.buttonState = false; bleu.buttonState = false; vert.buttonState = false; noir.buttonState = false; blanc.buttonState = false;
                        }
                        if (blanc.turnOnCheck(blanc, mouse, window)) {
                            rouge.buttonState = false; bleu.buttonState = false; vert.buttonState = false; noir.buttonState = false; rose.buttonState = false;
                        }
                    }
                }
            }
        }
                        //QUE FAIRE QUAND ICONE ACTIVE                
        if (souris.buttonState) {
            if (mouse.getPosition(window).y >= POS_LIGNE - SET_ROW) { 
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    for (sf::RectangleShape& rectangle : vectorRectangle) {
                        if (rectangle.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
                            rectangle.setPosition(mouse.getPosition(window).x - DEFAULT_RECT_SIZE / 2, mouse.getPosition(window).y - DEFAULT_RECT_SIZE / 4);
                        }
                    }
                    for (sf::CircleShape& rond : vectorRond) {
                        if (rond.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
                            rond.setPosition(mouse.getPosition(window).x - DEFAULT_CIRCLE_RADIUS, mouse.getPosition(window).y - DEFAULT_CIRCLE_RADIUS);
                        }
                    }
                    for (sf::ConvexShape& triangle : vectorTriangle) {
                        if (triangle.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
                            triangle.setPoint(0, sf::Vector2f(mouse.getPosition(window).x, mouse.getPosition(window).y - DEFAULT_TRIANGLE_SIZE));
                            triangle.setPoint(1, sf::Vector2f(mouse.getPosition(window).x - DEFAULT_TRIANGLE_SIZE, mouse.getPosition(window).y + DEFAULT_TRIANGLE_SIZE));
                            triangle.setPoint(2, sf::Vector2f(mouse.getPosition(window).x + DEFAULT_TRIANGLE_SIZE, mouse.getPosition(window).y + DEFAULT_TRIANGLE_SIZE));
                        }
                    }
                }
            }
        }
        if (stylo.buttonState) {
            if (mouse.getPosition(window).y >= POS_LIGNE - SET_ROW) { // pour ne pas dessiner quand on clique sur une icone
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    //image.setPixel(mouse.getPosition(window).x, mouse.getPosition(window).y, setColor);  anciennement pour setPixel sur une image
                    Drawings shape;
                    shape.setRadius(1);
                    shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius()); //centrer rond sur souris
                    shape.setFillColor(currentColor);
                    vectorDrawing.push_back(shape);
                    window.draw(shape);
                }
            }

        }
        if (pinceau.buttonState) {
            if (mouse.getPosition(window).y >= POS_LIGNE - SET_ROW) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (isDrawing == false) {
                        for (sf::RectangleShape& rectangle : vectorRectangle) {
                            if (rectangle.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
                                rectangle.setFillColor(currentColor);
                                selecShapeCount = 1;
                            }
                        }
                        for (sf::CircleShape& rond : vectorRond) {
                            if (rond.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
                                rond.setFillColor(currentColor);
                                selecShapeCount = 1;
                            }
                        }
                        for (sf::ConvexShape& triangle : vectorTriangle) {
                            if (triangle.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window)))) {
                                triangle.setFillColor(currentColor);
                                selecShapeCount = 1;
                            }
                        }
                    }
                    if (selecShapeCount == 0) {
                        isDrawing = true;
                        Drawings shape;
                        shape.setRadius((float)currentSizePinceau);//(float) c'est pour enlever avertissement
                        shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius());
                        shape.setFillColor(currentColor);
                        vectorDrawing.push_back(shape);
                        window.draw(shape);
                    }
                    selecShapeCount = 0;
                }
                if (event.type == sf::Event::MouseButtonReleased) isDrawing = false;
            }
        }
        if (gomme.buttonState) {
            if (mouse.getPosition(window).y >= POS_LIGNE - SET_ROW) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    Drawings shape;
                    shape.setRadius((float)currentSizePinceau);//(float) c'est pour enlever avertissement
                    shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius());
                    shape.setFillColor(backgroundChoice);
                    shape.gommeState = true;
                    vectorDrawing.push_back(shape);
                    window.draw(shape);
                }
            }
        }
        if (backgroundColor.buttonState) {
            if (backgroundCheck(noir)) for (Drawings& gomme : vectorDrawing) { if (gomme.gommeState) gomme.setFillColor(noir.getFillColor()); }
            if (backgroundCheck(rouge)) for (Drawings& gomme : vectorDrawing) { if (gomme.gommeState) gomme.setFillColor(rouge.getFillColor()); }
            if (backgroundCheck(bleu)) for (Drawings& gomme : vectorDrawing) { if (gomme.gommeState) gomme.setFillColor(bleu.getFillColor()); }
            if (backgroundCheck(vert)) for (Drawings& gomme : vectorDrawing) { if (gomme.gommeState) gomme.setFillColor(vert.getFillColor()); }
            if (backgroundCheck(rose)) for (Drawings& gomme : vectorDrawing) { if (gomme.gommeState) gomme.setFillColor(rose.getFillColor()); }
            if (backgroundCheck(blanc)) for (Drawings& gomme : vectorDrawing) { if (gomme.gommeState) gomme.setFillColor(blanc.getFillColor()); }
        }
        //if (ecrire.buttonState) {
        //    if (mouse.getPosition(window).y > POS_LIGNE) { // écrire du texte uniquement sur le plan
        //        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        //            ecrire.buttonState = false; //pour avoir uniquement 1 button press, à priorit ça n'arrête pas le if
        //            ecrireActif = true;
        //        }
        //    }
        //} pas fini la fonction écrire
        if (placeShape.buttonState) { //nécessaire dans le cas où on a appuyé sur une des 3 formes mais qu'on souhaite utiliser un autre outil
            if (placeRectangle.buttonState) {
                if (mouse.getPosition(window).y >= POS_LIGNE) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        sf::RectangleShape shape(sf::Vector2f(DEFAULT_RECT_SIZE, DEFAULT_RECT_SIZE / 2));
                        shape.setPosition(mouse.getPosition(window).x - DEFAULT_RECT_SIZE/2, mouse.getPosition(window).y - DEFAULT_RECT_SIZE/4);
                        shape.setOutlineColor(sf::Color::Black);
                        shape.setOutlineThickness(1);
                        shape.setFillColor(shapeColor);
                        vectorRectangle.push_back(shape);
                        placeRectangle.buttonState = false;
                        placeShape.buttonState = false;
                    }
                }
            }
            if (placeRond.buttonState) {
                if (mouse.getPosition(window).y >= POS_LIGNE) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        sf::CircleShape shape(DEFAULT_CIRCLE_RADIUS);
                        shape.setPosition(mouse.getPosition(window).x - DEFAULT_CIRCLE_RADIUS, mouse.getPosition(window).y - DEFAULT_CIRCLE_RADIUS);
                        shape.setOutlineColor(sf::Color::Black);
                        shape.setOutlineThickness(1);
                        shape.setFillColor(shapeColor);
                        vectorRond.push_back(shape);
                        placeRond.buttonState = false;
                        placeShape.buttonState = false;
                    }
                }
            }
            if (placeTriangle.buttonState) {
                if (mouse.getPosition(window).y >= POS_LIGNE) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        sf::ConvexShape shape;
                        shape.setPointCount(3);
                        shape.setPoint(0, sf::Vector2f(mouse.getPosition(window).x, mouse.getPosition(window).y - DEFAULT_TRIANGLE_SIZE));
                        shape.setPoint(1, sf::Vector2f(mouse.getPosition(window).x - DEFAULT_TRIANGLE_SIZE, mouse.getPosition(window).y + DEFAULT_TRIANGLE_SIZE));
                        shape.setPoint(2, sf::Vector2f(mouse.getPosition(window).x + DEFAULT_TRIANGLE_SIZE, mouse.getPosition(window).y + DEFAULT_TRIANGLE_SIZE));
                        shape.setOutlineColor(sf::Color::Black);
                        shape.setOutlineThickness(1);
                        shape.setFillColor(shapeColor);
                        vectorTriangle.push_back(shape);
                        placeTriangle.buttonState = false;
                        placeShape.buttonState = false;
                    }
                }
            }
        }
                    //CHANGER L'OUTLINE DE LA CASE SELECTIONNEE (pas réussi avec une boucle dans un vecteur)
        showActiveButton(souris);
        showActiveButton(stylo);
        showActiveButton(pinceau);
        showActiveButton(diminuerTaille);
        showActiveButton(augmenterTaille);
        showActiveButton(gomme);
        showActiveButton(clearAll); 
        showActiveButton(ecrire);
        showActiveButton(backgroundColor);
        showActiveButton(noir);
        showActiveButton(rouge);
        showActiveButton(bleu);
        showActiveButton(vert);
        showActiveButton(rose);
        showActiveButton(blanc);

                    //DESSINER
        window.clear(backgroundChoice);
        for (Drawings& points : vectorDrawing) {
            window.draw(points);
        }
        for (sf::RectangleShape& shapes : vectorRectangle) {
            window.draw(shapes);
        }
        for (sf::CircleShape& shapes : vectorRond) {
            window.draw(shapes);
        }
        for (sf::ConvexShape& shapes : vectorTriangle) {
            window.draw(shapes);
        }
        window.draw(spriteImage);//background du menu (pour éviter de dessiner dessus)
        window.draw(ligne); //la ligne qui sépare le menu de l'espace de dessin

        window.draw(souris);
        window.draw(stylo);
        window.draw(pinceau);
        window.draw(gomme);
        window.draw(clearAll);
        //window.draw(ecrire); pas fini
        window.draw(placeShape);
        window.draw(noir);
        window.draw(rouge);
        window.draw(bleu);
        window.draw(vert);
        window.draw(rose);
        window.draw(blanc);
        window.draw(backgroundColor);
        //for (allButtons &bouton : vectorButtons) {
        //    window.draw(bouton);//tous les boutons 
        //}

        if (placeShape.buttonState) {
            window.draw(placeRectangle);
            window.draw(placeRond);
            window.draw(placeTriangle);
        }
        if (pinceau.buttonState || gomme.buttonState) {
            window.draw(diminuerTaille);
            window.draw(augmenterTaille);
            window.draw(displaySize);
        }
        window.display();
    }
    return 0;
}

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float BUTTON_SIZE = 30.f;

const float SET_ROW = 10.f; //écart entre les lignes

const float POS_LIGNE = BUTTON_SIZE * 2 + SET_ROW * 3; // le menu a la place pour 2 lignes d'icones

const float BUTTON_GAP = SET_ROW + BUTTON_SIZE; //pour éviter les calculs trop longs

const float OUTLINE_THICK = 1.0f;

unsigned int currentSizePinceau = 10;

//COULEUR ACTUELLE
sf::Color currentColor = sf::Color::Black;

std::vector<sf::CircleShape> vectorDrawing;

class allButtons : public sf::RectangleShape{
public :
    
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
            std::cout << "dessiner" << std::endl;
            if (button.id != "diminuerTaille" || button.id != "augmenterTaille" || button.id != "clearAll") {
                button.buttonState = true;
            }
            if (button.id == "diminuerTaille") {
                if (currentSizePinceau <= 3) { std::cout << "min size" << std::endl; currentSizePinceau = 3; }
                else { currentSizePinceau -= 3; }
            } 
            if (button.id == "augmenterTaille") {
                if (currentSizePinceau >= 37) { std::cout << "max size" << std::endl; currentSizePinceau = 40; }
                else { currentSizePinceau += 3; }
            }
            if (button.id == "clearAll") {
                vectorDrawing.clear();
                std::cout << "reset" << std::endl;
            }
            if (button.getFillColor() != sf::Color::White) { //si le bouton est une couleur, alors la couleur actuelle devient celle-ci
                currentColor = button.getFillColor();
            }
            return true;
        }
        return false;
    } //ajouter une condition pour éviter de changer de couleur quand la souris est activée ? faudrait faire une deuxième boucle copiée-collée ici
};


std::vector<allButtons> vectorButtons;

bool ecrireActif = false;

std::string cinUserText;

int main() {
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
    allButtons souris("souris", SET_ROW, SET_ROW, sf::Color::White);
    sf::Texture iconeSouris;
    if (!iconeSouris.loadFromFile("asset/icone souris.png")) { std::cout << "erreur chargement icone souris" << std::endl; }
    souris.setTexture(&iconeSouris);
    vectorButtons.push_back(souris);

                        //STYLO
    allButtons stylo("stylo", SET_ROW + BUTTON_GAP, SET_ROW, sf::Color::White);
    sf::Texture iconeStylo;
    if (!iconeStylo.loadFromFile("asset/icone stylo.png")) { std::cout << "erreur chargement icone stylo" << std::endl; }
    stylo.setTexture(&iconeStylo);
    vectorButtons.push_back(stylo);

                        //PINCEAU
    allButtons pinceau("pinceau", SET_ROW + BUTTON_GAP * 2, SET_ROW, sf::Color::White);
    sf::Texture iconePinceau;
    //sf::Image iconeApp;
    if (!iconePinceau.loadFromFile("asset/icone pinceau.png") /*&& !iconeApp.loadFromFile("asset/icone pinceau.png")*/) { std::cout << "erreur chargement icone pinceau" << std::endl; }
    pinceau.setTexture(&iconePinceau);
    vectorButtons.push_back(pinceau);
    // window.setIcon(iconeApp.getSize().x, iconeApp.getSize().y, iconeApp.getPixelsPtr());

                        //TAILLE PINCEAU
    //DIMINUER
    allButtons diminuerTaille("diminuerTaille", SET_ROW + BUTTON_GAP * 3, SET_ROW, sf::Color::White);
    //diminuerTaille.setPosition(400, 400);
    sf::Texture flecheBas;
    if (!flecheBas.loadFromFile("asset/icone down.png")) { std::cout << "erreur chargement icone down" << std::endl; }
    diminuerTaille.setTexture(&flecheBas);
    vectorButtons.push_back(diminuerTaille);

    //AUGMENTER
    allButtons augmenterTaille("augmenterTaille", SET_ROW + BUTTON_GAP * 4, SET_ROW, sf::Color::White);
    //augmenterTaille.setPosition(500, 400);
    sf::Texture flecheHaut;
    if (!flecheHaut.loadFromFile("asset/icone up.png")) { std::cout << "erreur chargement icone up" << std::endl; }
    augmenterTaille.setTexture(&flecheHaut);
    vectorButtons.push_back(augmenterTaille);

                        //TOUT EFFACER
    allButtons clearAll("clearAll", (WINDOW_WIDTH - BUTTON_SIZE) / 2, (POS_LIGNE - BUTTON_SIZE) / 2, sf::Color::White);
    sf::Texture poubelle;
    if (!poubelle.loadFromFile("asset/icone poubelle.png")) { std::cout << "erreur chargement icone poubelle" << std::endl; }
    clearAll.setTexture(&poubelle);
    vectorButtons.push_back(clearAll);

                        //GOMME
    allButtons gomme("gomme", SET_ROW, SET_ROW + BUTTON_GAP, sf::Color::White);
    sf::Texture iconeGomme;
    if (!iconeGomme.loadFromFile("asset/icone gomme.png")) { std::cout << "erreur chargement icone gomme" << std::endl; }
    gomme.setTexture(&iconeGomme);
    vectorButtons.push_back(gomme);

                        //ECRIRE
    allButtons ecrire("ecrire", SET_ROW + BUTTON_GAP, SET_ROW + BUTTON_GAP, sf::Color::White);
    sf::Texture iconeEcrire;
    if (!iconeEcrire.loadFromFile("asset/icone ecrire.png")) { std::cout << "erreur chargement icone ecrire" << std::endl; }
    ecrire.setTexture(&iconeEcrire);
    vectorButtons.push_back(ecrire);


                        //COULEURS
    //NOIR
    allButtons noir("couleur noir", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE, SET_ROW, sf::Color::Black);
    vectorButtons.push_back(noir);

    //ROUGE
    allButtons rouge("couleur rouge", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP, SET_ROW, sf::Color::Red);
    vectorButtons.push_back(rouge);

    //BLEU
    allButtons bleu("couleur bleu", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 2, SET_ROW, sf::Color::Blue);
    vectorButtons.push_back(bleu);

    //VERT
    allButtons vert("couleur vert", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 3, SET_ROW, sf::Color::Green);
    vectorButtons.push_back(vert);

    //ROSE
    allButtons rose("couleur rose", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE - BUTTON_GAP * 4, SET_ROW, sf::Color(255, 0, 255));
    vectorButtons.push_back(rose);

    //LIGNE SEPARATRICE
    sf::VertexArray ligne(sf::Lines);
    ligne.append(sf::Vertex(sf::Vector2f(0, POS_LIGNE), sf::Color::Black));
    ligne.append(sf::Vertex(sf::Vector2f(WINDOW_WIDTH, POS_LIGNE), sf::Color::Black));

    //if (stylo.buttonState) std::cout << "STYLOOOOOOOOOOOOOOO" << std::endl;

    while (window.isOpen()) {

        textImage.loadFromImage(image);
        spriteImage.setTexture(textImage);
        
        sf::Event event;
        sf::Mouse mouse;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Key::Escape) { window.close(); }
                //if (ecrireActif) {
                //    std::cout << "debut input text" << std::endl;
                //    cinUserText += sf::Keyboard::getDescription(event.key.scancode).toAnsiString();
                //    std::cout << cinUserText << std::endl;
                //    sf::Text userText;
                //    userText.setString(cinUserText);
                //    userText.setFont(font);
                //    userText.setPosition(mouse.getPosition(window).x, mouse.getPosition(window).y); //  ////////////////////////////////////PAS FINI, MARCHE PAS
                //    window.draw(userText);
                //    if (event.key.code == sf::Keyboard::Key::Enter) {
                //        ecrireActif = false;
                //        std::cout << "fin input text" << std::endl;
                //    }
                //}
            }

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //if (event.type == sf::Event::MouseMoved) 
            // std::cout << "X : " << mouse.getPosition(window).x << "  Y : " << mouse.getPosition(window).y << std::endl;

            if (event.type == sf::Event::MouseButtonReleased) {

                if (event.mouseButton.button == sf::Mouse::Left) {
                    souris.turnOnCheck(souris, mouse, window);
                    if (souris.buttonState) { stylo.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false; }
                    stylo.turnOnCheck(stylo, mouse, window);
                    if (stylo.buttonState) { souris.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false; }
                    pinceau.turnOnCheck(pinceau, mouse, window);
                    if (pinceau.buttonState) { souris.buttonState = false; stylo.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false; }
                    gomme.turnOnCheck(gomme, mouse, window);
                    if (gomme.buttonState) { souris.buttonState = false; stylo.buttonState = false; pinceau.buttonState = false; ecrire.buttonState = false; }
                    ecrire.turnOnCheck(ecrire, mouse, window);
                    if (ecrire.buttonState) { souris.buttonState = false; stylo.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; }
                    clearAll.turnOnCheck(clearAll, mouse, window);
                    diminuerTaille.turnOnCheck(diminuerTaille, mouse, window);
                    augmenterTaille.turnOnCheck(augmenterTaille, mouse, window);
                    noir.turnOnCheck(noir, mouse, window);
                    rouge.turnOnCheck(rouge, mouse, window);
                    bleu.turnOnCheck(bleu, mouse, window);
                    vert.turnOnCheck(vert, mouse, window);
                    rose.turnOnCheck(rose, mouse, window);
                }
            }
        }
        if (stylo.buttonState && mouse.getPosition(window).y >= POS_LIGNE-currentSizePinceau * 2) { // pour ne pas dessiner sur le menu
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                //image.setPixel(mouse.getPosition(window).x, mouse.getPosition(window).y, setColor);  anciennement pour setPixel sur une image
                sf::CircleShape shape = sf::CircleShape(1);
                shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius()); //centrer rond sur souris
                shape.setFillColor(currentColor);
                vectorDrawing.push_back(shape);
                window.draw(shape);
            }
        }
        if (pinceau.buttonState) {
            if (mouse.getPosition(window).y >= POS_LIGNE-currentSizePinceau * 2) { // pour ne pas dessiner sur le menu
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::CircleShape shape = sf::CircleShape(currentSizePinceau);
                    shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius());
                    shape.setFillColor(currentColor);
                    vectorDrawing.push_back(shape);
                    window.draw(shape);
                }
            }
        }
        if (gomme.buttonState) {
            if (mouse.getPosition(window).y >= POS_LIGNE - currentSizePinceau * 2) { // pour ne pas dessiner sur le menu
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::CircleShape shape = sf::CircleShape(currentSizePinceau);
                    shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius());
                    shape.setFillColor(sf::Color::White);
                    vectorDrawing.push_back(shape);
                    window.draw(shape);
                }
            }
        }
        if (ecrire.buttonState) {
            if (mouse.getPosition(window).y > POS_LIGNE) { // écrire du texte uniquement sur le plan
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    ecrire.buttonState = false; //pour avoir uniquement 1 button press, à priorit ça n'arrête pas le if
                    ecrireActif = true;
                }
            }
        }
        /*else {
            diminuerTaille.setPosition(400, 400);
            augmenterTaille.setPosition(500 , 400);
        }*/

        //DESSINER
        window.clear(sf::Color::White); //on peut ajouter un if ici qui permet de check si un bouton de choix de background est appuyé, un peu useless tho
        for (sf::CircleShape& shape : vectorDrawing) {
            window.draw(shape);//les cercles créés par le pinceau, a potentiellement changer
        }
        window.draw(spriteImage);
        window.draw(ligne); //la ligne qui sépare les boutons de l'espace de dessin
        for (allButtons &bouton : vectorButtons) {
            window.draw(bouton);//tous les boutons
        }
        window.display();

    }

    return 0;
}

/*                            BAILS A FIX
* 
REMPLIRE L'ESPACE ENTRE LES RONDS

DRAW LE CHOIX DES TAILLES UNIQUEMENT QUAND LE PINCEAU EST UTILISE

DESACTIVER AUTOMATIQUEMENT LES BOOLEENS DES BOUTONS DANS LA FONCTIONE ACTIVEBUTTON()

DEMANDER : SI LA CONDITION D'UN IF EST UN TRUE, EST CE QUE SI JE MET CE BOOLEEN EN FALSE AU DEBUT DU IF, CELUI CI S'ARRETE ?
           OU ALORS CA CONTINU PCK LE BOOLEEN ETAIT TRUE UNE FOIS ? (j'espère) C'EST POUR LE IF(ECRIREB) 

           DEMANDER AUTREMENT : EST CE QUE LA CONDITION D'UN IF EST CHECK UNIQUEMENT AVANT LE IF, OU MEME PENDANT LE IF ?

           PAS FINI FONCTION POUR ECRIRE
*/
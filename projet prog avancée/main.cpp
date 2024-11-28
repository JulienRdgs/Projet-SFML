
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
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

                    //VARIABLES
unsigned int currentSizePinceau = 10;
bool ecrireActif = false;
std::string cinUserText;
sf::Color currentColor = sf::Color::Black;
sf::Color backgroundChoice = sf::Color::White;

                    //VECTEURS
std::vector<sf::CircleShape> vectorDrawing;
//std::vector<allButtons> vectorButtons;

                    //CLASSE DES BOUTONS
class allButtons : public sf::RectangleShape {
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
            if (button.id.substr(0, 3) != "Mod") { //pour séparer les outils des modificateurs et couleurs
                button.buttonState = true;
                system("cls");
                std::cout << button.id << std::endl; //montrer le choix de bouton dans la console
                if (button.id == "pinceau") std::cout << "current size : " << currentSizePinceau << std::endl;
                if (button.id.substr(0, 3) == "Col") { //si le bouton est une couleur, alors la couleur actuelle devient celle-ci
                    currentColor = button.getFillColor();
                }
            }
            if (button.id == "Mod diminuerTaille") {
                if (currentSizePinceau <= 4) {
                    currentSizePinceau = 3;
                    system("cls");
                    std::cout << "min size : " << currentSizePinceau << std::endl;
                }
                else {
                    currentSizePinceau -= 3;
                    system("cls");
                    std::cout << "size down : " << currentSizePinceau << std::endl;
                }
            }
            if (button.id == "Mod augmenterTaille") {
                if (currentSizePinceau >= 37) {
                    currentSizePinceau = 40;
                    system("cls");
                    std::cout << "max size : " << currentSizePinceau << std::endl;
                }
                else {
                    currentSizePinceau += 3;
                    system("cls");
                    std::cout << "size up : " << currentSizePinceau << std::endl;
                }
            }
            if (button.id == "Mod clearAll") {
                vectorDrawing.clear();
                system("cls");
                std::cout << "all cleared" << std::endl;
            }
            return true;
        }
        else { return false; }
    } //ajouter une condition pour éviter de changer de couleur quand la souris est activée ? faudrait faire une deuxième boucle copiée-collée ici
};

                    //POUR ENTOURER L'OUTIL ACTIF
void showActiveButton(allButtons &bouton) {
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

int main() {
                        //IMAGE POUR LE BACKGROUND DU MENU
    sf::Image image;
    image.create((unsigned int)WINDOW_WIDTH, (unsigned int)POS_LIGNE, sf::Color(222,222,222)); //(unsigned int) c'est pour éviter les avertissements
    sf::Texture textImage;
    textImage.loadFromImage(image);
    sf::Sprite spriteImage;
    spriteImage.setTexture(textImage);

                        //FONT
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
    //vectorButtons.push_back(souris);

                        //STYLO
    allButtons stylo("stylo", SET_ROW + BUTTON_GAP, SET_ROW, sf::Color::White);
    sf::Texture iconeStylo;
    if (!iconeStylo.loadFromFile("asset/icone stylo.png")) { std::cout << "erreur chargement icone stylo" << std::endl; }
    stylo.setTexture(&iconeStylo);
    //vectorButtons.push_back(stylo);

                        //PINCEAU
    allButtons pinceau("pinceau", SET_ROW + BUTTON_GAP * 2, SET_ROW, sf::Color::White);
    sf::Texture iconePinceau;
    //sf::Image iconeApp;
    if (!iconePinceau.loadFromFile("asset/icone pinceau.png")) { std::cout << "erreur chargement icone pinceau" << std::endl; }
    pinceau.setTexture(&iconePinceau);
    //vectorButtons.push_back(pinceau);

    //POUR MODIF L'ICONE DE L'APP
    /*sf::Image iconeApp;
    if(!iconeApp.loadFromFile("asset/icone pinceau.png")) { std::cout << "erreur chargement icone appli" << std::endl; }
    window.setIcon(iconeApp.getSize().x, iconeApp.getSize().y, iconeApp.getPixelsPtr());*/

                        //TAILLE PINCEAU
    //DIMINUER
    allButtons diminuerTaille("Mod diminuerTaille", SET_ROW + BUTTON_GAP * 3, (POS_LIGNE - BUTTON_SIZE) / 2, sf::Color::White);
    sf::Texture flecheBas;
    if (!flecheBas.loadFromFile("asset/icone down.png")) { std::cout << "erreur chargement icone down" << std::endl; }
    diminuerTaille.setTexture(&flecheBas);
    //vectorButtons.push_back(diminuerTaille);

    //AUGMENTER
    allButtons augmenterTaille("Mod augmenterTaille", SET_ROW + BUTTON_GAP * 4, (POS_LIGNE - BUTTON_SIZE) / 2, sf::Color::White);
    sf::Texture flecheHaut;
    if (!flecheHaut.loadFromFile("asset/icone up.png")) { std::cout << "erreur chargement icone up" << std::endl; }
    augmenterTaille.setTexture(&flecheHaut);
    //vectorButtons.push_back(augmenterTaille);

                        //TOUT EFFACER
    allButtons clearAll("Mod clearAll", (WINDOW_WIDTH - BUTTON_SIZE) / 2, (POS_LIGNE - BUTTON_SIZE) / 2, sf::Color::White);
    sf::Texture poubelle;
    if (!poubelle.loadFromFile("asset/icone poubelle.png")) { std::cout << "erreur chargement icone poubelle" << std::endl; }
    clearAll.setTexture(&poubelle);
    //vectorButtons.push_back(clearAll);

                        //GOMME
    allButtons gomme("gomme", SET_ROW + BUTTON_GAP * 2, SET_ROW + BUTTON_GAP, sf::Color::White);
    sf::Texture iconeGomme;
    if (!iconeGomme.loadFromFile("asset/icone gomme.png")) { std::cout << "erreur chargement icone gomme" << std::endl; }
    gomme.setTexture(&iconeGomme);
    //vectorButtons.push_back(gomme);

                        //ECRIRE
    allButtons ecrire("ecrire", SET_ROW + BUTTON_GAP, SET_ROW + BUTTON_GAP, sf::Color::White);
    sf::Texture iconeEcrire;
    if (!iconeEcrire.loadFromFile("asset/icone ecrire.png")) { std::cout << "erreur chargement icone ecrire" << std::endl; }
    ecrire.setTexture(&iconeEcrire);
    //vectorButtons.push_back(ecrire);

                        //CHANGER COULEUR BACKGROUND
    allButtons backgroundColor("backgroundColor", SET_ROW, SET_ROW + BUTTON_GAP, sf::Color::White);
    sf::Texture iconeBackgroundColor;
    if (!iconeBackgroundColor.loadFromFile("asset/icone background.png")) { std::cout << "erreur chargement icone background" << std::endl; }
    backgroundColor.setTexture(&iconeBackgroundColor);
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
    
    //BLANC //sert uniquement à intuitivement changer la couleur du background en blanc. actuellement, c'est le même bouton que la gomme.
    allButtons blanc("Col blanc", WINDOW_WIDTH - SET_ROW - BUTTON_SIZE, SET_ROW + BUTTON_GAP, sf::Color::White);
    //vectorButtons.push_back(blanc);

                        //LIGNE SEPARATRICE
    sf::VertexArray ligne(sf::Lines);
    ligne.append(sf::Vertex(sf::Vector2f(0, POS_LIGNE), sf::Color::Black));
    ligne.append(sf::Vertex(sf::Vector2f(WINDOW_WIDTH, POS_LIGNE), sf::Color::Black));

                        //BOUCLE PRINCIPALE
    while (window.isOpen()) {

        textImage.loadFromImage(image);
        spriteImage.setTexture(textImage);
        
        sf::Event event;
        sf::Mouse mouse;

                        //BOUCLE D'EVENEMENTS
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
            
                            //DETECTION DES CLIQUES SUR ICONES
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (souris.turnOnCheck(souris, mouse, window)) {
                        stylo.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false; backgroundColor.buttonState = false;
                    }
                    if (stylo.turnOnCheck(stylo, mouse, window)) {
                        souris.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false; backgroundColor.buttonState = false;
                    }
                    if (pinceau.turnOnCheck(pinceau, mouse, window)) {
                        souris.buttonState = false; stylo.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false; backgroundColor.buttonState = false;
                    } 
                    if (gomme.turnOnCheck(gomme, mouse, window)) {
                        souris.buttonState = false; stylo.buttonState = false; pinceau.buttonState = false; ecrire.buttonState = false; backgroundColor.buttonState = false;
                    }
                    if (ecrire.turnOnCheck(ecrire, mouse, window)) {
                        souris.buttonState = false; stylo.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; backgroundColor.buttonState = false;
                    }
                    if (backgroundColor.turnOnCheck(backgroundColor, mouse, window)) {
                        souris.buttonState = false; stylo.buttonState = false; pinceau.buttonState = false; gomme.buttonState = false; ecrire.buttonState = false;
                        rouge.buttonState = false; bleu.buttonState = false; vert.buttonState = false; noir.buttonState = false; rose.buttonState = false; blanc.buttonState = false;
                        //désactiver les couleurs en plus pour ne pas appliquer une couleur au background dès qu'on clique sur le seau
                    }

                    clearAll.turnOnCheck(clearAll, mouse, window);

                    if (pinceau.buttonState || gomme.buttonState) { //pour que le choix de la taille soit exclusif au pinceau et à la gomme
                        diminuerTaille.turnOnCheck(diminuerTaille, mouse, window);
                        augmenterTaille.turnOnCheck(augmenterTaille, mouse, window);
                    }
                    if (pinceau.buttonState || stylo.buttonState || backgroundColor.buttonState){
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
        if (stylo.buttonState) {
            if (mouse.getPosition(window).y >= POS_LIGNE - currentSizePinceau * 2) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    //image.setPixel(mouse.getPosition(window).x, mouse.getPosition(window).y, setColor);  anciennement pour setPixel sur une image
                    sf::CircleShape shape = sf::CircleShape(1);
                    shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius()); //centrer rond sur souris
                    shape.setFillColor(currentColor);
                    vectorDrawing.push_back(shape);
                    window.draw(shape);
                }
            } // pour ne pas dessiner sur le menu
            
        }
        if (pinceau.buttonState) {
            if (mouse.getPosition(window).y >= POS_LIGNE-currentSizePinceau * 2) { // pour ne pas dessiner sur le menu
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::CircleShape shape = sf::CircleShape((float)currentSizePinceau); //(float) c'est pour enlever avertissement
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
                    sf::CircleShape shape = sf::CircleShape((float)currentSizePinceau); //(float) c'est pour enlever avertissement
                    shape.setPosition(mouse.getPosition(window).x - shape.getRadius(), mouse.getPosition(window).y - shape.getRadius());
                    shape.setFillColor(sf::Color::White);
                    vectorDrawing.push_back(shape);
                    window.draw(shape);
                }
            }
        }
        if (backgroundColor.buttonState) {
            if (noir.buttonState) backgroundChoice = noir.getFillColor();
            if (rouge.buttonState) backgroundChoice = rouge.getFillColor();
            if (bleu.buttonState) backgroundChoice = bleu.getFillColor();
            if (vert.buttonState) backgroundChoice = vert.getFillColor();
            if (rose.buttonState) backgroundChoice = rose.getFillColor();
            if (blanc.buttonState) backgroundChoice = blanc.getFillColor();
        }
        //if (ecrire.buttonState) {
        //    if (mouse.getPosition(window).y > POS_LIGNE) { // écrire du texte uniquement sur le plan
        //        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        //            ecrire.buttonState = false; //pour avoir uniquement 1 button press, à priorit ça n'arrête pas le if
        //            ecrireActif = true;
        //        }
        //    }
        //}
        showActiveButton(souris);
        showActiveButton(stylo);
        showActiveButton(pinceau);
        showActiveButton(diminuerTaille);
        showActiveButton(augmenterTaille);
        showActiveButton(gomme);
        showActiveButton(clearAll); //manuellement car pas réussi à modifier l'outLine d'un objet qui se trouve dans un vecteur
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
        for (sf::CircleShape& shape : vectorDrawing) {
            window.draw(shape);
        }
        window.draw(spriteImage);//background du menu (pour éviter de dessiner dessus)
        window.draw(ligne); //la ligne qui sépare les boutons de l'espace de dessin

        window.draw(souris);
        window.draw(stylo);
        window.draw(pinceau);
        window.draw(gomme);
        window.draw(clearAll);
        window.draw(ecrire);
        window.draw(noir);
        window.draw(rouge);
        window.draw(bleu);
        window.draw(vert);
        window.draw(rose);
        window.draw(blanc);
        window.draw(backgroundColor);
        //for (allButtons &bouton : vectorButtons) {
        //    window.draw(bouton);//tous les boutons 
        //}//ça fonctionnait mais le fait que les boutons sont dans un vecteur m'empechait de changer leur outLine.

        if (pinceau.buttonState || gomme.buttonState) {//afficher le choix des tailles uniquement quand le pinceau est activé
            window.draw(diminuerTaille);
            window.draw(augmenterTaille);
        }
        window.display();
    }
    return 0;
}
/* En-tête du programme
=======================
Programme:      tutoGitsSFML.cpp
Auteur:			Jean-Alain Sainton & Olivier Lemay Dostie
Date création:	16/04/2017
Description:	Programme des essais SFML et de connection Git pour le projet final en C++
.				Espace de développement pour l'application.*/

/* Directives au pré-processeur.
==============================*/
//#include <SDL.h>			//Pas inclus
//#include <SDL_ttf.h>		//Pas inclus
//#include <saisieSDL.h>	//Non fonctionnel si pas inclus
#include <utile.h>
#include <date.h>
#include <rectangle.h>
#include <saisieSecurisee.h>
#include <pieces.h>			//Celui-ci

#include <locale>			
#include <string>					
#include <iostream>			
#include <iomanip>			
#include <stdlib.h>			//Pk déjà?
#include <assert.h>			
#include <vector>			
#include <math.h>			
#include <SFML\Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;


/*Prototypes des fonctions*/
/*========================*/
int saisie(RenderWindow &window, Music &music, string &nomJoueur, int &i);
void formePiece(RectangleShape forme[5][5], int profil[5][5], Vector2f pos);
int menuOption(RenderWindow &window, Music &music, string &nomJoueur);
int choixMenu(RenderWindow &windowMenu, RenderWindow &window, Music &music, string &nomJoueur);
void afficherMenu(RenderWindow &window, Music &music, string &nomJoueur);



/* Programme principal.
===================== */
int main()
{
	srand(time_t(NULL));		//Trouve une source aléatoire


	string nomJoueur = "Nouveau Joueur";
	setlocale(LC_CTYPE, "fra");

	Music music;
	music.stop();
	music.openFromFile("music3.ogg");


	RenderWindow window(VideoMode(1000, 800), "TETRIS Jeu");

	afficherMenu(window, music, nomJoueur);

	if (music.getStatus() == false)
		music.play();
	int i = 0;



	Texture texture;
	if (!texture.loadFromFile("Tetris-Background.jpg"));
	Sprite background(texture);
	window.draw(background);

	Time moment;
	unsigned seed = time(0);
	Clock tempsEcoule;

	/* Création d'une police */
	//Font font;
	//if (!font.loadFromFile("arial.ttf"))
	//	return 1;
	//Text FPS("0", font, 25);
	//FPS.setOrigin(-10, -10);
	//FPS.setColor(Color::Red);
	/* Test FPS à inclure dans la boucle */
	//static int cptImage = 0;
	//static int fps = 0;
	//cptImage++;
	//moment = tempsEcoule.getElapsedTime();
	//if (moment.asMilliseconds() > 999)
	//{
	//	fps = cptImage;
	//	cptImage = 0;
	//	tempsEcoule.restart();
	//}
	//stringstream ss;
	//ss << fps;
	//FPS.setString(ss.str());
	//window.draw(FPS);


	vector<Vector2i> occupations;
	bloc actif = TETRIS[rand() % (7) + 1];
	bloc prochain = TETRIS[rand() % (7) + 1];
	salle espace;
	espace.init(Vector2f(30, 30), 1, 1, occupations, "Joueur", 1, 0, 0, 1, TETRIS, actif, prochain);

	//Tests
	teStruct test;

	//Essais de blocs
	bloc active = espace.getBloc();
	int angle = active.getAngle();
	vector<Vector2i> profil;
	
	espace.getOccupation(occupations);

	Event event;


	int piece, pieceActive;

	piece = espace.prochain();

	pieceActive = piece;
	
	actif = espace.getBloc();

	window.display();
	

	do
	{
		prochain = espace.getProchains();
		
		int posy, posx;
		int nbY = 0;
		//coord debut;
		//debut.x = 205;
		//debut.y = 0;
		//posx = blocActif.getX();
		//posy = blocActif.getY();
		//blocActif.setPosX(debut);
		//blocActif.setPosY(debut);
		espace.afficherInterface(window);
		espace.modifierInterface(window, prochain, profil, nomJoueur);
		int mouvement;
		do {
			/*posx = blocActif.getX();
			posy = blocActif.getY();
*/
		
			if (!texture.loadFromFile("Tetris-Background.jpg"));
			Sprite background(texture);
			window.draw(background);


			espace.afficherInterface(window);
			espace.modifierInterface(window, prochain, profil, nomJoueur);
			active.drawBloc(window, active.getAngle());
			
			/*blocActif.getProfil(profil);
			formePiece(formeActif, profil, blocActif.getPlace());*/ 
		
			mouvement = saisie(window, music, nomJoueur, i);

			switch (mouvement)
			{
			case 2:
				active.setPosY(1);

				break;
			case 4:
				active.setPosX(-1);
				break;
			case 5:
				active.setPosX(1);
				break;
			default:
				break;
			}
 
			nbY++;
			window.display();
			sleep(seconds(0));
		} while (nbY < 18);

		/*RectangleShape piecePlacer;
		piecePlacer.setSize(Vector2f(600, 763));
		piecePlacer.setFillColor(Color::Transparent);
		piecePlacer.setOutlineThickness(0);
		piecePlacer.setOutlineColor(Color::Red);
		piecePlacer.setPosition(20,20);
		window.draw(piecePlacer);*/
		/*window.display();*/

		//??
		//blocActif.setId(blocSuivant.getId());
		//blocActif.getProfil(profil);
		//posActif2.x = -3;;
		//piece = blocSuivant.getId();
		//blocActif.setPosX(debut);
		//blocActif.setPosY(debut);
		//formePiece(pieceSuivante, profil, blocActif.getPlace());
		//window.display();
	} while (true);
	return 0;

	
}


/*Fonctions*/
/*=========*/
// Première fonction.
int saisie(RenderWindow &window, Music &music, string &nomJoueur, int &i)
{
	sf::Event event;
	//A B C D E F G H I J K L M N O P Q R S T U V W X Y Z .20-46								26
	//Num0 Num1 Num2 Num3 Num4 Num5 Num6 Num7 Num8 Num9 .0-9									10
	//Numpad0 Numpad1 Numpad2 Numpad3 Numpad4 Numpad5 Numpad6 Numpad7 Numpad8 Numpad9 .10-19	10
	//LControl RControl LShift RShift LAlt RAlt LSystem RSystem .47-56							9
	//LBracket RBracket Slash BackSlash //LParentesis RParentesis .58-							9
	//Equal Dash Space Return BackSpace Tab PageUp PageDown End Home Insert Delete				12
	//Add Substract Multiply Divide Left Right Up Down											8
	//F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12 F13 F14 F15 Pause KeyCount (toujours 102?)			15+2
	//
	////Tilde SemiColon Comma Period Quote Unknown .-1 
	////• $ € ¥ £ & % # @ ! ? _ < > é à ç ï ù ë (MAJ?) 
	////© ® ™ | ' " ` ° ¤ ¬ µ ` ¨ ^ ∞ × ÷ ≥ ≤ ≠ ± 
	////Menu Play Pause2 Previous Next PrtSc Fn Window (System?) CapsLock
	////Mute Sound- Sound+ MuteMic Refresh MousePadLock PlaneMode CameraLock
	////Lock Display Contrast- Contrast+ 

	while (window.pollEvent(event))
		// check the type of the event...
		switch (event.type)
		{
			// window closed
		case Event::Closed:
		{
			window.close();
			break;
		}
		// key pressed
		case Event::KeyPressed:
			if (Keyboard::isKeyPressed(Keyboard::Unknown))
			{

			}
			else if (event.key.code == Keyboard::Escape)
			{

				afficherMenu(window, music, nomJoueur);
			}
			else if (event.key.code == Keyboard::Z)
			{
				return 6;
			}
			else if (event.key.code == Keyboard::Down)
			{
				return 2;
			}
			else if (event.key.code == Keyboard::X || event.key.code == Keyboard::Up)
			{
				return 7;
			}
			else if (event.key.code == Keyboard::Left)
			{
				return 4;
			}
			else if (event.key.code == Keyboard::Right)
			{
				return 5;
			}
			else if (event.key.code == Keyboard::Pause)
			{
				//Pause
				//Afficher menu
			}
			else if (event.key.code == Keyboard::M)
			{
				//Met la musique sur stop
				music.setVolume(0);
			}
			else if (event.key.code == Keyboard::Q)
			{
				//Met la musique sur stop
				music.setVolume(75);
			}
			else if (event.key.code == Keyboard::T)
			{
				music.stop();
				//Change la musique 
				i++;
				switch (i)
				{
				case 1:
					if (music.openFromFile("music.ogg"))
						music.play();
					;
					break;
				case 2:
					if (music.openFromFile("music2.ogg"))
						music.play();
					;
					break;
				case 3:
					if (music.openFromFile("music3.ogg"))
						music.play();
					i = 0;
					break;
				default:
					break;
					// we don't process other types of events

				}
			}
		}
	return 1;
}

<<<<<<< HEAD
	void load()
	{}
	void demare()
	{}
	void pause()
	{}
	void menu()
	{}
	int prochain()
	{
		srand(time(NULL));
		int numBlock;
		numBlock = rand() % 6;
		return numBlock;
	}
	void balaye()
	{}
	void tetris()
	{}
	void compresse()
	{}
	void ferme()
	{}
	void creeObstacle()		//**
	{}
	void marcheArriere()	//**
	{}
	void tourne()			//**
	{}
	void brasse()			//**
	{}
};

//class indice
//{
//private:
//	int _style;
//public:
//	indice();
//	~indice();
//
//};
//
//indice::indice()
//{
//}
//
//indice::~indice()
//{
//}


//Prototypes des fonctions

void formePiece(sf::RectangleShape forme[5][5], int profil[5][5], coord pos)
=======
void formePiece(RectangleShape forme[5][5], int profil[5][5], Vector2f pos)
>>>>>>> refs/remotes/origin/master
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (profil[i][j] == 1)
			{
				forme[i][j].setSize(Vector2f(24, 24));
				forme[i][j].setPosition(pos.x + i * 26, pos.y + j * 26);
				forme[i][j].setFillColor(Color::Green);
				forme[i][j].setOutlineThickness(1);
				forme[i][j].setOutlineColor(Color(0, 0, 0));
			}
		}
	}
}

int menuOption(RenderWindow &window, Music &music, string &nomJoueur)
{
	window.clear(Color::Black);
	window.display();
	Texture texture;
	RectangleShape btnMusique(Vector2f(300, 100));
	RectangleShape btnMusiquePowerOn(Vector2f(50, 50));
	RectangleShape btnMusiquePowerOff(Vector2f(50, 50));
	RectangleShape btnMusiqueChoix(Vector2f(200, 50));
	RectangleShape btnMenu(Vector2f(100, 50));
	RectangleShape btnNomJoueur(Vector2f(300, 125));
	RectangleShape btnAutre(Vector2f(300, 100));
	Font font;
	Text text;
	if (!texture.loadFromFile("Tetris-Background.jpg"));
	Sprite background(texture);
	window.draw(background);
	font.loadFromFile("font_arial.ttf"); // choix de la police à utiliser
	text.setFont(font);
	text.setCharacterSize(30); // choix de la taille des caractères exprimée en pixels, pas en points !					
	text.setStyle(Text::Bold); 	// choix du style du texte
	btnMusique.setFillColor(Color::White);
	btnMusique.setOutlineThickness(0);
	btnMusique.setOutlineColor(Color::Red);
	btnMusique.setPosition(350, 150);
	if (!texture.loadFromFile("button.png", IntRect(50, 25, 300, 100)));
	btnMusique.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Musique"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(450, 188);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	window.draw(btnMusique);
	window.draw(text);
	btnMusiquePowerOn.setFillColor(Color::Magenta);
	btnMusiquePowerOn.setOutlineThickness(0);
	btnMusiquePowerOn.setOutlineColor(Color::Red);
	btnMusiquePowerOn.setPosition(350, 275);
	/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
	btnMusiquePowerOn.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("On"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(352, 278);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	window.draw(btnMusiquePowerOn);
	window.draw(text);
	btnMusiquePowerOff.setFillColor(Color::White);
	btnMusiquePowerOff.setOutlineThickness(0);
	btnMusiquePowerOff.setOutlineColor(Color::Red);
	btnMusiquePowerOff.setPosition(600, 275);
	//if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));
	btnMusiquePowerOff.setTexture(&texture);
	text.setString("Off"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(605, 278);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	window.draw(btnMusiquePowerOff);
	window.draw(text);
	btnMusiqueChoix.setFillColor(Color::White);
	btnMusiqueChoix.setOutlineThickness(0);
	btnMusiqueChoix.setOutlineColor(Color::Red);
	btnMusiqueChoix.setPosition(350, 355);
	/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
	btnMusiqueChoix.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 362);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	window.draw(btnMusiqueChoix);
	window.draw(text);
	btnMusiqueChoix.setFillColor(Color::White);
	btnMusiqueChoix.setOutlineThickness(0);
	btnMusiqueChoix.setOutlineColor(Color::Red);
	btnMusiqueChoix.setPosition(350, 420);
	/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
	btnMusiqueChoix.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 422);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	window.draw(btnMusiqueChoix);
	window.draw(text);
	btnMusiqueChoix.setFillColor(Color::White);
	btnMusiqueChoix.setOutlineThickness(0);
	btnMusiqueChoix.setOutlineColor(Color::Red);
	btnMusiqueChoix.setPosition(350, 485);
	/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
	btnMusiqueChoix.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 487);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	window.draw(btnMusiqueChoix);
	window.draw(text);
	btnMenu.setFillColor(Color::White);
	btnMenu.setOutlineThickness(0);
	btnMenu.setOutlineColor(Color::Red);
	btnMenu.setPosition(10, 10);
	/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
	btnMenu.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Menu"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(15, 15);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	window.draw(btnMenu);
	window.draw(text);
	btnNomJoueur.setFillColor(Color::White);
	btnNomJoueur.setOutlineThickness(0);
	btnNomJoueur.setOutlineColor(Color::Red);
	btnNomJoueur.setPosition(350, 550);
	//if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));
	btnNomJoueur.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 565);		// position du texte
	window.draw(btnNomJoueur);
	window.draw(text);



	text.setString(nomJoueur); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 625);		// position du texte
	window.draw(text);
	window.display();
	font.loadFromFile("font_arial.ttf");
	text.setFont(font);

	Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{

			switch (event.type)
			{
			case Event::Closed:
				window.close();
				return -1;
				break;
			case Event::MouseButtonPressed:
				Mouse::getPosition(window);
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 400) && (event.mouseButton.y > 275 && event.mouseButton.y < 325))
				{
					//Option musique on
					window.clear(Color::Black);
					window.display();
					music.setVolume(100);
					btnMusiquePowerOn.setFillColor(Color::Magenta);
					btnMusiquePowerOn.setOutlineThickness(0);
					btnMusiquePowerOn.setOutlineColor(Color::Red);
					btnMusiquePowerOn.setPosition(350, 275);
					/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
					btnMusiquePowerOn.setTexture(&texture);
					texture.setSmooth(true);
					text.setString("On"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(352, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiquePowerOn);
					window.draw(text);
					btnMusiquePowerOff.setFillColor(Color::White);
					btnMusiquePowerOff.setOutlineThickness(0);
					btnMusiquePowerOff.setOutlineColor(Color::Red);
					btnMusiquePowerOff.setPosition(600, 275);
					/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
					btnMusiquePowerOff.setTexture(&texture);
					text.setString("Off"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(605, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiquePowerOff);
					window.draw(text);
					window.display();
				}
				else if ((event.mouseButton.x > 600 && event.mouseButton.x < 650) && (event.mouseButton.y > 275 && event.mouseButton.y < 325))
				{
					window.clear(Color::Black);
					window.display();
					music.setVolume(0);
					btnMusiquePowerOn.setFillColor(Color::White);
					btnMusiquePowerOn.setOutlineThickness(0);
					btnMusiquePowerOn.setOutlineColor(Color::Red);
					btnMusiquePowerOn.setPosition(350, 275);
					/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
					btnMusiquePowerOn.setTexture(&texture);
					texture.setSmooth(true);
					text.setString("On"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(352, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiquePowerOn);
					window.draw(text);
					btnMusiquePowerOff.setFillColor(Color::Magenta);
					btnMusiquePowerOff.setOutlineThickness(0);
					btnMusiquePowerOff.setOutlineColor(Color::Red);
					btnMusiquePowerOff.setPosition(600, 275);
					/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
					btnMusiquePowerOff.setTexture(&texture);
					text.setString("Off"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(605, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiquePowerOff);
					window.draw(text);
					window.display();
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 355 && event.mouseButton.y < 405))
				{
					window.clear(Color::Black);
					window.display();
					music.openFromFile("music.ogg");
					music.play();
					btnMusiqueChoix.setPosition(350, 355);
					btnMusiqueChoix.setFillColor(Color::Magenta);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);
					btnMusiqueChoix.setPosition(350, 420);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);
					btnMusiqueChoix.setPosition(350, 485);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);
					window.display();
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 420 && event.mouseButton.y < 470))
				{
					window.clear(Color::Black);
					window.display();
					music.openFromFile("music2.ogg");
					music.play();
					btnMusiqueChoix.setPosition(350, 355);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);
					btnMusiqueChoix.setPosition(350, 420);
					btnMusiqueChoix.setFillColor(Color::Magenta);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);
					btnMusiqueChoix.setPosition(350, 485);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);
					window.display();
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 485 && event.mouseButton.y < 535))
				{
					window.clear(Color::Black);
					window.display();
					music.openFromFile("music3.ogg");
					music.play();
					btnMusiqueChoix.setPosition(350, 355);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);
					btnMusiqueChoix.setPosition(350, 420);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);
					btnMusiqueChoix.setPosition(350, 485);
					btnMusiqueChoix.setFillColor(Color::Magenta);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);
					window.display();
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 550 && event.mouseButton.y < 700))
				{
					RenderWindow window2(VideoMode(500, 500), "TETRIS v1.2 Nom du joueur");
					font.loadFromFile("font_arial.ttf");
					if (!texture.loadFromFile("Tetris-Background_2.jpg"));
					Sprite background(texture);
					window2.draw(background);
					font.loadFromFile("font_arial.ttf");
					text.setFont(font);
					text.setString("Quel est votre nom?"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(100, 15);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					window2.draw(text);
					window2.display();
					std::string s;
					while (window2.isOpen())
					{
						//Event processing.
						Event event;
						while (window2.pollEvent(event)) {
							if (event.type == Event::Closed)
								window2.close();
							if (event.type == Event::TextEntered) {
								if (event.key.code == 8 && s.size() != 0)
								{
									s.pop_back();
								}
								else if (event.key.code == 13 && s.size() != 0)
								{
									nomJoueur = s;
									window2.close();
								}
								else if (event.text.unicode < 128)
								{
									s.push_back((char)event.text.unicode);
								}
								btnMusiqueChoix.setPosition(100, 50);
								btnMusiqueChoix.setFillColor(Color::White);
								btnMusiqueChoix.setOutlineColor(Color::Transparent);
								text.setString(s); 	// choix de la chaîne de caractères à afficher
								text.setPosition(100, 50);		// position du texte
								text.setColor(Color::Black);   // choix de la couleur du texte
								window2.display();
								window2.draw(btnMusiqueChoix);
								window2.draw(text);
								window2.display();
							}
						}
					}
					if (!texture.loadFromFile("button.png", IntRect(50, 25, 300, 100)));
					btnNomJoueur.setFillColor(Color::White);
					btnNomJoueur.setOutlineThickness(0);
					btnNomJoueur.setOutlineColor(Color::Red);
					btnNomJoueur.setPosition(350, 550);
					btnNomJoueur.setTexture(&texture);
					texture.setSmooth(true);
					font.loadFromFile("font_arial.ttf");
					text.setFont(font);
					text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 565);		// position du texte
					window.display();
					window.draw(btnNomJoueur);
					window.draw(text);
					window.display();
					text.setString(nomJoueur); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 625);		// position du texte
					window.display();
					window.draw(text);
					window.display();
				}
				else if ((event.mouseButton.x > 15 && event.mouseButton.x < 105) && (event.mouseButton.y > 15 && event.mouseButton.y < 65))
				{
					return 0;
				}
				break;
			default:
				break;
			}
		}
	}
}


int choixMenu(RenderWindow &windowMenu, RenderWindow &window, Music &music, string &nomJoueur)
{
	Event event;
	while (windowMenu.isOpen())
	{
		while (windowMenu.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				windowMenu.close();
				return -1;
				break;
			case Event::MouseButtonPressed:
				Mouse::getPosition(windowMenu);
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 150 && event.mouseButton.y < 250))
				{
					return 1;
				}
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 300 && event.mouseButton.y < 400))
				{
					menuOption(windowMenu, music, nomJoueur);
					return 0;
				}
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 600 && event.mouseButton.y < 700))
				{
				
					window.close();
					windowMenu.close();
					exit(0);
					return -1;
				}
				break;
			default:
				break;
			}
		}
	}
}

void afficherMenu(RenderWindow &window, Music &music, string &nomJoueur)
{
	window.setVisible(false);
	RenderWindow windowMenu(VideoMode(1000, 800), "TETRIS Menu");

	Texture texture;
	RectangleShape btnCommencer(Vector2f(300, 100));
	RectangleShape btnOption(Vector2f(300, 100));
	RectangleShape btnScore(Vector2f(300, 100));
	RectangleShape btnQuitter(Vector2f(300, 100));
	Font font;
	font.loadFromFile("font_arial.ttf"); // choix de la police à utiliser
	Text text;


	if (!texture.loadFromFile("Tetris-Background.jpg"));
	Sprite background(texture);

	do
	{
		if (!texture.loadFromFile("Tetris-Background.jpg"));
		windowMenu.draw(background);


		text.setFont(font);


		text.setCharacterSize(60);		// choix de la taille des caractères exprimée en pixels, pas en points !					
		text.setStyle(Text::Bold); 		// choix du style du texte
		text.setString("T"); 			// choix de la chaîne de caractères à afficher
		text.setPosition(350, 10);		// position du texte
		text.setColor(Color::Cyan);		// choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("E"); 			// choix de la chaîne de caractères à afficher
		text.setPosition(385, 10);		// position du texte
		text.setColor(Color::Red);		// choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("T"); 			// choix de la chaîne de caractères à afficher
		text.setPosition(420, 10);		// position du texte
		text.setColor(Color::Green);	// choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("R"); 			// choix de la chaîne de caractères à afficher
		text.setPosition(455, 10);		// position du texte
		text.setColor(Color::Cyan);		// choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("I"); 			// choix de la chaîne de caractères à afficher
		text.setPosition(490, 10);		// position du texte
		text.setColor(Color::White);	// choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("S"); 			// choix de la chaîne de caractères à afficher
		text.setPosition(500, 10);		// position du texte
		text.setColor(Color::Yellow);   // choix de la couleur du texte
		windowMenu.draw(text);

		text.setColor(Color::Black);	// choix de la couleur du texte
		text.setCharacterSize(30);		// choix de la taille des caractères exprimée en pixels, pas en points !			
		btnCommencer.setFillColor(Color::White);
		btnCommencer.setOutlineThickness(5);
		btnCommencer.setOutlineColor(Color::Red);
		btnCommencer.setPosition(350, 150);
<<<<<<< HEAD

		if (!texture.loadFromFile("Tetris-Background_2.jpg", sf::IntRect(0, 0, 200, 100)));
=======
		if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 200, 100)));
>>>>>>> refs/remotes/origin/master
		btnCommencer.setTexture(&texture);
		texture.setSmooth(true);
		windowMenu.draw(btnCommencer);
		text.setString("Play :D"); 		// choix de la chaîne de caractères à afficher
		text.setPosition(450, 188);		// position du texte
		windowMenu.draw(text);
		btnOption.setFillColor(Color::White);
		btnOption.setOutlineThickness(5);
		btnOption.setOutlineColor(Color::Red);
		btnOption.setPosition(350, 300);
		texture.~Texture();
<<<<<<< HEAD
		if (!texture.loadFromFile("Tetris-Background_2.jpg", sf::IntRect(0, 100, 200, 100)));

=======
		if (!texture.loadFromFile("image.jpg", IntRect(200, 500, 300, 100)));
>>>>>>> refs/remotes/origin/master
		btnOption.setTexture(&texture);
		texture.setSmooth(true);
		windowMenu.draw(btnOption);
		text.setString("Options "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 333);		// position du texte
		windowMenu.draw(text);
		btnScore.setFillColor(Color::White);
		btnScore.setOutlineThickness(5);
		btnScore.setOutlineColor(Color::Red);
		btnScore.setPosition(350, 450);
<<<<<<< HEAD
	
		if (!texture.loadFromFile("Tetris-Background_2.jpg", sf::IntRect(0, 200, 200, 100)));
=======
>>>>>>> refs/remotes/origin/master

		if (!texture.loadFromFile("image.jpg", IntRect(300, 300, 300, 100)));
		btnScore.setTexture(&texture);
		texture.setSmooth(true);
		windowMenu.draw(btnScore);
		text.setString("Scores ");		// choix de la chaîne de caractères à afficher
		text.setPosition(450, 488);		// position du texte
		windowMenu.draw(text);
		btnQuitter.setFillColor(Color::White);
		btnQuitter.setOutlineThickness(5);
		btnQuitter.setOutlineColor(Color::Red);
		btnQuitter.setPosition(350, 600);
<<<<<<< HEAD

		if (!texture.loadFromFile("Tetris-Background_2.jpg", sf::IntRect(0, 300, 200, 100)));

=======
		if (!texture.loadFromFile("image.jpg", IntRect(100, 500, 300, 100)));
>>>>>>> refs/remotes/origin/master
		btnQuitter.setTexture(&texture);
		texture.setSmooth(true);
		windowMenu.draw(btnQuitter);
		text.setString("Quitter "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 625);		// position du texte
		windowMenu.draw(text);
		windowMenu.display();
		// check the type of the event...
	} while (choixMenu(windowMenu, window, music, nomJoueur) != 1 && (choixMenu(windowMenu, window, music, nomJoueur) == -1));

<<<<<<< HEAD
	salle espace;

	bloc l(2, 2, 30, 30, 1, 1, 1, 1, 1, 1, blocL);
	bloc t(2, 2, 30, 30, 2, 1, 1, 1, 1, 1, blocT);
	bloc Carrer(2, 2, 30, 30, 3, 1, 1, 1, 1, 1, blocCarrer);
	bloc ligne(2, 2, 30, 30, 3, 1, 1, 1, 1, 1, blocLigne);
	bloc lInverser(2, 2, 30, 30, 4, 1, 1, 1, 1, 1, blocLInverser);
	bloc Z(2, 2, 30, 30, 5, 1, 1, 1, 1, 1, blocZ);
	bloc ZInverser(2, 2, 30, 30, 6, 1, 1, 1, 1, 1, blocZInverser);
	bloc pieces[7] = { l,l,l,l,l,l,l };
	bloc blocActif = { 2, 2, 30, 30, 1, 1, 1, 1, 1, 1, blocL };
	bloc blocSuivant = { 2, 2, 30, 30, 1, 1, 1, 1, 1, 1, blocL };
	string nomJoueur = "Nouveau Joueur";
	setlocale(LC_CTYPE, "fra");

	sf::RenderWindow window(sf::VideoMode(1000, 800), "TETRIS v1.2");


	//Tests
	teStruct test;

	//Essais de blocs
	sf::Music music;
	music.stop();
	music.openFromFile("music3.ogg");


	afficherMenu( music, nomJoueur);

	if (music.getStatus() == false)
		music.play();

	int i = 0;

	sf::RectangleShape formeL[5][5];
	sf::RectangleShape formeT[5][5];
	sf::RectangleShape formeLInverser[5][5];
	sf::RectangleShape formeZ[5][5];
	sf::RectangleShape formeZInverser[5][5];
	sf::RectangleShape formeLigne[5][5];
	sf::RectangleShape formeCarrer[5][5];
	sf::RectangleShape formeActif[5][5];
	sf::RectangleShape piecePlacer;
	sf::RectangleShape pieceSuivante[5][5];
	int profil[5][5];

	int prochainePiece;
	int piece ;
	int pieceActive;

	coord posL2 = l.getPlace();
	coord posT2 = t.getPlace();
	coord posLInverser2 = lInverser.getPlace();
	coord posZ2 = Z.getPlace();
	coord posZinverser2 = ZInverser.getPlace();
	coord posLigne2 = ligne.getPlace();
	coord posCarrer2 = Carrer.getPlace();
	coord posActif2 = blocActif.getPlace();
	coord posSuivant2 = blocSuivant.getPlace();

	//bloque de -2 ou -1 à 2

	espace.getOccupation(occupations);


	while (window.isOpen()) {
		
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		sf::Texture texture;
		if (!texture.loadFromFile("Tetris-Background.jpg"));
		sf::Sprite background(texture);
		window.draw(background);
		piece = espace.prochain();
		
		pieceActive = piece;

		switch (piece)
		{
		case 1:
			l.getProfil(profil);
			posL2.x -= 3;
			espace.setOccupation(profil, posL2);
			formePiece(formeL, profil, l.getPlace());

			blocActif.setId(1);
			blocActif.setAxes(blocL);

			
			break;

		case 2:
			t.getProfil(profil);


			posT2.x -= 3;
			espace.setOccupation(profil, posT2);
			formePiece(formeT, profil, t.getPlace());
			blocActif.setId(2);
			blocActif.setAxes(blocT);

			
			break;
		case 3:
			Carrer.getProfil(profil);
			posCarrer2.x -= 3;
			espace.setOccupation(profil, posCarrer2);
			formePiece(formeCarrer, profil, Carrer.getPlace());
			blocActif.setId(3);
			blocActif.setAxes(blocCarrer);

			
			break;
		case 4:
			ligne.getProfil(profil);


			posLigne2.x -= 3;
			espace.setOccupation(profil, posLigne2);
			formePiece(formeLigne, profil, ligne.getPlace());
			blocActif.setId(4);
			blocActif.setAxes(blocLigne);

			
			break;
		case 5:
			lInverser.getProfil(profil);


			posLInverser2.x -= 3;
			espace.setOccupation(profil, posLInverser2);
			formePiece(formeLInverser, profil, lInverser.getPlace());
			blocActif.setId(5);
			blocActif.setAxes(blocLInverser);

			
			break;
		case 6:
			Z.getProfil(profil);

			posZ2.x -= 3;
			espace.setOccupation(profil, posZ2);
			formePiece(formeZ, profil, Z.getPlace());
			blocActif.setId(6);
			blocActif.setAxes(blocZ);

			
			break;
		case 7:
			ZInverser.getProfil(profil);
			posZinverser2.x -= 3;
			espace.setOccupation(profil, posZinverser2);
			formePiece(formeZInverser, profil, ZInverser.getPlace());
			blocActif.setId(7);
			blocActif.setAxes(blocZInverser);

			
			break;
		default:
			break;
		}
		/*t.getProfil(profil);*/
		espace.afficherInterface(window);
		blocActif.getProfil(profil);
		posActif2.x = -3;
		espace.setOccupation(profil, posActif2);
		formePiece(formeActif, profil, blocActif.getPlace());
		window.display();
		
		do
		{
			prochainePiece = espace.prochain();
			while (prochainePiece == piece)
			{
				prochainePiece = espace.prochain();
			}
			switch (prochainePiece)
			{
			case 1:
				l.getProfil(profil);
				blocSuivant.setId(1);
				blocSuivant.setAxes(blocL);
				formePiece(pieceSuivante, profil, l.getPlace());
				espace.modifierInterface(window, formeL, profil, nomJoueur);
				
				break;
			case 2:
				t.getProfil(profil);
				blocSuivant.setId(2);
				blocSuivant.setAxes(blocT);
				formePiece(pieceSuivante, profil, t.getPlace());
				espace.modifierInterface(window, formeT, profil, nomJoueur);
				
				break;
			case 3:
				Carrer.getProfil(profil);
				blocSuivant.setId(3);
				blocSuivant.setAxes(blocCarrer);
				formePiece(pieceSuivante, profil, Carrer.getPlace());
				espace.modifierInterface(window, formeCarrer, profil, nomJoueur);
				
				break;
			case 4:
				ligne.getProfil(profil);
				blocSuivant.setId(4);
				blocSuivant.setAxes(blocLigne);
				formePiece(pieceSuivante, profil, ligne.getPlace());
				espace.modifierInterface(window, formeLigne, profil, nomJoueur);
				
				break;
			case 5:
				lInverser.getProfil(profil);
				blocSuivant.setId(5);
				blocSuivant.setAxes(blocLInverser);
				formePiece(pieceSuivante, profil, lInverser.getPlace());
				espace.modifierInterface(window, formeLInverser, profil, nomJoueur);
				
				break;
			case 6:
				Z.getProfil(profil);
				blocSuivant.setId(6);
				blocSuivant.setAxes(blocZ);
				formePiece(pieceSuivante, profil, Z.getPlace());
				espace.modifierInterface(window, formeZ, profil, nomJoueur);
				
				break;
			case 7:
				ZInverser.getProfil(profil);
				formePiece(pieceSuivante, profil, ZInverser.getPlace());
				espace.modifierInterface(window, formeZInverser, profil, nomJoueur);
				blocSuivant.setId(7);
				blocSuivant.setAxes(blocZInverser);
			
				
				break;
			default:
				break;
			}
			espace.modifierInterface(window, pieceSuivante, profil, nomJoueur);
			
			posSuivant2.x -= 3;
			

			window.display();
			

			//testPackPlay(test, window);

					//l.tourneGauche(occupations);
					//l.getProfil(profil);

			int posy, posx;
			int nbY = 0;
			coord debut;
			debut.x = 205;
			debut.y = 0;
			posx = blocActif.getX();
			posy = blocActif.getY();
			blocActif.setPosX(debut);
			blocActif.setPosY(debut);

			espace.afficherInterface(window);
			espace.modifierInterface(window, pieceSuivante, profil, nomJoueur);
			window.display();
			do {
				posx = blocActif.getX();
				posy = blocActif.getY();
				
				saisie(window, l, occupations, music, i, nomJoueur);
				if (!texture.loadFromFile("Tetris-Background.jpg"));
				sf::Sprite background(texture);
				window.draw(background);
				

				blocActif.getProfil(profil);
				formePiece(formeActif, profil, blocActif.getPlace());
				espace.afficherInterface(window);
				espace.modifierInterface(window, pieceSuivante, profil, nomJoueur);
			
				drawPiece(window, formeActif, profil);
				formePiece(formeActif, profil, blocActif.getPlace());

				
			
			
				blocActif.bougeY(posy, posx, occupations);
			
				
			
				nbY++;
				window.display();
				sf::sleep(sf::seconds(0.2));

=======
	window.setVisible(true);


>>>>>>> refs/remotes/origin/master


}

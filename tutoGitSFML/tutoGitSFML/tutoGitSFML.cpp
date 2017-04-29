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
int saisie(RenderWindow &window, Music &music, Music &music2, Music &music3, string &nomJoueur, int &i);
void formePiece(RectangleShape forme[5][5], int profil[5][5], Vector2f pos);
int menuOption(RenderWindow &window, Music &music, Music &music2, Music &music3, string &nomJoueur);
int choixMenu(RenderWindow &windowMenu, RenderWindow &window, Music &music, Music &music2, Music &music3, string &nomJoueur);
void afficherMenu(RenderWindow &window, Music &music, Music &music2, Music &music3, string &nomJoueur, Texture &texture, Font &font);

void titreTetris(RenderWindow &windowMenu, Font font, float posx, float posy, float size);

/* Programme principal.
===================== */
int main()
{
	Music music2;
	Music music3;
	srand(time_t(NULL));		//Trouve une source aléatoire

	string nomJoueur = "Nouveau Joueur";
	setlocale(LC_CTYPE, "fra");

	Music music;
	music.stop();
	music.openFromFile("music3.ogg");

	Texture texture;
	if (!texture.loadFromFile("Tetris-Background.jpg"));
	Sprite background(texture);
	Font font;
	if (!font.loadFromFile("font_arial.ttf")); // choix de la police à utiliser
	int i = 0;

	RenderWindow window(VideoMode(1000, 800), "TETRIS Jeu");

	afficherMenu(window, music, music2, music3, nomJoueur, texture, font);

	if (music.getStatus() == false)
			music.play();





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

	int lvl = 1;
	int score = 0;

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
		espace.modifierInterface(window, prochain, profil, nomJoueur, lvl, score);

		int mouvement = 0;

		do {


			/*posx = blocActif.getX();
			posy = blocActif.getY();
			*/


			Sprite background(texture);
			window.draw(background);


			espace.afficherInterface(window);
			espace.modifierInterface(window, prochain, profil, nomJoueur, lvl, score);
			active.drawBloc(window, active.getAngle());

			/*blocActif.getProfil(profil);
			formePiece(formeActif, profil, blocActif.getPlace());*/


			mouvement = saisie(window, music,music2, music3, nomJoueur, i);
			titreTetris(window, font, 150, 20, 60);
			nbY++;
			window.display();

			switch (mouvement)
			{
			case 1:
				/*if (!texture.loadFromFile("Tetris-Background.jpg"));
				if (!font.loadFromFile("font_arial.ttf"));*/
				afficherMenu(window, music, music2, music3, nomJoueur, texture, font);

				break;
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
int saisie(RenderWindow &window, Music &music, Music &music2, Music &music3, string &nomJoueur, int &i)
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
			exit(0);
		}
		// key pressed
		case Event::KeyPressed:
			if (Keyboard::isKeyPressed(Keyboard::Unknown))
			{

			}

			else if (event.key.code == Keyboard::Escape)
			{
				return 1;

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
			else if (event.key.code == Keyboard::Pause || event.key.code == Keyboard::P)
			{
				//Pause
				menuOption( window, music, music2,music3, nomJoueur);
			}
			else if (event.key.code == Keyboard::M)
			{
				//Met la musique sur stop
				music.setVolume(0);
				music2.setVolume(0);
				music3.setVolume(0);
			}
			else if (event.key.code == Keyboard::Q)
			{
				//Met la musique sur stop
				music.setVolume(75);
				music2.setVolume(75);
				music3.setVolume(75);
			}
			else if (event.key.code == Keyboard::T)
			{

				
				if (music.getStatus() == sf::Music::Playing)
				{
					music.stop();
					music2.openFromFile("music2.ogg");
					music2.play();
				}
				else if (music2.getStatus() == sf::Music::Playing)
				{
					music2.stop();
					music3.openFromFile("music3.ogg");
					music3.play();

				}
				else if (music3.getStatus() == sf::Music::Playing)
				{
					music3.stop();
					music.openFromFile("music.ogg");
					music.play();
				}
				else
				{
					music.openFromFile("music.ogg");
					music.play();
				}

				
			}
		}
	return 0;
}

void formePiece(RectangleShape forme[5][5], int profil[5][5], Vector2f pos)
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

int menuOption(RenderWindow &window, Music &music, Music &music2, Music &music3, string &nomJoueur)
{
	window.setVisible(false);
	window.clear();
	RenderWindow windowOption(VideoMode(1000, 800), "TETRIS Option");
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


	btnMusique.setFillColor(Color::White);
	btnMusiquePowerOn.setFillColor(Color::White);
	btnMusiquePowerOff.setFillColor(Color::White);
	btnMusiqueChoix.setFillColor(Color::White);
	btnNomJoueur.setFillColor(Color::White);



	if (!texture.loadFromFile("Tetris-Background.jpg"));
	Sprite background(texture);
	windowOption.draw(background);
	if (!font.loadFromFile("font_arial.ttf")); // choix de la police à utiliser
	text.setFont(font);
	text.setCharacterSize(30); // choix de la taille des caractères exprimée en pixels, pas en points !					
	text.setStyle(Text::Bold); 	// choix du style du texte
/*	btnMusique.setFillColor(Color::White);*/
	btnMusique.setOutlineThickness(5);
	btnMusique.setOutlineColor(Color::Red);
	btnMusique.setPosition(350, 150);
	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(0, 190, 300, 100)));
	btnMusique.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Musique"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(450, 188);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	windowOption.draw(btnMusique);
	windowOption.draw(text);

	btnMusiquePowerOn.setFillColor(Color::Magenta);
	btnMusiquePowerOn.setOutlineThickness(5);
	btnMusiquePowerOn.setOutlineColor(Color::Red);
	btnMusiquePowerOn.setPosition(350, 275);
	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(150, 150, 50, 50)));
	btnMusiquePowerOn.setTexture(&texture);

	if (music.getVolume() == 0)
	{
		btnMusiquePowerOn.setFillColor(Color::White);
	}
	texture.setSmooth(true);
	text.setString("On"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(352, 278);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	windowOption.draw(btnMusiquePowerOn);
	windowOption.draw(text);

	//btnMusiquePowerOff.setFillColor(Color::White);
	btnMusiquePowerOff.setOutlineThickness(5);
	btnMusiquePowerOff.setOutlineColor(Color::Red);
	btnMusiquePowerOff.setPosition(600, 275);
	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(150, 150, 50, 50)));
	texture.setSmooth(true);
	btnMusiquePowerOff.setTexture(&texture);
	if (music.getVolume() == 0)
	{
		btnMusiquePowerOff.setFillColor(Color::Magenta);
	}
	text.setString("Off"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(605, 278);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	windowOption.draw(btnMusiquePowerOff);
	windowOption.draw(text);

	/*btnMusiqueChoix.setFillColor(Color::White);*/
	btnMusiqueChoix.setOutlineThickness(5);
	btnMusiqueChoix.setOutlineColor(Color::Red);
	btnMusiqueChoix.setPosition(350, 355);
	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(450, 450, 200, 50)));
	texture.setSmooth(true);
	btnMusiqueChoix.setTexture(&texture);
	if (music.getStatus() == sf::Music::Playing)
	{
		btnMusiqueChoix.setFillColor(Color::Magenta);
	}
	else
	{
		btnMusiqueChoix.setFillColor(Color::White);
	}

	text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 355);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	windowOption.draw(btnMusiqueChoix);
	windowOption.draw(text);

	/*	btnMusiqueChoix.setFillColor(Color::White);*/
	btnMusiqueChoix.setOutlineThickness(5);
	btnMusiqueChoix.setOutlineColor(Color::Red);
	btnMusiqueChoix.setPosition(350, 420);
	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(450, 450, 200, 50)));
	texture.setSmooth(true);
	btnMusiqueChoix.setTexture(&texture);
	if (music2.getStatus() == sf::Music::Playing)
	{
		btnMusiqueChoix.setFillColor(Color::Magenta);
	}
	else
	{
		btnMusiqueChoix.setFillColor(Color::White);
	}
	text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 420);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	windowOption.draw(btnMusiqueChoix);
	windowOption.draw(text);

	btnMusiqueChoix.setFillColor(Color::White);
	btnMusiqueChoix.setOutlineThickness(5);
	btnMusiqueChoix.setOutlineColor(Color::Red);
	btnMusiqueChoix.setPosition(350, 485);
	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(450, 450, 200, 50)));
	texture.setSmooth(true);
	btnMusiqueChoix.setTexture(&texture);
	text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 485);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	if (music3.getStatus() == sf::Music::Playing)
	{
		btnMusiqueChoix.setFillColor(Color::Magenta);
	}
	else
	{
		btnMusiqueChoix.setFillColor(Color::White);
	}
	windowOption.draw(btnMusiqueChoix);
	windowOption.draw(text);

	//btnMenu.setFillColor(Color::White);
	btnMenu.setOutlineThickness(5);
	btnMenu.setOutlineColor(Color::Red);
	btnMenu.setPosition(10, 10);
	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(190, 310, 50, 50)));
	texture.setSmooth(true);
	btnMenu.setTexture(&texture);
	text.setString("Retour"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(10, 15);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	windowOption.draw(btnMenu);
	windowOption.draw(text);
	/*	btnNomJoueur.setFillColor(Color::White);*/
	btnNomJoueur.setOutlineThickness(5);
	btnNomJoueur.setOutlineColor(Color::Red);
	btnNomJoueur.setPosition(350, 550);
	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(90, 290, 300, 100)));
	texture.setSmooth(true);
	btnNomJoueur.setTexture(&texture);
	
	if (nomJoueur != "Nouveau Joueur")
	{
		btnNomJoueur.setFillColor(Color::Magenta);
	}
	text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 565);		// position du texte
	windowOption.draw(btnNomJoueur);
	windowOption.draw(text);



	text.setString(nomJoueur); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 625);		// position du texte
	windowOption.draw(text);
	windowOption.display();
	if (!font.loadFromFile("font_arial.ttf"));
	text.setFont(font);

	Event event;
	while (window.isOpen())
	{
		while (windowOption.pollEvent(event))
		{

			switch (event.type)
			{
			case Event::Closed:
				exit(0);
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
					window.setVisible(true);
					return 0;
			case Event::MouseButtonPressed:
				Mouse::getPosition(windowOption);
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 400) && (event.mouseButton.y > 275 && event.mouseButton.y < 325))
				{
					//Option musique on
					if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(150, 150, 50, 50)));
					windowOption.display();
					music.setVolume(100);
					music2.setVolume(100);
					music3.setVolume(100);
					btnMusiquePowerOn.setFillColor(Color::Magenta);
					btnMusiquePowerOn.setTexture(&texture);
					text.setString("On"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(352, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiquePowerOn);
					windowOption.draw(text);

					btnMusiquePowerOff.setFillColor(Color::White);
					btnMusiquePowerOff.setTexture(&texture);
					/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
					text.setString("Off"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(605, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiquePowerOff);
					windowOption.draw(text);

					windowOption.display();
				}
				else if ((event.mouseButton.x > 600 && event.mouseButton.x < 650) && (event.mouseButton.y > 275 && event.mouseButton.y < 325))
				{
					if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(150, 150, 50, 50)));
					btnMusiquePowerOn.setTexture(&texture);
					windowOption.display();
					music.setVolume(0);
					music2.setVolume(0);
					music3.setVolume(0);
					btnMusiquePowerOn.setFillColor(Color::White);
					text.setString("On"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(352, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiquePowerOn);
					windowOption.draw(text);

					btnMusiquePowerOff.setTexture(&texture);
					btnMusiquePowerOff.setFillColor(Color::Magenta);
					/*if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 300, 100)));*/
					text.setString("Off"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(605, 278);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiquePowerOff);
					windowOption.draw(text);


					windowOption.display();
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 355 && event.mouseButton.y < 405))
				{
					if (music.getStatus() != sf::Music::Playing)
					{
						music2.stop();
						music3.stop();
						music.openFromFile("music.ogg");
						music.play();
					}

					windowOption.display();
					if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(450, 450, 200, 50)));
					btnMusiqueChoix.setTexture(&texture);
					btnMusiqueChoix.setPosition(350, 355);
					btnMusiqueChoix.setFillColor(Color::Magenta);

					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix);
					windowOption.draw(text);

					btnMusiqueChoix.setPosition(350, 420);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiqueChoix);
					windowOption.draw(text);
					btnMusiqueChoix.setPosition(350, 485);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiqueChoix);
					windowOption.draw(text);
					windowOption.display();
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 420 && event.mouseButton.y < 470))
				{
					

					if (music2.getStatus() != sf::Music::Playing)
					{
						music.stop();
						music3.stop();
						music2.openFromFile("music2.ogg");
						music2.play();
					}


					windowOption.display();
					if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(450, 450, 200, 50)));
					btnMusiqueChoix.setTexture(&texture);

					btnMusiqueChoix.setPosition(350, 355);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiqueChoix);
					windowOption.draw(text);
					btnMusiqueChoix.setPosition(350, 420);
					btnMusiqueChoix.setFillColor(Color::Magenta);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiqueChoix);
					windowOption.draw(text);
					btnMusiqueChoix.setPosition(350, 485);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiqueChoix);
					windowOption.draw(text);
					windowOption.display();
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 485 && event.mouseButton.y < 535))
				{
					if (music3.getStatus() != sf::Music::Playing)
					{
						music.stop();
						music2.stop();
						music3.openFromFile("music3.ogg");
						music3.play();
					}
					windowOption.display();
					btnMusiqueChoix.setTexture(&texture);
					
					btnMusiqueChoix.setPosition(350, 355);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiqueChoix);
					windowOption.draw(text);
					btnMusiqueChoix.setPosition(350, 420);
					btnMusiqueChoix.setFillColor(Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiqueChoix);
					windowOption.draw(text);
					btnMusiqueChoix.setPosition(350, 485);
					btnMusiqueChoix.setFillColor(Color::Magenta);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte
					windowOption.draw(btnMusiqueChoix);
					windowOption.draw(text);
					windowOption.display();
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 550 && event.mouseButton.y < 700))
				{
					RenderWindow window2(VideoMode(500, 500), "TETRIS v1.2 Nom du joueur");
					if (!font.loadFromFile("font_arial.ttf"));
					if (!texture.loadFromFile("Tetris-Background_2.jpg"));
					Sprite background(texture);
					window2.draw(background);
					if (!font.loadFromFile("font_arial.ttf"));
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
								exit(0);
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
					windowOption.display();
					if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(90, 290, 300, 100)));
					btnNomJoueur.setFillColor(Color::Magenta);
					btnNomJoueur.setOutlineThickness(0);
					btnNomJoueur.setOutlineColor(Color::Red);
					btnNomJoueur.setPosition(350, 550);
					btnNomJoueur.setTexture(&texture);
					texture.setSmooth(true);


					text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 565);		// position du texte

					windowOption.draw(btnNomJoueur);
					windowOption.draw(text);


					text.setColor(Color::Black);
					text.setString(nomJoueur); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 625);		// position du texte

					windowOption.draw(text);
					windowOption.display();
					if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(450, 450, 200, 50)));
					btnMusique.setTexture(&texture);
				}
				else if ((event.mouseButton.x > 15 && event.mouseButton.x < 105) && (event.mouseButton.y > 15 && event.mouseButton.y < 65))
				{
					window.setVisible(true);
					return 0;
				}
				break;
			default:
				break;
			}

		}
	}
}


int choixMenu(RenderWindow &windowMenu, RenderWindow &window, Music &music, Music &music2, Music &music3, string &nomJoueur)
{
	Event event;
	while (windowMenu.isOpen())
	{
		while (windowMenu.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				exit(0);
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
					exit(0);
			case Event::MouseButtonPressed:
				Mouse::getPosition(windowMenu);
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 150 && event.mouseButton.y < 250))
				{
					return 1;
				}
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 300 && event.mouseButton.y < 400))
				{
					window.clear();
					menuOption(windowMenu, music, music2, music3, nomJoueur);
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

void titreTetris(RenderWindow &windowMenu, Font font, float posx, float posy, float size)
{
	Text text;

	text.setCharacterSize(size);
	text.setStyle(sf::Text::Bold);
	if (!font.loadFromFile("hemi_head_bd_it.ttf"));
	text.setFont(font);
	text.setString("T"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx, posy);		// position du texte
	text.setColor(Color::Cyan);		// choix de la couleur du texte
	windowMenu.draw(text);

	/*if (!font.loadFromFile("font_arial.ttf"));*/
	text.setFont(font);
	text.setString("E"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx + (size *1), posy);		// position du texte
	text.setColor(Color::Red);		// choix de la couleur du texte
	windowMenu.draw(text);
	/*	if (!font.loadFromFile("font_arial.ttf"));*/
	text.setFont(font);
	text.setString("T"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx + (size *2), posy);		// position du texte
	text.setColor(Color::Green);	// choix de la couleur du texte
	windowMenu.draw(text);
	/*if (!font.loadFromFile("font_arial.ttf"));*/
	text.setFont(font);
	text.setString("R"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx + (size *3), posy);		// position du texte
	text.setColor(Color::Cyan);		// choix de la couleur du texte
	windowMenu.draw(text);
	/*if (!font.loadFromFile("font_arial.ttf"));*/
	text.setFont(font);
	text.setString("I"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx + (size *4), posy);		// position du texte
	text.setColor(Color::White);	// choix de la couleur du texte
	windowMenu.draw(text);
	/*if (!font.loadFromFile("font_arial.ttf"));*/
	text.setFont(font);
	text.setString("S"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx + (size*4.65), posy);		// position du texte
	text.setColor(Color::Yellow);   // choix de la couleur du texte
	windowMenu.draw(text);
}
void afficherMenu(RenderWindow &window, Music &music, Music &music2, Music &music3, string &nomJoueur, Texture &texture, Font &font)
{
	window.setVisible(false);
	RenderWindow windowMenu(VideoMode(1000, 800), "TETRIS Menu");

	RectangleShape btnCommencer(Vector2f(300, 100));
	RectangleShape btnOption(Vector2f(300, 100));
	RectangleShape btnScore(Vector2f(300, 100));
	RectangleShape btnQuitter(Vector2f(300, 100));

	Text text;
	int BtnMenuChoisi;
	Sprite  background(texture);


	if (!texture.loadFromFile("Tetris-Background.jpg"));

	if (!font.loadFromFile("font_arial.ttf"));



	do
	{
		/*if (!texture.loadFromFile("Tetris-Background.jpg"));

		if (!font.loadFromFile("font_arial.ttf"));
*/

		windowMenu.draw(background);





		titreTetris(windowMenu, font, 340,50,60);
		Texture textureBouton;

		if (!font.loadFromFile("font_arial.ttf"));
		text.setFont(font);
		text.setColor(Color::Black);	// choix de la couleur du texte
		text.setCharacterSize(30);		// choix de la taille des caractères exprimée en pixels, pas en points !			
		btnCommencer.setFillColor(Color::White);
		btnCommencer.setOutlineThickness(5);
		btnCommencer.setOutlineColor(Color::Red);
		btnCommencer.setPosition(350, 150);
		if (!textureBouton.loadFromFile("image.jpg", IntRect(0, 0, 200, 100)));
		windowMenu.draw(btnCommencer);
		if (!textureBouton.loadFromFile("image.jpg", IntRect(0, 0, 200, 100)));
		btnCommencer.setTexture(&textureBouton);
		textureBouton.setSmooth(true);
		windowMenu.draw(btnCommencer);

		text.setString("Play :D"); 		// choix de la chaîne de caractères à afficher
		text.setPosition(450, 188);		// position du texte
		windowMenu.draw(text);


		btnOption.setFillColor(Color::White);
		btnOption.setOutlineThickness(5);
		btnOption.setOutlineColor(Color::Red);
		btnOption.setPosition(350, 300);


		if (!textureBouton.loadFromFile("image.jpg", IntRect(200, 500, 300, 100)));
		btnOption.setTexture(&textureBouton);

		windowMenu.draw(btnOption);
		text.setString("Options "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 333);		// position du texte
		windowMenu.draw(text);
		btnScore.setFillColor(Color::White);
		btnScore.setOutlineThickness(5);
		btnScore.setOutlineColor(Color::Red);
		btnScore.setPosition(350, 450);

		if (!textureBouton.loadFromFile("image.jpg", IntRect(300, 300, 300, 100)));
		btnScore.setTexture(&textureBouton);
		textureBouton.setSmooth(true);
		windowMenu.draw(btnScore);
		text.setString("Scores ");		// choix de la chaîne de caractères à afficher
		text.setPosition(450, 488);		// position du texte
		windowMenu.draw(text);

		btnQuitter.setFillColor(Color::White);
		btnQuitter.setOutlineThickness(5);
		btnQuitter.setOutlineColor(Color::Red);
		btnQuitter.setPosition(350, 600);

		if (!textureBouton.loadFromFile("image.jpg", IntRect(100, 500, 300, 100)));
		btnQuitter.setTexture(&textureBouton);
		textureBouton.setSmooth(true);
		windowMenu.draw(btnQuitter);
		text.setString("Quitter "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 625);		// position du texte
		windowMenu.draw(text);
		windowMenu.display();
		// check the type of the event...


		BtnMenuChoisi = choixMenu(windowMenu, window, music,music2, music3, nomJoueur);

		if (!font.loadFromFile("font_arial.ttf"));
		text.setFont(font);

	} while (BtnMenuChoisi != 1 && BtnMenuChoisi != -1);


	window.setVisible(true);




}
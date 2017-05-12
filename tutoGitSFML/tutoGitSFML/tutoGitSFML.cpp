//Commentaire inutile 
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
#include <pieces.h>			//Celui-ci		
#include <string>	
#include <saisieSecurisee.h>
#include <iostream>			
#include <fstream>			
#include <stdlib.h>			//Pk déjà?		
#include <vector>						
#include <SFML\Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;
//Commentaire inutile 
struct joueur
{
	string nomJoueur;
	bool nouveauJoueur = true;
	int level = 1,
		score = 0,
		rang = 0;
};
/*Prototypes des fonctions*/
/*========================*/
int saisie(RenderWindow &window, Music &music, Music &music2, Music &music3, joueur &joueur);
void formePiece(RectangleShape forme[5][5], int profil[5][5], Vector2f pos);
int menuOption(RenderWindow &window, Music &music, Music &music2, Music &music3, joueur &joueur);
int choixMenu(RenderWindow &windowMenu, RenderWindow &window, Music &music, Music &music2, Music &music3, joueur &joueur);
void afficherMenu(RenderWindow &window, Music &music, Music &music2, Music &music3, Texture &texture, Font &font, joueur &joueur);

void titreTetris(RenderWindow &windowMenu, Font font, float posx, float posy, float size);

int listeEnregistrement(RenderWindow &windowMenu, joueur &joueur);
void saisirNomJoueur(RenderWindow &window, Font font, Texture texture, joueur &joueur);
int questionOuiNonSFML(RenderWindow &window2, joueur &joueur);
int questionEnregistrement(RenderWindow &window2, Font font, joueur &joueur);
void afficherScore(RenderWindow &windowMenu);
void trierInsertion(char nomJoueur[][20], int scoreMax[], int taille);
void enregistrerScore(RenderWindow &window, joueur &joueur);

/* Programme principal.
===================== */
int main()
{
	joueur joueur;
	Music music2;
	Music music3;
	srand(time_t(NULL));		//Trouve une source aléatoire
	int lvl = 1,
		score = 0;
	string nomJoueur = "Nouveau Joueur"; //Nnom du joueur a la base
	setlocale(LC_CTYPE, "fra");			//Met la langue du programme en francais pour les accents.

	Music music;
	music.stop();
	music.openFromFile("music3.ogg"); // Ouvre la musique de Tétris

	Texture texture;
	if (!texture.loadFromFile("Tetris-Background.jpg"));
	Sprite background(texture);

	Font font;
	if (!font.loadFromFile("font_arial.ttf")); // choix de la police à utiliser
	int i = 0;

	int joueurEnregistrer, reponse = 1;

	RenderWindow window2(VideoMode(500, 500), "TETRIS v1.2 Nom du joueur");
	do
	{
		saisirNomJoueur(window2, font, texture, joueur);
		joueurEnregistrer = listeEnregistrement(window2, joueur);
		if (joueurEnregistrer == 1)
			reponse = questionEnregistrement(window2, font, joueur);
		else
		{
			reponse = 1;
			lvl = 0;
			score = 0;
		}
	} while (reponse != 1);
	
	window2.close();

	RenderWindow window(VideoMode(1000, 800), "TETRIS Jeu");
	Image icon;
	icon.loadFromFile("icon.png");
	window.setIcon(128, 128, icon.getPixelsPtr());  //Affiche l'icone de Tétris

	afficherMenu(window, music, music2, music3, texture, font, joueur);

	if (music.getStatus() == false)
		music.play();

	window.draw(background);

	Time moment;
	unsigned seed = time(0);
	Clock tempsEcoule;

	vector<Vector2i> occupations;
	bloc actif = TETRIS[rand() % (7) + 1];
	bloc prochain = TETRIS[rand() % (7) + 1];
	salle espace;
	espace.init(Vector2f(30, 30), 1, 1, occupations, "Joueur", 1, 0, 0, 1, TETRIS, actif, prochain);


	//Essais de blocs
	bloc active = espace.getBloc();
	int angle = active.getAngle();
	vector<Vector2i> profil;

	espace.getOccupation(occupations);

	Event event;

	int piece, pieceActive;

	piece = espace.prochain();
	actif = espace.getBloc();



	do
	{
		prochain = espace.getProchains();

		int posy, posx;
		int nbY = 0;

		espace.afficherInterface(window);
		espace.modifierInterface(window, prochain, profil, joueur.nomJoueur, joueur.level, joueur.score);

		int mouvement = 0;

		do {
			Sprite background(texture);
			window.draw(background);


			espace.afficherInterface(window);
			espace.modifierInterface(window, prochain, profil, joueur.nomJoueur, joueur.level, joueur.score);
			active.drawBloc(window, active.getAngle());

			mouvement = saisie(window, music, music2, music3, joueur);
			titreTetris(window, font, 150, 20, 60);
			nbY++;
			window.display();


			switch (mouvement)
			{
			case 1:
				afficherMenu(window, music, music2, music3, texture, font, joueur);
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



	} while (true);
	enregistrerScore(window2, joueur);
	return 0;


}

void trierInsertion(char nomJoueur[][20], int scoreMax[], int taille)
{
	char nomJoueurTemp[20];
	int scoreJoueur[20];
	int i, j;
	for (i = 1; i < taille; ++i) {
		int elem = scoreMax[i];
		strcpy_s(nomJoueurTemp, 20, nomJoueur[i]);
		for (j = i; j > 0 && scoreMax[j - 1] < elem; j--) {
			scoreMax[j] = scoreMax[j - 1];
			strcpy_s(nomJoueur[j], 20, nomJoueur[j - 1]);
		}
		scoreMax[j] = elem;
		strcpy_s(nomJoueur[j], 20, nomJoueurTemp);
	}
}

int questionOuiNonSFML(RenderWindow &window2, joueur &joueur)
{
	Event event;
	while (window2.isOpen())
	{


		while (window2.waitEvent(event))
		{
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

				else if (event.key.code == Keyboard::Escape) // Pour quitter
				{
					return 3;

				}
				else if (event.key.code == Keyboard::O)
				{
					return 1;
				}
				else if (event.key.code == Keyboard::N)
				{

					return 2;
				}

			} while (event.key.code == Keyboard::N && event.key.code == Keyboard::O);

		}

	}
}
void enregistrerScore(RenderWindow &window, joueur &joueur) {

	window.clear();

	int reponse = 0;
	int longueurNom = 1;
	Text text;

	text.setColor(Color::Green);
	text.setFillColor(Color::Green);
	Font font;
	if (!font.loadFromFile("font_arial.ttf"));
	text.setFont(font);
	text.setPosition(100, 50);
	text.setCharacterSize(22);

	do {

		if (joueur.nouveauJoueur == false) {
			window.display();
			text.setString("Voulez vous enregistrer par-dessus votre ancienne sauvegarde? (o / n)? : ");
			window.draw(text);
			window.display();

		}
		else {
			window.display();
			text.setString("Voulez vous enregistrer votre score? (o / n)? : ");
			window.draw(text);
			window.display();


		}
		reponse = questionOuiNonSFML(window, joueur);
	} while (reponse != 1 && reponse != 2);

	if (reponse == 1) {						//Enregistre tout les score dans un tableau

		ifstream vieuxscore;
		ofstream nouveauscore;

		vieuxscore.open("score.txt", ifstream::in | ifstream::out | ifstream::app);
		nouveauscore.open("score.txt", ofstream::in | ofstream::out | ofstream::app);

		char listeNomJoueur[20][20] = { '\0' };				//maximum de 30 joueur enregistrer
		int levelMax[20];
		int scoreMax[20];

		int cptJoueur = 0;

		if (joueur.nouveauJoueur == false) {					//si le joueur existe, remplace sont score par le nouveau
			while (vieuxscore >> listeNomJoueur[cptJoueur])
			{
				vieuxscore >> levelMax[cptJoueur];
				vieuxscore >> scoreMax[cptJoueur];
				cptJoueur++;
			}

			vieuxscore.close();
			vieuxscore.open("score.txt", ifstream::out | ifstream::trunc);
			vieuxscore.close();

			trierInsertion(listeNomJoueur, scoreMax, cptJoueur);

			/*char nomJoueurTemp[20];
			int scoreJoueur[20];
			int i, j;
			for (i = 1; i < conteur; ++i) {
			int elem = scoreMax[i];
			for (j = i; j > 0; j--) {
			if (elem < scoreMax[j - 1])
			break;
			else
			{
			scoreMax[j+1] = scoreMax[j];
			strcpy_s(nomJoueurTemp, 20, listeNomJoueur[j]);
			strcpy_s(listeNomJoueur[j], 20, listeNomJoueur[j - 1]);
			strcpy_s(listeNomJoueur[j - 1], 20, nomJoueurTemp);
			}
			}
			scoreMax[j+1] = elem;
			}*/


			for (int i = 0; i < joueur.rang - 1; i++)		// Écrit tout les autre joueur avant
			{
				nouveauscore << left << setw(20) << listeNomJoueur[i] << setw(20) << levelMax[i] << scoreMax[i] << endl;
			}
			nouveauscore << left << setw(20) << joueur.nomJoueur << setw(20) << joueur.level << joueur.score;

			for (int i = 0 + joueur.rang; i < cptJoueur; i++) // Écrit tout les autre joueur apres
			{
				nouveauscore << endl << left << setw(20) << listeNomJoueur[i] << setw(20) << levelMax[i] << scoreMax[i];
			}
		}

		else										//inscrit le nouveau joueur a la fin de la liste de score
		{
			nouveauscore << endl << left << setw(20) << joueur.nomJoueur << setw(20) << joueur.level << joueur.score;
			joueur.nouveauJoueur = false;
		}
		nouveauscore.close();
	}
}
void afficherScore(RenderWindow &windowMenu)
{

	Text text;
	Font font;
	Texture texture;
	if (!font.loadFromFile("font_arial.ttf"));		//va chercher la police pour le texte
	if (!texture.loadFromFile("Tetris-Background.jpg"));	//va chercher le fond d'écran

	Sprite background(texture);
	windowMenu.draw(background);		//affiche le fond d'écran	

	text.setFont(font);
	text.setColor(Color::Red);
	string nomJoueur;
	int lvl;
	int score;
	ifstream fichierScore;
	if (!ouvrirFichier(fichierScore, "score.txt", cout));

	text.setOutlineThickness(2);
	text.setOutlineColor(Color::Black);
	text.setPosition(400, 50);
	text.setString("10 Meilleurs score!");
	windowMenu.draw(text);

	text.setPosition(300, 100);
	text.setString("Joueur");
	windowMenu.draw(text);

	text.setPosition(475, 100);
	text.setString("Level");
	windowMenu.draw(text);
	text.setPosition(675, 100);
	text.setString("Score");
	windowMenu.draw(text);

	for (size_t i = 0; i < 10; i++)
	{
		fichierScore >> nomJoueur >> lvl >> score;

		text.setPosition(300, 150 + (50 * i));
		text.setString(nomJoueur);
		windowMenu.draw(text);

		text.setPosition(500, 150 + (50 * i));

		text.setString(to_string(lvl));
		windowMenu.draw(text);

		text.setPosition(700, 150 + (50 * i));
		text.setString(to_string(score));
		windowMenu.draw(text);
	}
	windowMenu.display();

	Event event;
	while (windowMenu.isOpen())
	{


		while (windowMenu.pollEvent(event))
		{
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

				if (event.key.code == Keyboard::Escape) // Pour quitter
				{
					return;

				}

			}
		}
	}
}
int questionEnregistrement(RenderWindow &window2, Font font, joueur &joueur)
{
	window2.clear();
	setlocale(LC_CTYPE, "fra");
	Event event;

	window2.display();
	Text text;

	if (!font.loadFromFile("font_arial.ttf")); // choix de la police à utiliser
	text.setFont(font);
	text.setString("Ce nom appartient a un joueur contenant \nles statistiques suivante :");
	text.setCharacterSize(22);
	text.setPosition(25, 100);
	text.setColor(Color::Black);
	window2.draw(text);


	char levelMax[15] = "Level max : ";
	levelMax[12] = joueur.level + '0';
	text.setString(levelMax);
	text.setPosition(25, 160);
	window2.draw(text);

	char textScoreMax[15] = "Score max : \0";
	char scoreMax[15];
	sprintf(scoreMax, "%d", joueur.score);
	strcat_s(textScoreMax, scoreMax);
	text.setString(textScoreMax);
	text.setPosition(25, 200);
	window2.draw(text);

	text.setString("Voulez vous continer avec cet enregistrement? \n Appuyez sur O ou sur N");
	text.setPosition(25, 250);
	window2.draw(text);
	/*joueurEnregistrer.setPosition(25, 25);
	joueurEnregistrer.setFillColor(Color::Black);*/
	/*window2.draw(joueurEnregistrer);*/
	window2.display();
	if (questionOuiNonSFML(window2, joueur) != 1)
	{

		return 2;
	}
	joueur.nouveauJoueur = false;
	return 1;
}
int listeEnregistrement(RenderWindow &window2, joueur &joueur)
{
	ifstream labyrinthe;
	char sauvegardeNomJoueur[15];
	int lvlEnregistrer,
		scoreEnregistrer,
		rangJoueur = 0;
	char continuerPartie;
	bool sauvegardeExiste = false,
		charIdentique = false;
	int longueur = NBCHARMAX;
	RectangleShape joueurEnregistrer(Vector2f(300, 125));
	Font font;
	if (!font.loadFromFile("font_arial.ttf")); // choix de la police à utiliser
	Text text;

	//strcpy_s(jeu.w.police, 30, nomsPolices[7]);
	//jeu.w.mesure -= 5;
	//saisirTabCharSDL(joueur.info.nom, longueur, "Quel est le nom de votre personnage ? : ", jeu);
	if (!ouvrirFichier(labyrinthe, "score.txt", cout))
		EXIT_FAILURE;

	while (labyrinthe >> sauvegardeNomJoueur && sauvegardeExiste == false) {			//lis le fichier de score au complet

		labyrinthe >> lvlEnregistrer >> scoreEnregistrer;
		rangJoueur++;
		strcat(sauvegardeNomJoueur, "\0");
		int longeurNom = strlen(sauvegardeNomJoueur);

		if (joueur.nomJoueur == sauvegardeNomJoueur)
		{//si le nom entrer existe dans le fichier score

			sauvegardeExiste = true;
			break;
		}
		else {

			sauvegardeExiste = false;
		}

	}

	if (sauvegardeExiste == true) {
		joueur.level = lvlEnregistrer;
		joueur.score = scoreEnregistrer;
		joueur.rang = rangJoueur;
		return 1;


	}
	else
		return 2;


}
void saisirNomJoueur(RenderWindow &window2, Font font, Texture texture, joueur &joueur)
{

	Image icon;
	Text text;
	icon.loadFromFile("icon.png");			//va chercher l'image de l'icone
	window2.setIcon(128, 128, icon.getPixelsPtr()); // Affiche l'icone

	if (!font.loadFromFile("font_arial.ttf"));		//va chercher la police pour le texte
	if (!texture.loadFromFile("Tetris-Background_2.jpg"));	//va chercher le fond d'écran

	Sprite background(texture);
	window2.draw(background);		//affiche le fond d'écran	

	text.setFont(font);
	text.setString("Quel est votre nom?"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(100, 15);		// position du texte
	text.setColor(Color::Red);   // choix de la couleur du texte
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(2);

	window2.draw(text);
	window2.display();

	std::string s;

	while (window2.isOpen())
	{
		//Event processing.
		Event event;
		while (window2.pollEvent(event)) {
		}
		while (window2.waitEvent(event)) {
			if (event.type == Event::Closed)
				window2.close();
			if (event.type == Event::TextEntered) {
				if (event.key.code == 8 && s.size() != 0)		//touche backspace
				{
					s.pop_back();  //Enleve le dernier char dans la string
				}
				else if (event.key.code == 13 && s.size() != 0) //touche enter
				{
					joueur.nomJoueur = s;
					return;
				}
				else if ((event.key.code >= 97 && event.key.code <= 122)
					|| event.key.code == 32
					|| (event.key.code >= 48 && event.key.code <= 57))	// caractere ASCII (A - Z, 0 - 9 )
				{
					s.push_back((char)event.text.unicode); // ajoute un char a la string
				}

				RectangleShape nomJoueur(Vector2f(400, 50));
				window2.display();

				if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(100, 50, 500, 50)));
				nomJoueur.setTexture(&texture);

				nomJoueur.setPosition(100, 50);
				window2.draw(nomJoueur);	//Affiche un rectangle vide

				text.setString(s); 	// choix de la chaîne de caractères à afficher
				text.setPosition(100, 50);		// position du texte
				text.setColor(Color::Green);   // choix de la couleur du texte

				window2.draw(text);

				window2.display();
			}

		}

	}

}
/*Fonctions*/
/*=========*/
// Si le joueur appuie sur une touche, la valeur de la touche sera retourné, sinon retourne 0
int saisie(RenderWindow &window, Music &music, Music &music2, Music &music3, joueur &joueur)
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
				return 0;
			}

			else if (event.key.code == Keyboard::Escape) // Pour quitter
			{
				return 1;

			}
			else if (event.key.code == Keyboard::Z) // Pour tourner la piece vers la gauche
			{
				return 6;
			}
			else if (event.key.code == Keyboard::Down) // pour descendre la piece
			{
				return 2;
			}
			else if (event.key.code == Keyboard::X || event.key.code == Keyboard::Up) // pour tourner la piece vert la droite
			{
				return 7;
			}
			else if (event.key.code == Keyboard::Left)	// pour bouger la piece vers la gauche
			{
				return 4;
			}
			else if (event.key.code == Keyboard::Right) 	// pour bouger la piece vers la droite
			{
				return 5;
			}
			else if (event.key.code == Keyboard::Pause || event.key.code == Keyboard::P) 	//Affiche le menu Option 
			{
				//Pause
				menuOption(window, music, music2, music3, joueur);
			}
			else if (event.key.code == Keyboard::M)  // Met le volume a 0
			{
				//Met la musique sur stop
				music.setVolume(0);
				music2.setVolume(0);
				music3.setVolume(0);
			}
			else if (event.key.code == Keyboard::Q)  // met le volume a 100
			{
				//Met la musique sur stop
				music.setVolume(100);
				music2.setVolume(100);
				music3.setVolume(100);
			}
			else if (event.key.code == Keyboard::T) // Fait jouer la prochaine musique dans la liste
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

//Permet de définir la forme de la piece
void formePiece(RectangleShape forme[5][5], int profil[5][5], Vector2f pos)
{
	for (int i = 0; i < 5; i++)	//pour chaque ligne
	{
		for (int j = 0; j < 5; j++) // chaque élément de la ligne
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

//Affiche le menu d'options pour pouvoir modifier la musique a jouer ou le nom du joueur
int menuOption(RenderWindow &window, Music &music, Music &music2, Music &music3, joueur &joueur)
{
	window.setVisible(false);	//Cache le Menu principal;

	RenderWindow windowOption(VideoMode(1000, 800), "TETRIS Option"); // Crée une nouvelle fenetre

	Texture texture;
	Image icon;
	icon.loadFromFile("icon.png");		//cherche l'image de l'icone
	windowOption.setIcon(128, 128, icon.getPixelsPtr());	//Défini une icone pour la fenetre

	RectangleShape btnMusique(Vector2f(300, 100));
	RectangleShape btnMusiquePowerOn(Vector2f(50, 50));
	RectangleShape btnMusiquePowerOff(Vector2f(50, 50));
	RectangleShape btnMusiqueChoix1(Vector2f(200, 50));
	RectangleShape btnMusiqueChoix2(Vector2f(200, 50));
	RectangleShape btnMusiqueChoix3(Vector2f(200, 50));
	RectangleShape btnRetour(Vector2f(100, 50));
	RectangleShape btnNomJoueur(Vector2f(300, 125));

	RectangleShape btnAutre(Vector2f(300, 100));

	Font font;
	Text text;

	if (!texture.loadFromFile("Tetris-Background.jpg"));  //Si la texture est introuvable, envoie un message d'erreur

	Sprite background(texture);

	windowOption.draw(background); // Affiche le fond d'écran

	if (!font.loadFromFile("font_arial.ttf")); // choix de la police à utiliser

	titreTetris(windowOption, font, 340, 50, 60); // Affiche le le titre TETRIS

	text.setFont(font);
	text.setCharacterSize(30); // choix de la taille des caractères exprimée en pixels, pas en points !					
	text.setStyle(Text::Bold); 	// choix du style du texte

	btnMusique.setOutlineThickness(5);  //défini la grosseur de la bordure
	btnMusique.setOutlineColor(Color::Red); //défini la couleur de la bordure
	btnMusique.setPosition(350, 150); //défini la position de la bordure

	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(0, 190, 300, 100))); // Si la texture ne peux etre 'loader' effiche un message d'erreur dans la console
	btnMusique.setTexture(&texture);	//défini la texture du bouton
	texture.setSmooth(true);			//addoucie la texture de l'image

	text.setString("Musique"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(450, 188);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	windowOption.draw(btnMusique);		//Dessine le bouton mussique
	windowOption.draw(text);			//Dessine le texte de bouton mussique

	btnMusiquePowerOn.setOutlineThickness(5);
	btnMusiquePowerOn.setOutlineColor(Color::Red);
	btnMusiquePowerOn.setPosition(350, 275);

	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(150, 150, 50, 50)));
	btnMusiquePowerOn.setTexture(&texture);
	texture.setSmooth(true);
	if (music.getVolume() == 0)
	{
		btnMusiquePowerOn.setFillColor(Color::White);
	}
	else
	{
		btnMusiquePowerOn.setFillColor(Color::Magenta);
	}


	text.setString("On"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(352, 278);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte
	windowOption.draw(btnMusiquePowerOn);
	windowOption.draw(text);


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

	btnMusiqueChoix1.setOutlineThickness(5);
	btnMusiqueChoix1.setOutlineColor(Color::Red);
	btnMusiqueChoix1.setPosition(350, 355);

	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(450, 450, 200, 50)));
	texture.setSmooth(true);
	btnMusiqueChoix1.setTexture(&texture);
	if (music.getStatus() == sf::Music::Playing)
	{
		btnMusiqueChoix1.setFillColor(Color::Magenta);
	}
	else
	{
		btnMusiqueChoix1.setFillColor(Color::White);
	}

	text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 355);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	windowOption.draw(btnMusiqueChoix1);
	windowOption.draw(text);


	btnMusiqueChoix2.setOutlineThickness(5);
	btnMusiqueChoix2.setOutlineColor(Color::Red);
	btnMusiqueChoix2.setPosition(350, 420);
	btnMusiqueChoix2.setTexture(&texture);

	if (music2.getStatus() == sf::Music::Playing)
	{
		btnMusiqueChoix2.setFillColor(Color::Magenta);
	}
	else
	{
		btnMusiqueChoix2.setFillColor(Color::White);
	}

	text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 420);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	windowOption.draw(btnMusiqueChoix2);
	windowOption.draw(text);


	btnMusiqueChoix3.setOutlineThickness(5);
	btnMusiqueChoix3.setOutlineColor(Color::Red);
	btnMusiqueChoix3.setPosition(350, 485);
	btnMusiqueChoix3.setTexture(&texture);

	text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 485);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	if (music3.getStatus() == sf::Music::Playing)
	{
		btnMusiqueChoix3.setFillColor(Color::Magenta);
	}
	else
	{
		btnMusiqueChoix3.setFillColor(Color::White);
	}
	windowOption.draw(btnMusiqueChoix3);
	windowOption.draw(text);

	btnRetour.setOutlineThickness(5);
	btnRetour.setOutlineColor(Color::Red);
	btnRetour.setPosition(10, 10);

	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(190, 310, 50, 50)));
	texture.setSmooth(true);
	btnRetour.setTexture(&texture);

	text.setString("Retour"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(10, 15);		// position du texte
	text.setColor(Color::Black);   // choix de la couleur du texte

	windowOption.draw(btnRetour);
	windowOption.draw(text);

	btnNomJoueur.setOutlineThickness(5);
	btnNomJoueur.setOutlineColor(Color::Red);
	btnNomJoueur.setPosition(350, 550);
	if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(90, 290, 300, 100)));
	texture.setSmooth(true);
	btnNomJoueur.setTexture(&texture);

	if (joueur.nomJoueur != "Nouveau Joueur")
	{
		btnNomJoueur.setFillColor(Color::Magenta);
	}
	text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 565);		// position du texte
	windowOption.draw(btnNomJoueur);
	windowOption.draw(text);

	text.setString(joueur.nomJoueur); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 625);		// position du texte

	windowOption.draw(text);
	windowOption.display();



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
				{// S l'utilisateur appuis que la touche echap, ferme le menu d'option et retourne au menu ou au jeu
					window.setVisible(true);
					return 0;
				}
			case Event::MouseButtonPressed:
				Mouse::getPosition(windowOption);
				// Si le joueur appuie sur le bouton On, met le volume de la musique a 100 et change le fond du bouton
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
				// Si le joueur appuie sur le bouton Off, Met le volume de la musique a 0 et change le fond du bouton
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
				// Si le joueur appuie sur le bouton Musique 1, fait jouer la musique correspondante et change le fond du bouton
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

					btnMusiqueChoix1.setFillColor(Color::Magenta);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix1);
					windowOption.draw(text);


					btnMusiqueChoix2.setFillColor(Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix2);
					windowOption.draw(text);


					btnMusiqueChoix3.setFillColor(Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix3);
					windowOption.draw(text);

					windowOption.display();
				}
				// Si le joueur appuie sur le bouton Musique 2, fait jouer la musique correspondante et change le fond du bouton
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
					btnMusiqueChoix1.setFillColor(Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix1);
					windowOption.draw(text);


					btnMusiqueChoix2.setFillColor(Color::Magenta);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix2);
					windowOption.draw(text);


					btnMusiqueChoix3.setFillColor(Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix3);
					windowOption.draw(text);

					windowOption.display();
				}
				// Si le joueur appuie sur le bouton Musique 3, fait jouer la musique correspondante et change le fond du bouton
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 485 && event.mouseButton.y < 535))
				{
					if (music3.getStatus() != sf::Music::Playing)
					{
						music.stop();
						music2.stop();
						music3.openFromFile("music3.ogg");
						music3.play();
					}
					if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(450, 450, 200, 50)));
					windowOption.display();
					btnMusiqueChoix1.setFillColor(Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix1);
					windowOption.draw(text);


					btnMusiqueChoix2.setFillColor(Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix2);
					windowOption.draw(text);


					btnMusiqueChoix3.setFillColor(Color::Magenta);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(Color::Black);   // choix de la couleur du texte

					windowOption.draw(btnMusiqueChoix3);
					windowOption.draw(text);


					windowOption.display();
				}
				// Si l'utilisateur appuie sur le bouton du Nom du joueur, ouvre une fenetre et demande a l'utilisateur d'entrer un nom
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 550 && event.mouseButton.y < 700))
				{
					RenderWindow window2(VideoMode(500, 500), "TETRIS v1.2 Nom du joueur");
					int joueurEnregistrer;
					int reponse = 1;
					do
					{

						saisirNomJoueur(window2, font, texture, joueur);

						joueurEnregistrer = listeEnregistrement(window2, joueur);
						if (joueurEnregistrer == 1)
						{

							reponse = questionEnregistrement(window2, font, joueur);
						}
						else
						{
							joueur.level = 0;
							joueur.score = 0;
						}
					} while (reponse != 1);





					windowOption.display();

					if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(90, 290, 300, 100)));
					btnNomJoueur.setTexture(&texture);
					texture.setSmooth(true);

					btnNomJoueur.setFillColor(Color::Magenta);
					btnNomJoueur.setOutlineThickness(0);
					btnNomJoueur.setOutlineColor(Color::Red);
					btnNomJoueur.setPosition(350, 550);

					text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 565);		// position du texte

					windowOption.draw(btnNomJoueur);
					windowOption.draw(text);

					text.setColor(Color::Black);
					text.setString(joueur.nomJoueur); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 625);		// position du texte

					windowOption.draw(text);
					windowOption.display();

					if (!texture.loadFromFile("Tetris-Background_2.jpg", IntRect(450, 450, 200, 50)));
					btnMusique.setTexture(&texture);
				}
				// Si le joueur appuie sur le bouton retour
				else if ((event.mouseButton.x > 15 && event.mouseButton.x < 105) && (event.mouseButton.y > 15 && event.mouseButton.y < 65))
				{
					window.setVisible(true);
					return 0;
				}
				break;
			}

		}
	}
}

// Selectionne le choix de l'utilisateur
int choixMenu(RenderWindow &windowMenu, RenderWindow &window, Music &music, Music &music2, Music &music3, joueur &joueur)
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
				//Bouton Play
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 150 && event.mouseButton.y < 250))
				{
					return 1;
				}
				//Bouton Option
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 300 && event.mouseButton.y < 400))
				{
					window.clear();
					menuOption(windowMenu, music, music2, music3, joueur);
					return 0;
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 450 && event.mouseButton.y < 550))
				{

					window.clear();
					afficherScore(windowMenu);
					return 0;
				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 600 && event.mouseButton.y < 700))
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

//Affiche le Titre de Tétris
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
	text.setPosition(posx + (size * 1), posy);		// position du texte
	text.setColor(Color::Red);		// choix de la couleur du texte
	windowMenu.draw(text);
	/*	if (!font.loadFromFile("font_arial.ttf"));*/
	text.setFont(font);
	text.setString("T"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx + (size * 2), posy);		// position du texte
	text.setColor(Color::Green);	// choix de la couleur du texte
	windowMenu.draw(text);
	/*if (!font.loadFromFile("font_arial.ttf"));*/
	text.setFont(font);
	text.setString("R"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx + (size * 3), posy);		// position du texte
	text.setColor(Color::Cyan);		// choix de la couleur du texte
	windowMenu.draw(text);
	/*if (!font.loadFromFile("font_arial.ttf"));*/
	text.setFont(font);
	text.setString("I"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx + (size * 4), posy);		// position du texte
	text.setColor(Color::White);	// choix de la couleur du texte
	windowMenu.draw(text);
	/*if (!font.loadFromFile("font_arial.ttf"));*/
	text.setFont(font);
	text.setString("S"); 			// choix de la chaîne de caractères à afficher
	text.setPosition(posx + (size*4.65), posy);		// position du texte
	text.setColor(Color::Yellow);   // choix de la couleur du texte
	windowMenu.draw(text);
}

//Affiche le menu et les options
void afficherMenu(RenderWindow &window, Music &music, Music &music2, Music &music3, Texture &texture, Font &font, joueur &joueur)
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

	Image icon;
	icon.loadFromFile("icon.png");
	windowMenu.setIcon(128, 128, icon.getPixelsPtr());

	do
	{
		windowMenu.draw(background);

		titreTetris(windowMenu, font, 340, 50, 60);
		Texture textureBouton;

		if (!font.loadFromFile("font_arial.ttf"));
		text.setFont(font);
		text.setColor(Color::Black);	// choix de la couleur du texte
		text.setCharacterSize(30);		// choix de la taille des caractères exprimée en pixels, pas en points !		

		btnCommencer.setFillColor(Color::White);
		btnCommencer.setOutlineThickness(5);
		btnCommencer.setOutlineColor(Color::Red);
		btnCommencer.setPosition(350, 150);
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

		BtnMenuChoisi = choixMenu(windowMenu, window, music, music2, music3, joueur);


	} while (BtnMenuChoisi != 1 && BtnMenuChoisi != -1);


	window.setVisible(true);




}
//Commentaire inutile 
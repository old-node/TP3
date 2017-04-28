﻿/* En-tête du programme
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

class salle
{
private:
	string _nomJoueur = "Joueur";	//Nom du joueur
	RectangleShape _boite;
	RectangleShape _information;
	Vector2f _pos = Vector2f(30, 30);//Position de la salle dans la fenêtre
	int _occupations[20][20] = { 0 };	//Zones où les blocs ne doivent pas pouvoir passer (murs) 
	int _noNiveau = 1,				//Numéro du niveau actuel du jeu
		_noJoueur = 1,				//Si plus qu'un joueur (peut être utilisé pour enregistrer son score)
		_points = 0,				//Score que le joueur à accumulé
		_nbBombe = 1,				//Autre option lol
		_styleBlocs = 1,			//textures, couleurs, etc. : nécessaire?
		_orientation = 1,			//si on fait tourner la salle
		_vitesseBloc = 1;			//vitesse des blocs qu'on crée par défaut
	bloc _typesBloc[7],				//La liste des blocs disponible
		_actif,						//Le bloc avec lequel on joue présentement
		_prochain;					//Le prochain bloc du jeu

public:
	//salle()
	//{
	//	initTypesBloc();
	//	placeMurs();
	//}
	salle()
	{
		_boite.setSize(Vector2f(600, 763));

		_boite.setOutlineThickness(10);
		_boite.setOutlineColor(Color::Red);
		_boite.setPosition(20, 20);
		Texture texture;
		if (!texture.loadFromFile("Tetris-Background_3.jpg", IntRect(50, 25, 300, 100)));
		_boite.setTexture(&texture);
		_information.setTexture(&texture);
		_information.setSize(Vector2f(300, 760));
		_information.setOutlineThickness(10);
		_information.setOutlineColor(Color::Red);
		_information.setPosition(650, 20);
	}
	~salle()
	{
		_boite.setSize(Vector2f(0, 0));

		_boite.setOutlineThickness(0);
		_boite.setOutlineColor(Color(0, 0, 0));
		_boite.setPosition(0, 0);
		Texture texture;
		if (!texture.loadFromFile("Tetris-Background_3.jpg", IntRect(50, 25, 300, 100)));
		_boite.setTexture(&texture);
		_information.setTexture(&texture);
		_information.setSize(Vector2f(0, 0));

		_information.setOutlineThickness(0);
		_information.setOutlineColor(Color(0, 0, 0));
		_information.setPosition(0, 0);
	}
	//~salle()
	//{
	//	_nomJoueur = "";
	//	_pos.x = _pos.y = _noNiveau = _noJoueur = _points = _nbBombe =
	//		_styleBlocs = _orientation = _vitesseBloc = 0;
	//	videOccupation();
	//	videTypeBloc();
	//}

	void afficherInterface(RenderWindow &window)
	{
		window.draw(_boite);
		window.draw(_information);
	}

	void modifierInterface(RenderWindow &window, RectangleShape forme[][5],
		int profil[][5], string nomJoueur)
	{
		RectangleShape interfacePieceSuivante(Vector2f(150, 150));
		RectangleShape afficherPieceSuivante(Vector2f(300, 750));
		Font font;
		font.loadFromFile("font_arial.ttf"); // choix de la police à utiliser
		Text text;
		text.setFont(font);
		afficherPieceSuivante.setFillColor(Color::White);
		afficherPieceSuivante.setOutlineThickness(10);
		afficherPieceSuivante.setOutlineColor(Color::Red);
		afficherPieceSuivante.setPosition(685, 25);
		window.draw(afficherPieceSuivante);
		interfacePieceSuivante.setFillColor(Color::Black);
		interfacePieceSuivante.setOutlineThickness(10);
		interfacePieceSuivante.setOutlineColor(Color::Red);
		interfacePieceSuivante.setPosition(740, 75);
		window.draw(interfacePieceSuivante);

		/*drawPiece(window, forme, profil);*/
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (profil[i][j] == 1)
				{
					window.draw(forme[i][j]);
				}
			}
		}
		//formePiece(forme, profil, { 750,85 });
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (profil[i][j] == 1)
				{
					forme[i][j].setSize(Vector2f(24, 24));
					forme[i][j].setPosition(745 + i * 26, 80 + j * 26);
					forme[i][j].setFillColor(Color::Red);
					forme[i][j].setOutlineThickness(1);
					forme[i][j].setOutlineColor(Color(0, 0, 0));
					window.draw(forme[i][j]);
				}
			}
		}


		text.setString("Prochaine piece"); 	// choix de la chaîne de caractères à afficher
		text.setCharacterSize(24); // choix de la taille des caractères exprimée en pixels, pas en points !						 
		text.setColor(Color::Black);   // choix de la couleur du texte
		text.setStyle(Text::Bold); 	// choix du style du texte
		text.setPosition(720, 25);		// position du texte
		window.draw(text);
		Text textLevel;
		textLevel.setFont(font);  // choix de la police à utiliser				
		textLevel.setString("Votre level : "); // choix de la chaîne de caractères à afficher
		textLevel.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textLevel.setColor(Color::Black);    // choix de la couleur du texte
		textLevel.setStyle(Text::Bold); 	// choix du style du texte
		textLevel.setPosition(700, 300);		// position du texte
		window.draw(textLevel);
		Text textNom;
		textLevel.setFont(font);  // choix de la police à utiliser				
		textLevel.setString(nomJoueur); // choix de la chaîne de caractères à afficher
		textLevel.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textLevel.setColor(Color::Black);    // choix de la couleur du texte
		textLevel.setStyle(Text::Bold); 	// choix du style du texte
		textLevel.setPosition(700, 350);		// position du texte
		window.draw(textLevel);
		Text textLigne;
		textLigne.setFont(font);  // choix de la police à utiliser				
		textLigne.setString("Nombre de ligne \nReussi : "); // choix de la chaîne de caractères à afficher
		textLigne.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textLigne.setColor(Color::Black);    // choix de la couleur du texte
		textLigne.setStyle(Text::Bold); 	// choix du style du texte
		textLigne.setPosition(700, 400);		// position du texte
		window.draw(textLigne);
		Text textScore;
		textScore.setFont(font);  // choix de la police à utiliser				
		textScore.setString("Score : "); // choix de la chaîne de caractères à afficher
		textScore.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textScore.setColor(Color::Black);    // choix de la couleur du texte
		textScore.setStyle(Text::Bold); 	// choix du style du texte
		textScore.setPosition(700, 500);		// position du texte
		window.draw(textScore);
		Text textAide;
		textAide.setFont(font);  // choix de la police à utiliser				
		textAide.setString("     Commande \n Z : Tourne a gauche \n X : Tourne a droite \n P : Pause \n M : Mute \n T : Prochaine musique \n Q : Unmute \n Esc : Menu"); // choix de la chaîne de caractères à afficher
		textAide.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textAide.setColor(Color::Black);    // choix de la couleur du texte
		textAide.setStyle(Text::Bold); 	// choix du style du texte
		textAide.setPosition(700, 550);		// position du texte
		window.draw(textAide);

	}
	/* Wich one? <><><><><> */
	//void modifierInterface(RenderWindow &window,
	//	RectangleShape forme[][5], int profil[][5], string nomJoueur)
	//{
	//
	//	RectangleShape interfacePieceSuivante(Vector2f(150, 150));
	//	RectangleShape afficherPieceSuivante(Vector2f(300, 750));
	//	Font font;
	//	font.loadFromFile("font_arial.ttf"); // choix de la police � utiliser
	//	Text text;
	//	text.setFont(font);
	//
	//	afficherPieceSuivante.setFillColor(Color::White);
	//	afficherPieceSuivante.setOutlineThickness(10);
	//	afficherPieceSuivante.setOutlineColor(Color::Red);
	//	afficherPieceSuivante.setPosition(685, 25);
	//	window.draw(afficherPieceSuivante);
	//
	//	interfacePieceSuivante.setFillColor(Color::Black);
	//	interfacePieceSuivante.setOutlineThickness(10);
	//	interfacePieceSuivante.setOutlineColor(Color::Red);
	//	interfacePieceSuivante.setPosition(740, 75);
	//	window.draw(interfacePieceSuivante);
	//
	//	/*drawPiece(window, forme, profil)*/
	//	for (int i = 0; i < 5; i++)
	//		for (int j = 0; j < 5; j++)
	//			if (profil[i][j] == 1)
	//				window.draw(forme[i][j]);
	//
	//
	//	//formePiece(forme, profil, { 750,85 });
	//	for (int i = 0; i < 5; i++)
	//		for (int j = 0; j < 5; j++)
	//		{
	//			if (profil[i][j] == 1)
	//			{
	//				forme[i][j].setSize(Vector2f(24, 24));
	//				forme[i][j].setPosition(745 + i * 26, 80 + j * 26);
	//				forme[i][j].setFillColor(Color::Red);
	//				forme[i][j].setOutlineThickness(1);
	//				forme[i][j].setOutlineColor(Color(0, 0, 0));
	//				window.draw(forme[i][j]);
	//			}
	//		}
	//
	//
	//	text.setString("Prochaine piece"); 	// choix de la cha�ne de caract�res � afficher
	//	text.setCharacterSize(24); // choix de la taille des caract�res exprim�e en pixels, pas en points !						 
	//	text.setColor(Color::Black);   // choix de la couleur du texte
	//	text.setStyle(Text::Bold); 	// choix du style du texte
	//	text.setPosition(720, 25);		// position du texte
	//	window.draw(text);
	//
	//	Text textLevel;
	//	textLevel.setFont(font);  // choix de la police � utiliser				
	//	textLevel.setString("Votre level : "); // choix de la cha�ne de caract�res � afficher
	//	textLevel.setCharacterSize(24); //choix de la taille des caract�res exprim�e en pixels, pas en points !
	//	textLevel.setColor(Color::Black);    // choix de la couleur du texte
	//	textLevel.setStyle(Text::Bold); 	// choix du style du texte
	//	textLevel.setPosition(700, 300);		// position du texte
	//	window.draw(textLevel);
	//
	//	Text textNom;
	//	textLevel.setFont(font);  // choix de la police � utiliser				
	//	textLevel.setString(nomJoueur); // choix de la cha�ne de caract�res � afficher
	//	textLevel.setCharacterSize(24); //choix de la taille des caract�res exprim�e en pixels, pas en points !
	//	textLevel.setColor(Color::Black);    // choix de la couleur du texte
	//	textLevel.setStyle(Text::Bold); 	// choix du style du texte
	//	textLevel.setPosition(700, 350);		// position du texte
	//	window.draw(textLevel);
	//
	//
	//	Text textLigne;
	//	textLigne.setFont(font);  // choix de la police � utiliser				
	//	textLigne.setString("Nombre de ligne \nReussi : "); // choix de la cha�ne de caract�res � afficher
	//	textLigne.setCharacterSize(24); //choix de la taille des caract�res exprim�e en pixels, pas en points !
	//	textLigne.setColor(Color::Black);    // choix de la couleur du texte
	//	textLigne.setStyle(Text::Bold); 	// choix du style du texte
	//	textLigne.setPosition(700, 400);		// position du texte
	//	window.draw(textLigne);
	//
	//	Text textScore;
	//	textScore.setFont(font);  // choix de la police � utiliser				
	//	textScore.setString("Score : "); // choix de la cha�ne de caract�res � afficher
	//	textScore.setCharacterSize(24); //choix de la taille des caract�res exprim�e en pixels, pas en points !
	//	textScore.setColor(Color::Black);    // choix de la couleur du texte
	//	textScore.setStyle(Text::Bold); 	// choix du style du texte
	//	textScore.setPosition(700, 500);		// position du texte
	//	window.draw(textScore);
	//
	//	Text textAide;
	//	textAide.setFont(font);  // choix de la police � utiliser				
	//	textAide.setString("     Commande \n Z : Tourne a gauche \n X : Tourne a droite \n P : Pause \n M : Mute \n T : Prochaine musique \n Q : Unmute \n Esc : Menu"); // choix de la cha�ne de caract�res � afficher
	//	textAide.setCharacterSize(24); //choix de la taille des caract�res exprim�e en pixels, pas en points !
	//	textAide.setColor(Color::Black);    // choix de la couleur du texte
	//	textAide.setStyle(Text::Bold); 	// choix du style du texte
	//	textAide.setPosition(700, 550);		// position du texte
	//	window.draw(textAide);
	//
	//}

	int prochain()
	{
		srand(time(NULL));
		int numBlock = rand() % 6 + 1;
		return numBlock;
	}

	salle(string nomJoueur)
	{
		setNomJoueur(nomJoueur);
		initTypesBloc();
		placeMurs();
	}
	salle(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
		string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
		const bloc typesBloc[7], bloc actif, bloc prochain)
	{
		init(pos, noNiveau, orientation, occupation, nomJoueur,
			noJoueur, points, nbBombe, vitesse, typesBloc, actif, prochain);
	}

	void setPos(Vector2f pos)
	{
		_pos = pos;
	}
	void setNoNiveau(int noNiveau)
	{
		_noNiveau = noNiveau;
	}
	void setOrientation(int orientation)
	{
		_orientation = orientation;
	}
	void getOccupationAbsolue(vector<int> occupation, vector<Vector2i> const& axes)
	{
		occupation.resize(0);
		for (auto const &element : axes)
			occupation.push_back(_occupations[element.x][element.y]);
	}
	void getOccupationRelative(vector<int> occupation, vector<Vector2i> const& axes, Vector2i place)
	{
		occupation.resize(0);
		for (auto const &element : axes)
			occupation.push_back(_occupations[place.x + element.x][place.y + element.y]);
	}
	bool checkOccupationAbsolue(vector<Vector2i> const& axes)
	{
		for (auto const &element : axes)
			if (_occupations[element.x][element.y] == 1)
				return true;
		return false;
	}
	bool checkOccupationRelative(vector<Vector2i> const& axes, Vector2i place)
	{
		for (auto const &element : axes)
			if (_occupations[place.x + element.x][place.y + element.y] == 1)
				return true;
		return false;
	}
	void setOccupationAbsolue(vector<Vector2i> const& axes)
	{
		for (auto const &element : axes)
			_occupations[element.x][element.y] = 1;
	}
	void setOccupationRelative(vector<Vector2i> const& axes, Vector2i place)
	{
		for (auto const &element : axes)
			_occupations[place.x + element.x][place.y + element.y] = 1;
	}
	void setNomJoueur(string nomJoueur)
	{
		_nomJoueur = nomJoueur;
	}
	void setNoJoueur(int noJoueur)
	{
		_noJoueur = noJoueur;
	}
	void setPoints(int points)
	{
		_points = points;
	}
	void setNbBombe(int nbBombe)
	{
		_nbBombe = nbBombe;
	}
	void setVitesse(int vitesse)
	{
		_vitesseBloc = vitesse;
	}
	void initTypesBloc()
	{
		for (int i = 0; i < 7; i++)
		{
			_typesBloc[i] = TETRIS[i];
		}
	}
	void setTypesBloc(const bloc typesBloc[7])
	{
		for (int i = 0; i < 7; i++)
			_typesBloc[i] = typesBloc[i];
	}
	void setActif(bloc actif)
	{
		_actif = actif;
	}
	void setProchain(bloc prochain)
	{
		_prochain = prochain;
	}

	void videTypeBloc()
	{
		vector<carre> vide[4];
		for (int i = 0; i < 7; i++)
			_typesBloc[i].setTours(vide);
	}
	void videOccupation()
	{
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
				_occupations[i][j] = { 0 };
	}
	//Other methods:: void set() {}

	void placeMurs()
	{
		for (int y = 0; y < 20; y++)
		{
			_occupations[19][y] = 1;
		}
		for (int x = 0; x < 20; x++)
		{
			_occupations[x][19] = 1;
			_occupations[x][0] = 1;
		}
	}
	string getNomJoueur()
	{
		return _nomJoueur;;
	}
	Vector2f getPos()
	{
		return _pos;
	}
	int getNoNiveau()
	{
		return _noNiveau;
	}
	int getNoJoueur()
	{
		return _noJoueur;
	}
	int getPoints()
	{
		return _points;
	}
	int getNbBombe()
	{
		return _nbBombe;
	}
	int getStyleBloc()
	{
		return _styleBlocs;
	}
	int getOrientation()
	{
		return _orientation;
	}
	int getVitesse()
	{
		return _vitesseBloc;
	}
	void getOccupation(vector<Vector2i> & occupation)
	{
		occupation.resize(0);
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
				if (_occupations[i][j] == 1)
					occupation.push_back(Vector2i(i, j));
	}

	bloc getBloc()
	{
		return _actif;
	}
	bloc getProchains()
	{
		return _prochain;
	}

	void init(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
		string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
		const bloc typesBloc[7], bloc actif, bloc prochain)
	{
		setPos(pos);
		setNoNiveau(noNiveau);
		setOrientation(orientation);
		setOccupationAbsolue(occupation);
		setNoJoueur(noJoueur);
		setPoints(points);
		setNbBombe(nbBombe);
		setVitesse(vitesse);
		setTypesBloc(typesBloc);
		setActif(actif);
		setProchain(prochain);

		placeMurs();
	}
	void init2(int noNiveau)
	{
		videOccupation();
		placeMurs();
		_noNiveau = noNiveau;
	}
	void load()
	{}
	void demare()
	{}
	void pause()
	{}
	void menu()
	{}
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

	void bougeX(int X)
	{
		int x = _actif.getPlace().x,
			y = _actif.getPlace().y,
			angle = _actif.getAngle();
		vector<carre>profil;
		_actif.getProfil(profil, angle);

		for (auto const &element : profil)
			if (_occupations[x + element._i + X][y + element._j] == 1)
				return;

		_actif.setPosX(x + X);
	}
	//Bouge le bloc d'une distance en y si elle n'entre pas en conflit avec la salle
	bool bougeY(int Y)
	{
		int x = _actif.getPlace().x,
			y = _actif.getPlace().y,
			angle = _actif.getAngle();
		vector<carre>profil[4];
		_actif.getProfil(profil[angle], angle);

		for (auto const &element : profil[angle])
			if (_occupations[x + element._i][y + element._j + Y] == 1)
				return false;

		_actif.setPosX(y + Y);
		return true;
	}

	void tourneGauche()
	{
		int angle = _actif.getAngle();
		if (--angle < 0)
			angle = 3;

		tourne(angle);
	}
	void tourneDroite()
	{
		int angle = _actif.getAngle();
		if (--angle < 0)
			angle = 3;

		tourne(angle);
	}
	void tourne(int angle)
	{
		int x = _actif.getPlace().x,
			y = _actif.getPlace().y;
		vector<Vector2i>profil;
		_actif.getAxes(profil, angle);

		for (auto const &element : profil)
			if (_occupations[x + element.x][y + element.y] == 1)
				return;

		_actif.setAngle(angle);
	}
	void tombe()
	{

	}
	void arrete()
	{

	}
	void colle()
	{

	}
};


//int occupations[30][20] = { '\0' };
//	for (size_t i = 0; i < 20; i++)
//		occupations[30][i] = { 1 };

/*Prototypes des fonctions*/
/*========================*/
int saisie();


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



int menuOption(RenderWindow &window, /*Music &music, */string &nomJoueur)
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
					//music.setVolume(100);
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
					//music.setVolume(0);
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
					//music.openFromFile("music.ogg");
					//music.play();
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
					//music.openFromFile("music2.ogg");
					//music.play();
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
					//music.openFromFile("music3.ogg");
					//music.play();
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


int choixMenu(RenderWindow &windowMenu, /*Music &music, */string &nomJoueur)
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
					menuOption(windowMenu, /*music, */nomJoueur);
					return 0;
				}
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 600 && event.mouseButton.y < 700))
				{
					windowMenu.close();
					return -1;
				}
				break;
			default:
				break;
			}
		}
	}
}

void afficherMenu(Music &music, string &nomJoueur)
{
	RenderWindow windowMenu(VideoMode(1000, 800), "TETRIS v1.2");
	Texture texture;
	RectangleShape btnCommencer(Vector2f(300, 100));
	RectangleShape btnOption(Vector2f(300, 100));
	RectangleShape btnScore(Vector2f(300, 100));
	RectangleShape btnQuitter(Vector2f(300, 100));
	Font font;
	Text text;

	do
	{

		if (!texture.loadFromFile("Tetris-Background.jpg"));
		Sprite background(texture);
		windowMenu.draw(background);
		font.loadFromFile("font_arial.ttf"); // choix de la police à utiliser
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
		if (!texture.loadFromFile("image.jpg", IntRect(0, 0, 200, 100)));
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
		if (!texture.loadFromFile("image.jpg", IntRect(200, 500, 300, 100)));
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
		if (!texture.loadFromFile("image.jpg", IntRect(100, 500, 300, 100)));
		btnQuitter.setTexture(&texture);
		texture.setSmooth(true);
		windowMenu.draw(btnQuitter);
		text.setString("Quitter "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 625);		// position du texte
		windowMenu.draw(text);
		windowMenu.display();
		// check the type of the event...
	} while (choixMenu(windowMenu, /*music, */nomJoueur) != 1);
}




/* Programme principal.
===================== */
int main()
{
	srand(time_t(NULL));		//Trouve une source aléatoire
	RenderWindow window(VideoMode(1000, 800), "TETRIS v1.2");
	
	string nomJoueur = "Nouveau Joueur";
	setlocale(LC_CTYPE, "fra");

	Music music;
	music.stop();
	music.openFromFile("music3.ogg");
	afficherMenu(music, nomJoueur);
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
	Vector2i posDud = active.getPlace();
	posDud.x -= 3;	//bloque de -2 ou -1 à 2 ?
	active.getAxes(profil, angle);

	espace.setOccupationRelative(profil, posDud);
	espace.getOccupation(occupations);

	Event event;
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
				afficherMenu(music, nomJoueur);
			}
			else if (event.key.code == Keyboard::Z)
			{
				//actif.tourneGauche(occupations);
			}
			else if (event.key.code == Keyboard::X || event.key.code == Keyboard::Up)
			{
				//actif.tourneDroite(occupations);
			}
			else if (event.key.code == Keyboard::Left)
			{
				//actif.bougeGauche(occupations);
			}
			else if (event.key.code == Keyboard::Right)
			{
				//actif.bougeDroite(occupations);
			}
			else if (event.key.code == Keyboard::Pause)
			{
				//Pause
				//Afficher menu
			}
			//else if (event.key.code == Keyboard::M)
			//{
			//	//Met la musique sur stop
			//	music.setVolume(0);
			//}
			//else if (event.key.code == Keyboard::Q)
			//{
			//	//Met la musique sur stop
			//	music.setVolume(75);
			//}
			//else if (event.key.code == Keyboard::T)
			//{
			//	music.stop();
			//	//Change la musique 
			//	i++;
			//	switch (i)
			//	{
			//	case 1:
			//		if (music.openFromFile("music.ogg"))
			//			music.play();
			//		;
			//		break;
			//	case 2:
			//		if (music.openFromFile("music2.ogg"))
			//			music.play();
			//		;
			//		break;
			//	case 3:
			//		if (music.openFromFile("music3.ogg"))
			//			music.play();
			//		i = 0;
			//		break;
			//	default:
			//		break;
			// we don't process other types of events
		default:
			break;
		}

	
	//piece = espace.prochain();

	//pieceActive = piece;

	switch (/*piece*/true)
	{
	case 1:
		//l.getProfil(profil);
		//posL2.x -= 3;
		//espace.setOccupation(profil, posL2);
		//formePiece(formeL, profil, l.getPlace());
		//blocActif.setId(1);
		//blocActif.setAxes(blocL);
		//piece = 8;
		break;
	case 2:
		//t.getProfil(profil);
		//posT2.x -= 3;
		//espace.setOccupation(profil, posT2);
		//formePiece(formeT, profil, t.getPlace());
		//blocActif.setId(2);
		//blocActif.setAxes(blocT);
		//piece = 8;
		break;
	case 3:
		//Carrer.getProfil(profil);
		//posCarrer2.x -= 3;
		//espace.setOccupation(profil, posCarrer2);
		//formePiece(formeCarrer, profil, Carrer.getPlace());
		//blocActif.setId(3);
		//blocActif.setAxes(blocCarrer);
		//piece = 8;
		break;
	case 4:
		//ligne.getProfil(profil);
		//posLigne2.x -= 3;
		//espace.setOccupation(profil, posLigne2);
		//formePiece(formeLigne, profil, ligne.getPlace());
		//blocActif.setId(4);
		//blocActif.setAxes(blocLigne);
		//piece = 8;
		break;
	case 5:
		//lInverser.getProfil(profil);
		//posLInverser2.x -= 3;
		//espace.setOccupation(profil, posLInverser2);
		//formePiece(formeLInverser, profil, lInverser.getPlace());
		//blocActif.setId(5);
		//blocActif.setAxes(blocLInverser);
		//piece = 8;
		break;
	case 6:
		//Z.getProfil(profil);
		//posZ2.x -= 3;
		//espace.setOccupation(profil, posZ2);
		//formePiece(formeZ, profil, Z.getPlace());
		//blocActif.setId(6);
		//blocActif.setAxes(blocZ);
		//piece = 8;
		break;
	case 7:
		//ZInverser.getProfil(profil);
		//posZinverser2.x -= 3;
		//espace.setOccupation(profil, posZinverser2);
		//formePiece(formeZInverser, profil, ZInverser.getPlace());
		//blocActif.setId(7);
		//blocActif.setAxes(blocZInverser);
		//piece = 8;
		break;
	default:
		break;
	}
	/*t.getProfil(profil);*/
	//espace.afficherInterface(window);
	//blocActif.getProfil(profil);
	//posActif2.x = -3;
	//espace.setOccupation(profil, posActif2);
	//formePiece(formeActif, profil, blocActif.getPlace());

	window.display();
	do
	{
		//prochainePiece = espace.prochain();
		//while (prochainePiece == piece)
		//{
		//	prochainePiece = espace.prochain();
		//}
		//switch (prochainePiece)
		//{
		//case 1:
		//	l.getProfil(profil);
		//	blocSuivant.setId(1);
		//	blocSuivant.setAxes(blocL);
		//	espace.modifierInterface(window, formeL, profil, nomJoueur);
		//	piece = 8;
		//	break;
		//case 2:
		//	t.getProfil(profil);
		//	blocSuivant.setId(2);
		//	blocSuivant.setAxes(blocT);
		//	espace.modifierInterface(window, formeT, profil, nomJoueur);
		//	piece = 8;
		//	break;
		//case 3:
		//	Carrer.getProfil(profil);
		//	blocSuivant.setId(3);
		//	blocSuivant.setAxes(blocCarrer);
		//	espace.modifierInterface(window, formeCarrer, profil, nomJoueur);
		//	piece = 8;
		//	break;
		//case 4:
		//	ligne.getProfil(profil);
		//	blocSuivant.setId(4);
		//	blocSuivant.setAxes(blocLigne);
		//	espace.modifierInterface(window, formeLigne, profil, nomJoueur);
		//	piece = 8;
		//	break;
		//case 5:
		//	lInverser.getProfil(profil);
		//	blocSuivant.setId(5);
		//	blocSuivant.setAxes(blocLInverser);
		//	espace.modifierInterface(window, formeLInverser, profil, nomJoueur);
		//	piece = 8;
		//	break;
		//case 6:
		//	Z.getProfil(profil);
		//	blocSuivant.setId(6);
		//	blocSuivant.setAxes(blocZ);
		//	espace.modifierInterface(window, formeZ, profil, nomJoueur);
		//	piece = 8;
		//	break;
		//case 7:
		//	ZInverser.getProfil(profil);
		//	blocSuivant.setId(7);
		//	blocSuivant.setAxes(blocZInverser);
		//	espace.modifierInterface(window, formeZInverser, profil, nomJoueur);
		//	piece = 8;
		//	break;
		//default:
		//	break;
		//}
	//blocSuivant.getProfil(profil);
	//posSuivant2.x = -3;
	//espace.setOccupation(profil, posSuivant2);
	//formePiece(pieceSuivante, profil, blocSuivant.getPlace());



	//testPackPlay(test, window);
	//l.tourneGauche(occupations);
	//l.getProfil(profil);
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
		//espace.modifierInterface(window, pieceSuivante, profil, nomJoueur);

		do {
			//posx = blocActif.getX();
			//posy = blocActif.getY();

			//saisie(window, l, occupations, music, i, nomJoueur);
			//if (!texture.loadFromFile("Tetris-Background.jpg"));
			//Sprite background(texture);
			//window.draw(background);

			//blocActif.getProfil(profil);
			//formePiece(formeActif, profil, blocActif.getPlace());
			//espace.afficherInterface(window);
			//espace.modifierInterface(window, pieceSuivante, profil, nomJoueur);

			//drawPiece(window, formeActif, profil);
			//formePiece(formeActif, profil, blocActif.getPlace());

			//blocActif.bougeY(posy, posx, occupations);

			nbY++;
			window.display();
			sleep(seconds(0.2));
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

	//Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
	}

	window.clear();
	testPackPlay(test, window);
	espace.getBloc().drawBloc(window);
	window.display();

	espace.tourneGauche();
	espace.bougeX(1);
	espace.setActif(espace.getBloc());

	sleep(seconds(0.3));
}


/*Fonctions*/
/*=========*/
// Première fonction.
int saisie()
{
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

	return 1;
}


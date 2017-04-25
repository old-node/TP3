<<<<<<< HEAD
/* En-t�te du programme patatemgqjebrhgikqurgqoierhgqrotih 
=======
﻿/* En-tête du programme		(version d'Olivier) ++
>>>>>>> refs/remotes/origin/master
=======================
Programme:      tutoGitsSFML.cpp
Auteur:			Jean-Alain Sainton & Olivier Lemay Dostie
Date création:	16/04/2017
Description:	Programme des essais SFML et de connection Git pour le projet final en C++
<<<<<<< HEAD
.				Espace .*/
=======
.				Espace de développement pour l'application.*/
>>>>>>> refs/remotes/origin/master

/* Directives au pré-processeur.
==============================*/
#include <locale>	
#include <string>
#include <iostream>
#include <iomanip>
#include <stdlib.h>	
#include <math.h>

#include <date.h>
#include <rectangle.h>
#include <saisieSecurisee.h>
#include <utile.h>

//#include <SDL.h>			//Pas inclus
//#include <SDL_ttf.h>		//Pas inclus
//#include <saisieSDL.h>	//Non fonctionnel si pas inclus
#include <SFML\Graphics.hpp>
using namespace sf;	//P-e que std serait plus approprié, mais on peut alterner


//Constantes des objets
const int NBCHARMAX = 30,	//Nombre de caractère maximum dans un message
PROCHAINS = 3;		//Nombre de prochai bloc visible

struct coord
{
	int x = 5,		//Coordonnée du message en x dans la fenêtre
		y = 5;		//Coordonnée du message en y dans la fenêtre
};

coord initCoord(int x, int y)
{
	coord point;
	point.x = x;
	point.y = y;
	return point;
}

int swapCoord(coord &gauche, coord &droite)
{
	swap(gauche.x, droite.x);
	swap(gauche.y, droite.y);
	return -1;
}

//struct style
//{
//	coord pos,		//Coordonnées
//		dim;		//Dimmenssion
//	int couleur,	//
//		texture;	//
//};
//
//class message 
//{
//private:
//	char _texte[NBCHARMAX] = { "\0" },
//		_police[NBCHARMAX] = { "\0" };
//	style _format;
//	int _propriete,
//		_durree,
//		_;
//	
//public:
//	//Initialisation de l'objet
//	message();
//	~message();
//
//};
//
//class fenetre 
//{
//private:
//	
//	style _format;			//
//	
//public:
//	fenetre ();
//	~fenetre ();
//};
//
//fenetre ::fenetre ()
//{
//}
//
//fenetre ::~fenetre () 
//{
//
//}

class bloc
{
private:
	coord _pos = initCoord(40, 20),	//Dans la fenêtre
		_place = initCoord(3, 10),	//Dans la salle
		_encrage = initCoord(2, 2);	//Point pivot du bloc
	int _id = 0,			//
		_styleBloc = 1,		//
		_vitesse = 1,		//
		_etat = 1,			//
		_forme = 1,			//
		_angle = 1,			//Orientation actuelle de la forme
		_axes[4][5][5] = {	//Composition de la forme selon chaque angles
			//270° vers la gauche (couché sur la droite)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
			},
			//Angle par défaut (debout)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
			},
			//90° vers la gauche (couché sur sa gauche)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
			},
			//180° sur elle-même (sur ça tête)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
			}, };

public:
	bloc()
	{}
	bloc(int forme, int axes[][5][5])
	{
		setForme(forme);
		setAxes(axes);
	}
	bloc(int x, int y, int i, int j, int id, int styleBloc,
		int vitesse, int etat, int forme, int angle, int axes[][5][5])
	{
		coord pos = initCoord(x, y);
		setPosX(pos);
		setPosY(pos);
		setEncrage(initCoord(i, j));
		setId(id);
		setStyleBloc(styleBloc);
		setVitesse(vitesse);
		setEtat(etat);
		setForme(forme);
		setAngle(angle);
		setAxes(axes);
	}
	~bloc()
	{
		_place.x = _place.y = _encrage.x = _encrage.y = _id =
			_styleBloc = _forme = _vitesse = _etat = 0;
		for (int p = 0; p < 4; p++)
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					_axes[p][i][j] = 0;
	}

	void setPosX(coord pos)
	{
		_place.x = pos.x;
	}
	void setPosY(coord pos)
	{
		_place.y = pos.y;
	}
	void setEncrage(coord pos)
	{
		_encrage.x = pos.x;
		_encrage.y = pos.y;
	}
	void setId(int id)
	{
		_id = id;
	}
	void setStyleBloc(int styleBloc)
	{
		_styleBloc = styleBloc;
	}
	void setForme(int forme)
	{
		_forme = forme;
	}
	void setVitesse(int vitesse)
	{
		_vitesse = vitesse;
	}
	void setEtat(int etat)
	{
		_etat = etat;
	}
	void setAngle(int angle)
	{
		_angle = angle;
	}
	void setAxes(int axes[][5][5])
	{
		for (int p = 0; p < 4; p++)
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					_axes[p][i][j] = axes[p][i][j];
	}
	void setProfil(int profil[5][5], int angle)
	{
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				_axes[angle][i][j] = profil[i][j];
	}
	coord getPlace()
	{
		return _place;
	}
	coord getPos()
	{
		return _pos;
	}
	coord getEncrage()
	{
		return _encrage;
	}
	int getId()
	{
		return _id;
	}
	int getStyleBloc()
	{
		return _styleBloc;
	}
	int getForme()
	{
		return _forme;
	}
	int getVitesse()
	{
		return _vitesse;
	}
	int getEtat()
	{
		return _etat;
	}
	int getAngle()
	{
		return _angle;
	}
	void getAxes(int axes[][5][5])
	{
		for (int p = 0; p < 4; p++)
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					axes[p][i][j] = _axes[p][i][j];
	}
	void getProfil(int profil[5][5])
	{
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				profil[i][j] = _axes[_angle][i][j];
	}

	void bougeX(int x, int salle[20][20])
	{
		bool vide = true;
		int sens = 1;
		coord depart = getPlace();
		coord fin = depart;
		fin.x += 5;
		fin.y += 5;
		if (x < 0)
			sens *= swapCoord(depart, fin);

		for (int i = depart.x; i < fin.x; i += sens)
			for (int j = depart.y; j < fin.y; j++)
				if (_axes[_angle][i][j] == 1)
					if (salle[i + x][j] == 1)
					{
						vide = false;
						break; break; break;
					}

		if (vide)
			_place.x -= x;
	}
	void bougeDroite(int salle[20][20])
	{

	}
	void tourneGauche(int salle[20][20])
	{
		int angle = _angle;
		bool permi = true;

		if (--angle < 0)
			angle = 3;

		for (int i = 0; permi && i < 5; i++)
			for (int j = 0; j < 5; j++)
			{
				if (_axes[angle][i][j] == 1)
					if (salle[i + _place.x][j + _place.y] == 1)
					{
						permi = false;
						break;
					}
			}

		if (permi)
			_angle = angle;
	}
	void tourneDroite(int salle[20][20])
	{
		int angle = _angle;
		bool permi = true;

		if (++angle < 0)
			angle = 3;

		for (int i = 0; permi && i < 5; i++)
			for (int j = 0; j < 5; j++)
			{
				if (_axes[angle][i][j] == 1)
					if (salle[i + _place.x][j + _place.y] == 1)
					{
						permi = false;
						break;
					}
			}

		if (permi)
			_angle = angle;
	}

	//void tombe()
	//{}
	//void ralenti()
	//{}
	//void arrete()
	//{}
	//void detruit()
	//{}
	//void colle()
	//{}
	//void separe()
	//{}
	//void fusionne()
	//{}
	//void efface()
	//{}
};

class salle
{
private:
<<<<<<< HEAD
	style _format;				//
	int _nomJoueur,				//
		_noJoueur,				//
		_points,				//
		_nbBombe,				//
		_styleBlocs,			//
		_orientation,			//
		_vitesseBloc,			//
		_bloc,					//
		_procains[PROCHAINS],	//
		_posBlocs[200][3],		//
		_occupation[20][20];	//
	
=======
	string _nomJoueur = "Joueur";	//
	coord _pos = initCoord(30, 30);	//
	int _noNiveau = 1,			//
		_noJoueur = 1,			//
		_points = 0,			//
		_nbBombe = 1,			//
		_styleBlocs = 1,		//
		_orientation = 1,		//
		_vitesseBloc = 1,		//
		_occupation[20][20] = { 0 };//
	//bloc _bloc,				//
	//	_prochains[PROCHAINS],	//
	//	_posBlocs[200][3],		//
>>>>>>> refs/remotes/origin/master

public:
	salle()
	{}
<<<<<<< HEAD
	
=======
>>>>>>> refs/remotes/origin/master
	~salle()
	{
		_nomJoueur = "";
		_pos.x = _pos.y = _noNiveau = _noJoueur = _points = _nbBombe =
			_styleBlocs = _orientation = _vitesseBloc = /*_bloc = */0;
		/*_procains[PROCHAINS] = _posBlocs[200][3] = */_occupation[20][20] = { 0 };
	}

	void setOccupation(int modification[5][5], coord pos)
	{
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				_occupation[pos.x + i][pos.y + j] = modification[i][j];
	}
	//Othermethods:: void set() {}

	string getNomJoueur()
	{
		return _nomJoueur;;
	}
	coord getPos()
	{
		return _pos;
	}
	int getNoNiveau()
	{
		return _noNiveau;
	}			//
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
	void getOccupation(int occupation[20][20])
	{
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
				occupation[i][j] = _occupation[i][j];
	}
	bloc getBloc()
	{
		//return _bloc;
	}
	void getProchains(int prochains[PROCHAINS])
	{
		for (int i = 0; i < PROCHAINS; i++)
			;//prochains[i] = _prochains[i];
	}
	void getPosBlocs(int posBlocs[200][3])
	{
		for (int i = 0; i < 200; i++)
			//if (_posBlocs[i][0] == 0)
			//	break;
			for (int j = 0; j < 3; j++)
				;//posBlocs = _posBlocs[i][j];
	}

	void init()
	{}
	void load()
	{}
	void demare()
	{}
	void pause()
	{}
	void menu()
	{}
	void prochain()
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
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (profil[i][j] == 1)
			{
				forme[i][j].setSize(sf::Vector2f(20, 20));
				forme[i][j].setPosition(pos.x + i * 20, pos.y + j * 20);
			}
		}
	}
}

void drawPiece(sf::RenderWindow &window,
	sf::RectangleShape forme[5][5], int profil[5][5])
{
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
}

int fonctionPosition(RectangleShape &rectangle)
{
	static bool retour = false;
	int x = rectangle.getPosition().x;

	if (retour)
	{
		x -= 200;
		if (x < 10)
			retour = false;
	}
	else
	{
		x += 200;
		if (x > 800)
			retour = true;
	}

	return x;
}

struct teStruct
{
	int outline = 10;
	sf::CircleShape boule;
	sf::RectangleShape rectangle;
	sf::RectangleShape shape;

	teStruct()
	{
		boule.setRadius(100.f);
		boule.setFillColor(sf::Color::Green);

		rectangle.setSize(sf::Vector2f(120, 50));

		shape.setSize(sf::Vector2f(100, 100));
		shape.setFillColor(sf::Color::Green);
		// set a 10-pixel wide orange outline
		shape.setOutlineThickness(10);
		shape.setOutlineColor(sf::Color(250, 150, 100));
		// set the absolute position of the entity
		shape.setPosition(60, 100);
		// set the absolute scale of the entity
		shape.setScale(4.0f, 1.6f);
		// set the absolute rotation of the entity
		shape.setRotation(45);
	}
};

void testPackPlay(teStruct &test, sf::RenderWindow &window)
{
	window.draw(test.boule);
	window.draw(test.shape);
	window.draw(test.rectangle);

	test.rectangle.setPosition(fonctionPosition(test.rectangle), 100);

	test.shape.setOutlineThickness(test.outline *= 3);
	// move the entity relatively to its current position
	test.shape.move(20, 5);
	test.boule.move(20, 5);

	// retrieve the absolute position of the entity
	sf::Vector2f position = test.shape.getPosition(); // = (15, 55)

	// rotate the entity relatively to its current orientation
	test.shape.rotate(10);

	// retrieve the absolute rotation of the entity
	float rotation = test.shape.getRotation(); // = 55

	// scale the entity relatively to its current scale
	test.shape.scale(0.8f, 0.4f);

	// retrieve the absolute scale of the entity
	sf::Vector2f scale = test.shape.getScale(); // = (2, 0.8)
}

int saisie()
{
	//Unknown .47 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z .20-46					1+26
	//Num0 Num1 Num2 Num3 Num4 Num5 Num6 Num7 Num8 Num9 .0-9									10
	//LControl RControl LShift RShift LAlt RAlt LSystem RSystem Menu .48-57						9
	//LBracket RBracket SemiColon Comma Period Quote Slash BackSlash Tilde .58-					9
	//^ ∞ × ÷ ≥ ≤ ≠ ± LParentesis RParentesis • $ € ¥ £ & % # @ ! ? _ < > é à ç ï ù ë (MAJ) © ® ™	
	//Play Pause2 Previous Next Mute Sound- Sound+ MuteMic Refresh MousePadLock PlaneMode CameraLock
	//Lock Display Contrast- Contrast+ 
	//Equal Dash Space Return BackSpace Tab PageUp PageDown End Home Insert Delete				12
	//Add Substract Multiply Divide Left Right Up Down											8
	//Numpad0 Numpad1 Numpad2 Numpad3 Numpad4 Numpad5 Numpad6 Numpad7 Numpad8 Numpad9 .10-19	10
	//F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12 F13 F14 F15 Pause KeyCount (toujours 102?)			15+2
	//
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Unknown))
	{
		//switch (switch_on)
		//{
		//default:
		//	break;
		//}
		//return 20;
		;
	}
	//else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))

	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))

	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		return -1;
}

//class MyClass {
//public:
//	MyClass & operator=(const MyClass &rhs);
//}

/* Programme principal.
===================== */
int main()
{

	//MyClass a, b;
	//b = a;   // Same as b.operator=(a);
	
	float p = atan(1) * 4;
	int occupations[20][20],
		blocL[4][5][5] = {
		//270° vers la gauche (couché sur la droite)
		{
			{ 0,0,0,0,0 },
			{ 0,0,0,0,0 },
			{ 0,1,1,1,0 },
			{ 0,1,0,0,0 },
			{ 0,0,0,0,0 },
		},
		//Angle par défaut (debout)
		{
			{ 0,0,0,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,1,1,0 },
			{ 0,0,0,0,0 },
		},
		//90° vers la gauche (couché sur sa gauche)
		{
			{ 0,0,0,0,0 },
			{ 0,0,0,1,0 },
			{ 0,1,1,1,0 },
			{ 0,0,0,0,0 },
			{ 0,0,0,0,0 },
		},
		//180° sur elle-même (sur ça tête)
		{
			{ 0,0,0,0,0 },
			{ 0,1,1,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,0,0,0 },
		}, };

	salle espace;
	bloc l(2, 2, 30, 30, 1, 1, 1, 1, 1, 1, blocL),
		pieces[7] = { l,l,l,l,l,l,l };

	sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML works!");

	//Tests
	teStruct test;

	//Essais de blocs
	sf::RectangleShape formeL[5][5];
	int profil[5][5];
	l.getProfil(profil);
	coord posL2 = l.getPlace();
	posL2.x -= 3;	//bloque de -2 ou -1 à 2
	espace.setOccupation(profil, posL2);
	espace.getOccupation(occupations);
	formePiece(formeL, profil, l.getPlace());

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		testPackPlay(test, window);
		drawPiece(window, formeL, profil);
		window.display();



		l.tourneGauche(occupations);
		l.getProfil(profil);
		formePiece(formeL, profil, l.getPlace());

		sf::sleep(sf::seconds(0.3));
	}

	return 0;
}

/*Méthodes
=========*/
//
//message::message() 
//{}
//
//message ::~message() 
//{
//	strcpy_s(_texte, NBCHARMAX, "\0");
//}

/*Fonctions
==========*/


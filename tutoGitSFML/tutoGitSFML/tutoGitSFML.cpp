/* En-tête du programme		(version d'Olivier) ++
=======================
Programme:      tutoGitsSFML.cpp
Auteur:			Jean-Alain Sainton & Olivier Lemay Dostie
Date création:	16/04/2017
Description:	Programme des essais SFML et de connection Git pour le projet final en C++
.				Espace de développement pour l'application.*/

/* Directives au pré-processeur.
==============================*/
#include <locale>	
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
	coord _pos = initCoord(30, 30),	//Dans la fenêtre
		_encrage = initCoord(2, 2);	//Point pivot du bloc
	int _id = 0,			//
		_styleBloc = 1,		//
		_vitesse = 1,		//
		_etat = 1,			//
		_forme = 1,			//
		_angle = 1,		//Orientation actuelle de la forme
		_axes[4][5][5] = {
		//Composition de la forme selon chaque angles
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
		_pos.x = _pos.y = _encrage.x = _encrage.y = _id =
			_styleBloc = _forme = _vitesse = _etat = 0;
		for (int p = 0; p < 4; p++)
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					_axes[p][i][j] = 0;
	}

	void setPosX(coord pos)
	{
		_pos.x = pos.x;
	}
	void setPosY(coord pos)
	{
		_pos.y = pos.y;
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


	void tourneGauche()
	{
		if (--_angle < 0)
			_angle = 3;
	}
	void tourneDroite()
	{
		if (++_angle > 3)
			_angle = 0;
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
	//void bougeGauche()
	//{}
	//void bougeDroite()
	//{}

};

//class salle
//{
//private:
//	style _format;				//
//	int _noNiveau,				//
//		_nomJoueur,				//
//		_noJoueur,				//
//		_points,				//
//		_nbBombe,				//
//		_styleBlocs,			//
//		_orientation,			//
//		_vitesseBloc,			//
//		_bloc,					//
//		_procains[PROCHAINS],	//
//		_posBlocs[200][3],		//
//		_occupation[20][20];	//
//	
//
//public:
//	salle()
//	{}
//	salle()
//	{}
//	~salle()
//	{
//
//	}
//	void init() 
//	{}
//	void load()
//	{}
//	void demare()
//	{}
//	void pause() 
//	{}
//	void menu() 
//	{}
//	void prochain() 
//	{}
//	void balaye() 
//	{}
//	void tetris() 
//	{}
//	void compresse() 
//	{}
//	void ferme()
//	{}
//	void creeObstacle()		//**
//	{}
//	void marcheArriere()	//**
//	{}
//	void tourne()			//**
//	{}
//	void brasse()			//**
//	{}
//};
//
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

/* Programme principal.
===================== */
int main()
{
	float p = atan(1) * 4;
	int blocL[4][5][5] = {
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

	bloc l(30, 30, 2, 2, 1, 1, 1, 1, 1, 1, blocL),
		pieces[7] = { l,l,l,l,l,l,l };

	sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML works!");
	sf::CircleShape boule(100.f);
	boule.setFillColor(sf::Color::Green);

	sf::RectangleShape rectangle(sf::Vector2f(120, 50));
	rectangle.setSize(sf::Vector2f(200, 20));
	sf::RectangleShape shape(sf::Vector2f(100, 100));
	shape.setFillColor(sf::Color::Green);
	// set a 10-pixel wide orange outline
	int outline = 10;
	shape.setOutlineThickness(outline);
	shape.setOutlineColor(sf::Color(250, 150, 100));
	// set the absolute position of the entity
	shape.setPosition(60, 100);
	// set the absolute scale of the entity
	shape.setScale(4.0f, 1.6f);
	// set the absolute rotation of the entity
	shape.setRotation(45);

	sf::RectangleShape formeL[5][5];
	int profil[5][5];
	l.getProfil(profil);
	formePiece(formeL, profil, l.getPos());

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(boule);
		window.draw(shape);
		window.draw(rectangle);
		drawPiece(window, formeL, profil);
		window.display();

		l.tourneGauche();
		l.getProfil(profil);
		formePiece(formeL, profil, l.getPos());

		rectangle.setPosition(fonctionPosition(rectangle), 100);

		shape.setOutlineThickness(outline *= 3);
		// move the entity relatively to its current position
		shape.move(20, 5);
		boule.move(20, 5);


		// retrieve the absolute position of the entity
		sf::Vector2f position = shape.getPosition(); // = (15, 55)

		// rotate the entity relatively to its current orientation
		shape.rotate(10);

		// retrieve the absolute rotation of the entity
		float rotation = shape.getRotation(); // = 55

		// scale the entity relatively to its current scale
		shape.scale(0.8f, 0.4f);

		// retrieve the absolute scale of the entity
		sf::Vector2f scale = shape.getScale(); // = (2, 0.8)

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


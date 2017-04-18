/* En-tête du programme
=======================
Programme:      tutoGitsSFML.cpp
Auteur:			Jean-Alain Sainton & Olivier Lemay Dostie
Date création:	16/04/2017
Description:	Programme des essais SFML et de connection Git pour le projet final en C++
.				Espace .*/

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

struct style
{
	coord pos,		//Coordonnées
		dim;		//Dimmenssion
	int couleur,	//
		texture;	//
};

class message 
{
private:
	char _texte[NBCHARMAX] = { "\0" },
		_police[NBCHARMAX] = { "\0" };
	style _format;
	int _propriete,
		_durree,
		_;
	
public:
	//Initialisation de l'objet
	message();
	~message();

};

class fenetre 
{
private:
	
	style _format;			//
	
public:
	fenetre ();
	~fenetre ();
};

fenetre ::fenetre ()
{
}

fenetre ::~fenetre () 
{

}

class bloc
{
private:
	style _format;			//
	int _id,				//
		_styleBloc,			//
		_forme,				//
		_profil[4][4],		//
		_vitesse,			//
		_etat;				//
public:
	bloc();
	~bloc();
	void tourneGauche() 
	{}
	void tourneDroite() 
	{}

	void tombe()
	{}
	void ralenti()
	{}
	void arrete()
	{}
	void detruit()
	{}
	void colle()
	{}
	void separe()
	{}
	void fusionne()
	{}
	void efface()
	{}
	void bougeGauche()
	{}
	void bougeDroite()
	{}

};

bloc::bloc()
{
}

bloc::~bloc()
{
}


class salle
{
private:
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
	

public:
	salle()
	{}
	salle()
	{}
	~salle()
	{

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

class indice
{
private:
	int _style;
public:
	indice();
	~indice();

};

indice::indice()
{
}

indice::~indice()
{
}


//Prototypes des fonctions


/* Programme principal.
===================== */
int main() 
{

	int p = atan(1) * 4;
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}

/*Méthodes
=========*/
//
message::message() 
{}

message ::~message() 
{
	strcpy_s(_texte, NBCHARMAX, "\0");
}

/*Fonctions
==========*/


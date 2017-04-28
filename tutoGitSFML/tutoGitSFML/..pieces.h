/* En-tête du fichier
======================
Fichier:		pieces.h
Auteur:         Olivier Lemay Dostie
Date création:  27/04/2017
Description:    header pour le TP3 de C++ pour la création de carrés, de blocs et 
.				de salles de jeu pour une version personnalisé de Tetris */

/*Directives au pré-processeur.
==========================*/
#pragma once
#include <locale>			
#include <string>			
#include <iostream>			
#include <iomanip>			
#include <stdlib.h>			//Pk déjà?
#include <assert.h>			
#include <vector>			
#include <math.h>			
#include <SFML\Graphics.hpp>
using namespace std;
using namespace sf;


//Constantes des objets
const int NBCHARMAX = 30,	//Nombre de caractère maximum dans un message
BLOCMAX = 5,				//Nb maximal en x et en y de carrés dans un bloc
CENTREDECARRE = 18,			//Largeur du centre des carrés
BORDUREDECARRE = 1;			//Largeur des bordures des carrés
const Vector2f LARGUEURCARRE = //Largeur des carrés dans la fenêtre
Vector2f(CENTREDECARRE + BORDUREDECARRE * 2, CENTREDECARRE + BORDUREDECARRE * 2),
MILLIEUCARRE =				//Point central des carrés
Vector2f(LARGUEURCARRE.x / 2, LARGUEURCARRE.y / 2);
const Vector2i COIN(9, 0),	//Position par défaut des bloc dans la salle
BASE(0, 0);					//Valeur par défaut de certaines composantes

const int PIECES[7][4][4][2] = // 7 formes, 4 angles, 4 carrés, 2 coordonnées {{{{2}*4}*4}*7} = 224? carrés o.o
{				// Les angles sont: droite (0), debout (1), gauche (2), renverse (3)
	{
		{ { 1,2 },{ 2,2 },{ 3,2 },{ 3,1 } },
		{ { 2,1 },{ 2,2 },{ 2,3 },{ 1,1 } },
		{ { 1,2 },{ 2,2 },{ 3,2 },{ 1,3 } },
		{ { 2,1 },{ 2,2 },{ 2,3 },{ 3,3 } }
	},	// Sept (L)
	{
		{ { 1,2 },{ 2,2 },{ 3,2 },{ 3,3 } },
		{ { 2,1 },{ 2,2 },{ 2,3 },{ 3,1 } },
		{ { 1,2 },{ 2,2 },{ 3,2 },{ 1,1 } },
		{ { 2,1 },{ 2,2 },{ 2,3 },{ 1,3 } }
	},	// Pendu (Mirroir du Sept)
	{
		{ { 2,2 },{ 2,3 },{ 3,2 },{ 3,3 } },
		{ { 2,2 },{ 2,3 },{ 3,2 },{ 3,3 } },
		{ { 2,2 },{ 2,3 },{ 3,2 },{ 3,3 } },
		{ { 2,2 },{ 2,3 },{ 3,2 },{ 3,3 } }
	},	// Carré
	{
		{ { 1,2 },{ 2,2 },{ 3,2 },{ 4,2 } },
		{ { 2,1 },{ 2,2 },{ 2,3 },{ 2,0 } },
		{ { 1,2 },{ 2,2 },{ 3,2 },{ 0,2 } },
		{ { 2,1 },{ 2,2 },{ 2,3 },{ 2,4 } }
	},	// Ligne
	{
		{ { 1,2 },{ 2,1 },{ 2,2 },{ 2,3 } },
		{ { 1,2 },{ 2,2 },{ 2,3 },{ 3,2 } },
		{ { 2,1 },{ 2,2 },{ 2,3 },{ 3,2 } },
		{ { 1,2 },{ 2,1 },{ 2,2 },{ 3,2 } }
	},	// Plateau
	{
		{ { 2,2 },{ 2,3 },{ 3,2 },{ 1,3 } },
		{ { 2,1 },{ 2,2 },{ 3,2 },{ 3,3 } },
		{ { 2,2 },{ 2,3 },{ 3,2 },{ 1,3 } },
		{ { 2,1 },{ 2,2 },{ 3,2 },{ 3,3 } }
	},	//	Croche (S)
	{
		{ { 1,2 },{ 2,2 },{ 2,3 },{ 3,3 } },
		{ { 1,2 },{ 2,3 },{ 3,2 },{ 3,1 } },
		{ { 1,2 },{ 2,2 },{ 2,3 },{ 3,3 } },
		{ { 1,2 },{ 2,3 },{ 3,2 },{ 3,1 } } } };//	Plié (Z)


/*Un des carrés d'un bloc ou dans la salle.
=========================================*/
struct carre
{
private:
	Color _couleur = RECT.getFillColor(),
		_couleurBord = RECT.getOutlineColor();
public:
	int _i = 2, _j = 2;				//Les coordonnées du carré dans le bloc
	RectangleShape vue = RECT;		//La forme SFML du carré
	int etat = 0;					//L'état du carré (selon son utilisation)

									// (Dim)menssion du bloc, (coin) de la salle, (x & y) coordonées du carré dans le bloc
	carre() {}
	carre(int x, int y, Vector2i coin, int rang)
	{
		setPos(x, y, coin);
		etat = rang;
	}
	carre(RectangleShape forme, int x, int y, Vector2i coin, int rang)
	{
		setVue(forme);
		setPos(x, y, coin);
		etat = rang;
	}
	carre(Vector2f dim, Vector2i coin, int x, int y, Color couleur,
		Color couleurBord, float bordure, float scaleX, float scaleY, int rang)
	{
		setVue(x, y, coin, dim, couleur, couleurBord, bordure, scaleX, scaleY);
		etat = rang;
	}
	~carre()
	{
		setVue(RectangleShape());
		_i = _j = etat = 0;
	}

	void setVue(RectangleShape forme)
	{
		vue = forme;
		_couleur = forme.getFillColor();
		_couleurBord = forme.getOutlineColor();
	}
	void setVue(RectangleShape forme, int x, int y,
		Vector2i coin, Vector2f dim, Color couleur)
	{
		setPos(x, y, coin);
		forme.setSize(Vector2f(dim.x, dim.y));
		forme.setFillColor(couleur);
		setVue(forme);
	}
	void setVue(int x, int y, Vector2i coin, Vector2f dim, Color couleur,
		Color couleurBord, float bordure, float scaleX, float scaleY)
	{
		RectangleShape forme;
		forme.setOutlineThickness(bordure);
		forme.setOutlineColor(couleurBord);
		forme.setScale(scaleX, scaleY);
		setVue(forme, x, y, coin, dim, couleur);
	}
	//Change la position du carré dans la salle
	void setPos(int x, int y, Vector2i coin)
	{
		assert(x >= 0 && x < BLOCMAX &&
			y >= 0 && y < BLOCMAX);
		_i = x; _j = y;
		vue.setPosition(Vector2f(
			_i * (MILLIEUCARRE.x * 2) + coin.x,
			_j * (MILLIEUCARRE.y * 2) + coin.y));
	}
	Vector2i getPos()
	{
		return Vector2i(_i, _j);
	}

	void cache()
	{
		vue.setFillColor(Color::Transparent);
		vue.setOutlineColor(Color::Transparent);
	}
	void montre()
	{
		vue.setFillColor(_couleur);
		vue.setOutlineColor(_couleurBord);
	}
};

class bloc
{
private:
	Vector2f _encrage = Vector2f(
		MILLIEUCARRE.x * BLOCMAX,
		MILLIEUCARRE.y * BLOCMAX);	//Point pivot du bloc
	Vector2i _place = Vector2i(9, 0);//Endroit situé dans la salle
	int _id = 0,			//
		_styleBloc = 0,		//
		_vitesse = 0,		//
		_etat = 0,			//
		_forme = 0,			//
		_angle = 0;			//Orientation actuelle de la forme
	vector<carre> _tours[4]{//Profil de la forme contenant tout les carrés
		{ vector<carre>(4) },{ vector<carre>(4) },
		{ vector<carre>(4) },{ vector<carre>(4) } };
	//vector<Vector2i> _axes[4]{		//Coordonnées du profil de la forme : nécessaire?
	//	{ vector<Vector2i>(4) },{ vector<Vector2i>(4) },
	//	{ vector<Vector2i>(4) },{ vector<Vector2i>(4) } };

public:
	//Instanciations
	bloc();
	bloc(int forme, vector<carre> tours[4]);
	bloc(int x, int y, int i, int j, int id, int styleBloc, int vitesse,
		int etat, int forme, int angle, vector<carre> tours[4]);
	//Destructeur
	~bloc();

	//Initialisation
	void init(int forme, vector<carre> tours[4]);
	void init2(int x, int y, int id, int styleBloc, int vitesse,
		int etat, int forme, int angle, vector<carre> tours[4]);
	
	//Coordonnées
	void setPos(Vector2i pos);
	void setPosX(int x);
	void setPosY(int y);
	void setEncrage(Vector2f pos);

	//Attributs du bloc
	void setId(int id);
	void setStyleBloc(int styleBloc);
	void setForme(int forme);
	void setVitesse(int vitesse);
	void setEtat(int etat);

	//Profils du bloc
	void setAngle(int angle);
	void setTours(vector<carre> tours[4]);

	//Modification du profil
	void ajouteCube(carre cube, int angle, int x, int y);
	void remplaceCube(carre cube, int angle, int position, int x, int y);
	void enleveCube(int angle);

	////Transforme les attributs du bloc
	//void ralenti();
	//void detruit();
	//void separe();
	//void fusionne();
	//void efface();

	//Retourne les attributs du bloc
	Vector2i getPlace();
	Vector2f getEncrage();
	int getId();
	int getStyleBloc();
	int getForme();
	int getVitesse();
	int getEtat();
	int getAngle();
	void getProfil(vector<carre> &tours, int angle);
	void getAxes(vector<Vector2i> axes, int angle);

	//Dessine le bloc
	void drawBloc(RenderWindow &window);
};

//Construit les pièces par défaut à partir des coordonnées du tableau PIECES
void initTetris(bloc )
{
	int x = COIN.x,
		y = COIN.y;
	vector<carre> constructeur[4]{//Profil de la forme contenant tout les carrés
		{ vector<carre>(0) },{ vector<carre>(0) },
		{ vector<carre>(0) },{ vector<carre>(0) } };

	for (int f = 0; f < 7; f++)
	{
		for (int a = 0; a < 4; a++)
			for (int c = 0; c < 4; c++)
				constructeur[a].push_back(carre(PIECES[f][a][c][0], PIECES[f][a][c][1], COIN, 0));

		tetris[f].init2(x, y, -1, 1, 1, -1, f, 1, constructeur);
		for (int i = 0; i < 4; i++)
			constructeur[i].resize(0);
	}
}

bloc tetris[7];
bool ok = initTetris(tetris);
const bloc TETRIS[7] = { tetris[0], tetris[1], tetris[2], tetris[3], tetris[4], tetris[5], tetris[6] };

/* Fonctions */
/* ========= */
//


/* Méthodes des carrés */
/* =================== */



//Méthodes des blocs

//
bloc::bloc() {
	//central(2, 2, _place, 1);
}

//
bloc::bloc(int forme, vector<carre> tours[4])
{
	init(forme, tours);
}

//
bloc::bloc(int x, int y, int i, int j, int id, int styleBloc, int vitesse,
	int etat, int forme, int angle, vector<carre> tours[4])
{
	init2(x, y, id, styleBloc, vitesse, etat, forme, angle, tours);
}

//
bloc::~bloc()
{
	_place.x = _place.y = _encrage.x = _encrage.y = _id =
		_styleBloc = _forme = _vitesse = _etat = _angle = 0;
	for (int i = 0; i < 4; i++)
		_tours[i].~vector();
}


//
void bloc::init(int forme, vector<carre> tours[4])
{
	setForme(forme);
	setTours(tours);
}

//
void bloc::init2(int x, int y, int id, int styleBloc, int vitesse,
	int etat, int forme, int angle, vector<carre> tours[4])
{
	setPos(Vector2i(x, y));
	setId(id);
	setStyleBloc(styleBloc);
	setVitesse(vitesse);
	setEtat(etat);
	setForme(forme);
	setAngle(angle);
	setTours(tours);
}

//
void bloc::setPos(Vector2i pos)
{
	_place = pos;
}

//
void bloc::setPosX(int x)
{
	_place.x = x;
}

//
void bloc::setPosY(int y)
{
	_place.y = y;
}

//
void bloc::setEncrage(Vector2f pos)
{
	_encrage.x = pos.x + MILLIEUCARRE.x * BLOCMAX;
	_encrage.y = pos.y + MILLIEUCARRE.y * BLOCMAX;
}

//
void bloc::setId(int id)
{
	_id = id;
}

//
void bloc::setStyleBloc(int styleBloc)
{
	_styleBloc = styleBloc;
}

//
void bloc::setForme(int forme)
{
	_forme = forme;
}

//
void bloc::setVitesse(int vitesse)
{
	_vitesse = vitesse;
}

//
void bloc::setEtat(int etat)
{
	_etat = etat;
}

//
void bloc::setAngle(int angle)
{
	_angle = angle;
}

//
void bloc::setTours(vector<carre> tours[4])
{
	//Chage les profils
	for (int i = 0; i < 4; i++)
	{
		_tours[i].resize(0);
		//Ajoute des carrés au profils
		for (auto const &element : tours[i])
			_tours[i].push_back(element);
	}
}

//
void bloc::ajouteCube(carre cube, int angle, int x, int y)
{
	cube.setPos(x, y, _place);
	_tours[angle].push_back(cube);
}

//
void bloc::remplaceCube(carre cube, int angle, int position, int x, int y)
{
	if (position < _tours[angle].size() && position >= 0)
	{
		cube.setPos(x, y, _place);
		_tours[angle].assign(position, cube);
	}
}

//
void bloc::enleveCube(int angle)
{
	//Est-ce vraiment le dernier qu'on veut enlever?
	_tours[angle].pop_back();
}

//
Vector2i bloc::getPlace()
{
	return _place;
}

//
Vector2f bloc::getEncrage()
{
	return _encrage;
}

//
int bloc::getId()
{
	return _id;
}

//
int bloc::getStyleBloc()
{
	return _styleBloc;
}

//
int bloc::getForme()
{
	return _forme;
}

//
int bloc::getVitesse()
{
	return _vitesse;
}

//
int bloc::getEtat()
{
	return _etat;
}

//
int bloc::getAngle()
{
	return _angle;
}

//
void bloc::getProfil(vector<carre> &tours, int angle)
{
	tours.resize(0);
	for (auto const &element : _tours[angle])
		tours.push_back(element);
}

//
void bloc::getAxes(vector<Vector2i> axes, int angle)
{
	axes.resize(0);
	for (auto &element : _tours[angle])
		axes.push_back(element.getPos());
}

//
void bloc::drawBloc(RenderWindow &window)
{
	for (auto const &element : _tours[_angle])
		window.draw(element.vue);
}

//
//void bloc::ralenti()
//{}
//void bloc::detruit()
//{}
//void bloc::separe()
//{}
//void bloc::fusionne()
//{}
//void bloc::efface()
//{}
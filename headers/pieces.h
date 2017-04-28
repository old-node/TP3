/* En-t�te du fichier
======================
Fichier:		pieces.h
Auteur:         Olivier Lemay Dostie
Date cr�ation:  27/04/2017
Description:    header pour le TP3 de C++ pour la cr�ation de carr�s, de blocs et
.				de salles de jeu pour une version personnalis� de Tetris */

/*Directives au pr�-processeur.
==========================*/
#pragma once
#include <locale>			
#include <string>			
#include <iostream>			
#include <iomanip>			
#include <stdlib.h>			//Pk d�j�?
#include <assert.h>			
#include <vector>			
#include <math.h>			
#include <SFML\Graphics.hpp>
using namespace std;
using namespace sf;

/*Constantes des objets*/
/*=====================*/
const int NBCHARMAX = 30,	//Nombre de caract�re maximum dans un message
BLOCMAX = 5,				//Nb maximal en x et en y de carr�s dans un bloc
CENTREDECARRE = 18,			//Largeur du centre des carr�s
BORDUREDECARRE = 1;			//Largeur des bordures des carr�s
const Vector2f LARGUEURCARRE = //Largeur des carr�s dans la fen�tre
Vector2f(CENTREDECARRE + BORDUREDECARRE * 2, CENTREDECARRE + BORDUREDECARRE * 2),
MILLIEUCARRE =				//Point central des carr�s
Vector2f(LARGUEURCARRE.x / 2, LARGUEURCARRE.y / 2);
const Vector2i COIN(9, 0),	//Position par d�faut des bloc dans la salle
BASE(0, 0);					//Valeur par d�faut de certaines composantes
double PI = atan(1) * 4;	//pi

//Les coordonn�es de chaque carr�s de chaque angles des 7 pi�ces par d�faut du jeu
const int PIECES[7][4][4][2] =
{		// 7 formes, 4 angles, 4 carr�s, 2 coordonn�es {{{{2}*4}*4}*7} = 224? carr�s o.o
	{	// Les angles sont: droite (0), debout (1), gauche (2), renverse (3)
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
	},	// Carr�
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
		{ { 1,2 },{ 2,3 },{ 3,2 },{ 3,1 } } } };//	Pli� (Z)

RectangleShape initRectangle(Vector2f echelle, Color couleur,
	Color couleurBord, float bordure, Vector2f pos, Vector2f origine);
//Forme rectangulaire des carr�s avec les valeurs de base
const RectangleShape RECT = initRectangle(Vector2f(1, 1),
	Color(150, 150, 255, 125), Color(), 1, Vector2f(0, 0), Vector2f(0, 0));

/*Un des carr�s d'un bloc ou d'une salle*/
/*======================================*/
struct carre
{
private:
	Color _couleur = RECT.getFillColor(),	//Couleur du carr� avant d'�tre mi transparent
		_couleurBord = RECT.getOutlineColor();//Couleur de la bordure avant d'�tre mi transparent
public:
	int _i = 2, _j = 2;				//Les coordonn�es du carr� dans le bloc
	RectangleShape vue = RECT;		//La forme SFML du carr�
	int etat = 0;					//L'�tat du carr� (selon son utilisation)

									// (Dim)menssion du bloc, (x & y) coordon�es du carr� dans le bloc, (coin) de la salle
									// Initialisateurs
	carre() {}
	carre(int x, int y, Vector2i coin, int rang);
	carre(RectangleShape forme, int x, int y, Vector2i coin, int rang);
	carre(Vector2f dim, Vector2i coin, int x, int y, Color couleur,
		Color couleurBord, float bordure, float scaleX, float scaleY, int rang);
	// Destructeurs
	~carre();

	// Change les propri�t�s du rectangle
	void setVue(RectangleShape forme);
	void setVue(RectangleShape forme, int x, int y,
		Vector2i coin, Vector2f dim, Color couleur);
	void setVue(int x, int y, Vector2i coin, Vector2f dim, Color couleur,
		Color couleurBord, float bordure, float scaleX, float scaleY);
	void cache();
	void montre();

	// Change la position du carr� dans la salle
	void setPos(int x, int y, Vector2i coin);

	//Retourne la position du carr� dans la salle
	Vector2i getPos();
};

class bloc
{
private:
	Vector2f _encrage = Vector2f(
		MILLIEUCARRE.x * BLOCMAX,
		MILLIEUCARRE.y * BLOCMAX);	//Point pivot du bloc
	Vector2i _place = Vector2i(1, 9);//Endroit situ� dans la salle
	int _id = 0,			//
		_styleBloc = 0,		//
		_vitesse = 0,		//
		_etat = 0,			//
		_forme = 0,			//
		_angle = 0;			// Orientation actuelle de la forme
	vector<carre> _tours[4]{// Profil de la forme contenant tout les carr�s
		{ vector<carre>(4) },{ vector<carre>(4) },
		{ vector<carre>(4) },{ vector<carre>(4) } };
	//vector<Vector2i> _axes[4]{		//Coordonn�es du profil de la forme : n�cessaire?
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

	//Initialisations
	void init(int forme, vector<carre> tours[4]);
	void init2(int x, int y, int id, int styleBloc, int vitesse,
		int etat, int forme, int angle, vector<carre> tours[4]);

	//Coordonn�es
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

/* Fonctions */
/* ========= */
//Forme un carr� selon les crit�res du jeu
RectangleShape initRectangle(Vector2f echelle, Color couleur,
	Color couleurBord, float bordure, Vector2f pos, Vector2f origine)
{
	RectangleShape rectangle(LARGUEURCARRE);
	rectangle.setScale(echelle);			//Transforme ses dimmensions selon une �chelle
	rectangle.setFillColor(couleur);		//Couleur principale
	rectangle.setOutlineColor(couleurBord);	//Couleur de la bordure
	rectangle.setOutlineThickness(bordure);	//�paisseur de la bordure
	rectangle.setPosition(pos);				//Position dans l'�cran
	rectangle.setOrigin(Vector2f(			//Point d'encrage du carr�
		MILLIEUCARRE.x + origine.x,
		MILLIEUCARRE.y + origine.y));
	Transform test = rectangle.getTransform();//Class qui m�morise les transformations effectu�s
	return rectangle;
}

//Construit les pi�ces par d�faut � partir des coordonn�es du tableau PIECES
bool initTetris(bloc tetris[7])
{
	int x = COIN.x,
		y = COIN.y;
	vector<carre> constructeur[4]{//Profil de la forme contenant tout les carr�s
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
	return 0;
}

bloc tetris[7];		// Liste de sept blocs � initialiser selon des formes
bool ok = initTetris(tetris);
// Les septs blocs par d�fauts du jeu: Sept(L), Pendu (Mirroir du Sept), Carr�, Ligne, Plateau (T), Croche (S), Pli� (Z)
const bloc TETRIS[7] = { tetris[0], tetris[1], tetris[2], tetris[3], tetris[4], tetris[5], tetris[6] };


/* M�thodes des carr�s */
/* =================== */

// 
carre::carre(int x, int y, Vector2i coin, int rang)
{
	setPos(x, y, coin);
	etat = rang;
}

// 
carre::carre(RectangleShape forme, int x, int y, Vector2i coin, int rang)
{
	setVue(forme);
	setPos(x, y, coin);
	etat = rang;
}

// Instancie un carr� avec toutes les attributs qu'il peut contenir
carre::carre(Vector2f dim, Vector2i coin, int x, int y, Color couleur,
	Color couleurBord, float bordure, float scaleX, float scaleY, int rang)
{
	setVue(x, y, coin, dim, couleur, couleurBord, bordure, scaleX, scaleY);
	etat = rang;
}

// Destructeur de carr�s
carre::~carre()
{
	setVue(RectangleShape());
	_i = _j = etat = 0;
}

// 
void carre::setVue(RectangleShape forme)
{
	vue = forme;
	_couleur = forme.getFillColor();
	_couleurBord = forme.getOutlineColor();
}

// 
void carre::setVue(RectangleShape forme, int x, int y,
	Vector2i coin, Vector2f dim, Color couleur)
{
	setPos(x, y, coin);
	forme.setSize(Vector2f(dim.x, dim.y));
	forme.setFillColor(couleur);
	setVue(forme);
}

// 
void carre::setVue(int x, int y, Vector2i coin, Vector2f dim, Color couleur,
	Color couleurBord, float bordure, float scaleX, float scaleY)
{
	RectangleShape forme;
	forme.setOutlineThickness(bordure);
	forme.setOutlineColor(couleurBord);
	forme.setScale(scaleX, scaleY);
	setVue(forme, x, y, coin, dim, couleur);
}

// Change la position du carr� dans la salle
void carre::setPos(int x, int y, Vector2i coin)
{
	assert(x >= 0 && x < BLOCMAX &&
		y >= 0 && y < BLOCMAX);
	_i = x; _j = y;
	vue.setPosition(Vector2f(
		_i * (MILLIEUCARRE.x * 2) + coin.x,
		_j * (MILLIEUCARRE.y * 2) + coin.y));
}

// 
Vector2i carre::getPos()
{
	return Vector2i(_i, _j);
}

// 
void carre::cache()
{
	vue.setFillColor(Color::Transparent);
	vue.setOutlineColor(Color::Transparent);
}

// 
void carre::montre()
{
	vue.setFillColor(_couleur);
	vue.setOutlineColor(_couleurBord);
}


/*M�thodes des blocs*/
/*==================*/

// Instancie un bloc par d�faut
bloc::bloc() {
	//central(2, 2, _place, 1);
}

// Instancie un bloc selon une forme sp�cifique
bloc::bloc(int forme, vector<carre> tours[4])
{
	init(forme, tours);
}

// Instancie un bloc avec toutes les attributs qu'il peut contenir
bloc::bloc(int x, int y, int i, int j, int id, int styleBloc, int vitesse,
	int etat, int forme, int angle, vector<carre> tours[4])
{
	init2(x, y, id, styleBloc, vitesse, etat, forme, angle, tours);
}

// Destructeur des blocs
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
		//Ajoute des carr�s au profils
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

//// 
//void bloc::ralenti()
//{
//}
//
//// 
//void bloc::detruit()
//{
//}
//
//// 
//void bloc::separe()
//{
//}
//
//// 
//void bloc::fusionne()
//{
//}
//
//// 
//void bloc::efface()
//{
//}


/*Autres cossins � tests*/
/*======================*/

struct teStruct	// Cr�e des formes pour les manipuler
{
	int outline = 10;
	CircleShape boule;
	RectangleShape rectangle;
	RectangleShape shape;

	teStruct()
	{
		boule.setRadius(100.f);
		boule.setFillColor(Color::Green);

		rectangle.setSize(Vector2f(120, 50));

		shape.setSize(Vector2f(100, 100));
		shape.setFillColor(Color::Green);
		// set a 10-pixel wide orange outline
		shape.setOutlineThickness(10);
		shape.setOutlineColor(Color(250, 150, 100));
		// set the absolute position of the entity
		shape.setPosition(60, 100);
		// set the absolute scale of the entity
		shape.setScale(4.0f, 1.6f);
		// set the absolute rotation of the entity
		shape.setRotation(45);
	}
};

// Manipule les formes
void testPackPlay(teStruct &test, RenderWindow &window)
{
	window.draw(test.boule);
	window.draw(test.shape);
	window.draw(test.rectangle);

	static bool retour = false;
	int x = test.rectangle.getPosition().x;
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
	test.rectangle.setPosition(x, 100);

	test.shape.setOutlineThickness(test.outline *= 3);
	// move the entity relatively to its current position
	test.shape.move(20, 5);
	test.boule.move(20, 5);

	// retrieve the absolute position of the entity
	Vector2f position = test.shape.getPosition(); // = (15, 55)

												  // rotate the entity relatively to its current orientation
	test.shape.rotate(10);

	// retrieve the absolute rotation of the entity
	float rotation = test.shape.getRotation(); // = 55

											   // scale the entity relatively to its current scale
	test.shape.scale(0.8f, 0.4f);

	// retrieve the absolute scale of the entity
	Vector2f scale = test.shape.getScale(); // = (2, 0.8)
}
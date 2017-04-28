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

/*Constantes des objets*/
/*=====================*/
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
double PI = atan(1) * 4;	//pi

//Les coordonnées de chaque carrés de chaque angles des 7 pièces par défaut du jeu
const int PIECES[7][4][4][2] =
{		// 7 formes, 4 angles, 4 carrés, 2 coordonnées {{{{2}*4}*4}*7} = 224? carrés o.o
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

RectangleShape initRectangle(Vector2f echelle, Color couleur,
	Color couleurBord, float bordure, Vector2f pos, Vector2f origine);
//Forme rectangulaire des carrés avec les valeurs de base
const RectangleShape RECT = initRectangle(Vector2f(1, 1),
	Color(150, 150, 255, 125), Color(), 1, Vector2f(0, 0), Vector2f(0, 0));

/*Un des carrés d'un bloc ou d'une salle*/
/*======================================*/
struct carre
{
private:
	Color _couleur = RECT.getFillColor(),	//Couleur du carré avant d'être mi transparent
		_couleurBord = RECT.getOutlineColor();//Couleur de la bordure avant d'être mi transparent
public:
	int _i = 2, _j = 2;				//Les coordonnées du carré dans le bloc
	RectangleShape vue = RECT;		//La forme SFML du carré
	int etat = 0;					//L'état du carré (selon son utilisation)

									// (Dim)menssion du bloc, (x & y) coordonées du carré dans le bloc, (coin) de la salle
									// Initialisateurs
	carre() {}
	carre(int x, int y, Vector2i coin, int rang);
	carre(RectangleShape forme, int x, int y, Vector2i coin, int rang);
	carre(Vector2f dim, Vector2i coin, int x, int y, Color couleur,
		Color couleurBord, float bordure, float scaleX, float scaleY, int rang);
	// Destructeurs
	~carre();

	// Change les propriétés du rectangle
	void setVue(RectangleShape forme);
	void setVue(RectangleShape forme, int x, int y,
		Vector2i coin, Vector2f dim, Color couleur);
	void setVue(int x, int y, Vector2i coin, Vector2f dim, Color couleur,
		Color couleurBord, float bordure, float scaleX, float scaleY);
	void cache();
	void montre();

	// Change la position du carré dans la salle
	void setPos(int x, int y, Vector2i coin);

	//Retourne la position du carré dans la salle
	Vector2i getPos();
};

class bloc
{
private:
	Vector2f _encrage = Vector2f(
		MILLIEUCARRE.x * BLOCMAX,
		MILLIEUCARRE.y * BLOCMAX);	//Point pivot du bloc
	Vector2i _place = Vector2i(1, 9);//Endroit situé dans la salle
	int _id = 0,			//
		_styleBloc = 0,		//
		_vitesse = 0,		//
		_etat = 0,			//
		_forme = 0,			//
		_angle = 0;			// Orientation actuelle de la forme
	vector<carre> _tours[4]{// Profil de la forme contenant tout les carrés
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

	//Initialisations
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

/* Fonctions pour les clocs */
/* ========= */
//Forme un carré selon les critères du jeu
RectangleShape initRectangle(Vector2f echelle, Color couleur,
	Color couleurBord, float bordure, Vector2f pos, Vector2f origine)
{
	RectangleShape rectangle(LARGUEURCARRE);
	rectangle.setScale(echelle);			//Transforme ses dimmensions selon une échelle
	rectangle.setFillColor(couleur);		//Couleur principale
	rectangle.setOutlineColor(couleurBord);	//Couleur de la bordure
	rectangle.setOutlineThickness(bordure);	//Épaisseur de la bordure
	rectangle.setPosition(pos);				//Position dans l'écran
	rectangle.setOrigin(Vector2f(			//Point d'encrage du carré
		MILLIEUCARRE.x + origine.x,
		MILLIEUCARRE.y + origine.y));
	Transform test = rectangle.getTransform();//Class qui mémorise les transformations effectués
	return rectangle;
}

//Construit les pièces par défaut à partir des coordonnées du tableau PIECES
bool initTetris(bloc tetris[7])
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
	return 0;
}

bloc tetris[7];		// Liste de sept blocs à initialiser selon des formes
bool ok = initTetris(tetris);
// Les septs blocs par défauts du jeu: Sept(L), Pendu (Mirroir du Sept), Carré, Ligne, Plateau (T), Croche (S), Plié (Z)
const bloc TETRIS[7] = { tetris[0], tetris[1], tetris[2], tetris[3], tetris[4], tetris[5], tetris[6] };


/* Méthodes des carrés */
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

// Instancie un carré avec toutes les attributs qu'il peut contenir
carre::carre(Vector2f dim, Vector2i coin, int x, int y, Color couleur,
	Color couleurBord, float bordure, float scaleX, float scaleY, int rang)
{
	setVue(x, y, coin, dim, couleur, couleurBord, bordure, scaleX, scaleY);
	etat = rang;
}

// Destructeur de carrés
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

// Change la position du carré dans la salle
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


/*Méthodes des blocs*/
/*==================*/

// Instancie un bloc par défaut
bloc::bloc() {
	//central(2, 2, _place, 1);
}

// Instancie un bloc selon une forme spécifique
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


/* Objet salle pour afficher ouvrir, jouer et manipuler le jeu principal*/
/* ===================================================================== */
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
	// Instanciations
	salle();
	salle(string nomJoueur);
	salle(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
		string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
		const bloc typesBloc[7], bloc actif, bloc prochain);
	// Destructeur
	~salle();
	void videOccupation();
	void videTypeBloc();
	void init(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
		string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
		const bloc typesBloc[7], bloc actif, bloc prochain);
	void init2(int noNiveau);
	void load();
	void demare();
	void placeMurs();

	// Change les attributs de la salle
	void modifierInterface(RenderWindow &window,
		RectangleShape forme[][5], int profil[][5], string nomJoueur);
	void initTypesBloc();
	void setTypesBloc(const bloc typesBloc[7]);
	void setPos(Vector2f pos);
	void setNoNiveau(int noNiveau);
	void setOrientation(int orientation);
	void setNomJoueur(string nomJoueur);
	void setNoJoueur(int noJoueur);
	void setPoints(int points);
	void setNbBombe(int nbBombe);
	void setVitesse(int vitesse);
	void setActif(bloc actif);
	void setProchain(bloc prochain);
	void setOccupationAbsolue(vector<Vector2i> const& axes);
	void setOccupationRelative(vector<Vector2i> const& axes, Vector2i place);
	int actif();
	int prochain();
	
	// Récupérations des attributs de la salle
	string getNomJoueur();
	Vector2f getPos();
	int getNoNiveau();
	int getNoJoueur();
	int getPoints();
	int getNbBombe();
	int getStyleBloc();
	int getOrientation();
	int getVitesse();
	bloc getBloc();
	bloc getProchains();
	void getOccupation(vector<Vector2i> & occupation);
	void getOccupationAbsolue(vector<int> occupation, vector<Vector2i> const& axes);
	void getOccupationRelative(vector<int> occupation, vector<Vector2i> const& axes, Vector2i place);
	bool checkOccupationAbsolue(vector<Vector2i> const& axes);
	bool checkOccupationRelative(vector<Vector2i> const& axes, Vector2i place);

	// Transformations de la salle
	void creeObstacle();
	void marcheArriere();
	void tourne();
	void brasse();

	// Contrôles principales
	void pause();
	void menu();
	void balaye();
	void tetris();
	void compresse();
	void ferme();
	
	// Déplacements et transformations du bloc actif
	void bougeX(int X);
	bool bougeY(int Y);
	void tourneGauche();
	void tourneDroite();
	void tourne(int angle);
	void tombe();
	void arrete();
	void colle();

	// Affichages
	void afficherInterface(RenderWindow &window);
};

/*Méthodes des salles*/
/*===================*/

// Instanciations

//salle::salle()
//{
//	initTypesBloc();
//	placeMurs();
//}

// 
salle::salle()
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

// 
salle::salle(string nomJoueur)
{
	setNomJoueur(nomJoueur);
	initTypesBloc();
	placeMurs();
}

// 
salle::salle(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
	string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
	const bloc typesBloc[7], bloc actif, bloc prochain)
{
	init(pos, noNiveau, orientation, occupation, nomJoueur,
		noJoueur, points, nbBombe, vitesse, typesBloc, actif, prochain);
}

//// Destructeur
//salle::~salle()
//{
//	_nomJoueur = "";
//	_pos.x = _pos.y = _noNiveau = _noJoueur = _points = _nbBombe =
//		_styleBlocs = _orientation = _vitesseBloc = 0;
//	videOccupation();
//	videTypeBloc();
//}

// 
salle::~salle()
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

// 
void salle::videOccupation()
{
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			_occupations[i][j] = { 0 };
}

// 
void salle::videTypeBloc()
{
	vector<carre> vide[4];
	for (int i = 0; i < 7; i++)
		_typesBloc[i].setTours(vide);
}

// 
void salle::init(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
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

// 
void salle::init2(int noNiveau)
{
	videOccupation();
	placeMurs();
	_noNiveau = noNiveau;
}

// 
void salle::load()
{}

// 
void salle::demare()
{}

// 
void salle::placeMurs()
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

/*Change les attributs de la salle*/

// 
void salle::modifierInterface(RenderWindow &window,
	RectangleShape forme[][5], int profil[][5], string nomJoueur)
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

// 
void salle::initTypesBloc()
{
	for (int i = 0; i < 7; i++)
	{
		_typesBloc[i] = TETRIS[i];
	}
}

// 
void salle::setTypesBloc(const bloc typesBloc[7])
{
	for (int i = 0; i < 7; i++)
		_typesBloc[i] = typesBloc[i];
}

// 
void salle::setPos(Vector2f pos)
{
	_pos = pos;
}

// 
void salle::setNoNiveau(int noNiveau)
{
	_noNiveau = noNiveau;
}

// 
void salle::setOrientation(int orientation)
{
	_orientation = orientation;
}

// 
void salle::setNomJoueur(string nomJoueur)
{
	_nomJoueur = nomJoueur;
}

// 
void salle::setNoJoueur(int noJoueur)
{
	_noJoueur = noJoueur;
}

// 
void salle::setPoints(int points)
{
	_points = points;
}

// 
void salle::setNbBombe(int nbBombe)
{
	_nbBombe = nbBombe;
}

// 
void salle::setVitesse(int vitesse)
{
	_vitesseBloc = vitesse;
}

// 
void salle::setActif(bloc actif)
{
	_actif = actif;
}

// 
void salle::setProchain(bloc prochain)
{
	_prochain = prochain;
}

// 
void salle::setOccupationAbsolue(vector<Vector2i> const& axes)
{
	for (auto const &element : axes)
		_occupations[element.x][element.y] = 1;
}

// 
void salle::setOccupationRelative(vector<Vector2i> const& axes, Vector2i place)
{
	for (auto const &element : axes)
		_occupations[place.x + element.x][place.y + element.y] = 1;
}

// 
int salle::actif()
{
	return 1;
}

// 
int salle::prochain()
{
	srand(time(NULL));
	int numBlock = rand() % 6 + 1;
	return numBlock;
}

// Récupérations des attributs de la salle

// 
string salle::getNomJoueur()
{
	return _nomJoueur;;
}

// 
Vector2f salle::getPos()
{
	return _pos;
}

// 
int salle::getNoNiveau()
{
	return _noNiveau;
}

// 
int salle::getNoJoueur()
{
	return _noJoueur;
}

// 
int salle::getPoints()
{
	return _points;
}

// 
int salle::getNbBombe()
{
	return _nbBombe;
}

// 
int salle::getStyleBloc()
{
	return _styleBlocs;
}

// 
int salle::getOrientation()
{
	return _orientation;
}

// 
int salle::getVitesse()
{
	return _vitesseBloc;
}

// 
bloc salle::getBloc()
{
	return _actif;
}

// 
bloc salle::getProchains()
{
	return _prochain;
}

// 
void salle::getOccupation(vector<Vector2i> & occupation)
{
	occupation.resize(0);
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			if (_occupations[i][j] == 1)
				occupation.push_back(Vector2i(i, j));
}

// 
void salle::getOccupationAbsolue(vector<int> occupation, vector<Vector2i> const& axes)
{
	occupation.resize(0);
	for (auto const &element : axes)
		occupation.push_back(_occupations[element.x][element.y]);
}

// 
void salle::getOccupationRelative(vector<int> occupation, vector<Vector2i> const& axes, Vector2i place)
{
	occupation.resize(0);
	for (auto const &element : axes)
		occupation.push_back(_occupations[place.x + element.x][place.y + element.y]);
}

// 
bool salle::checkOccupationAbsolue(vector<Vector2i> const& axes)
{
	for (auto const &element : axes)
		if (_occupations[element.x][element.y] == 1)
			return true;
	return false;
}

// 
bool salle::checkOccupationRelative(vector<Vector2i> const& axes, Vector2i place)
{
	for (auto const &element : axes)
		if (_occupations[place.x + element.x][place.y + element.y] == 1)
			return true;
	return false;
}

/* Transformations de la salle */

// 
void salle::creeObstacle()		//**
{}

// 
void salle::marcheArriere()	//**
{}

// 
void salle::tourne()			//**
{}

// 
void salle::brasse()			//**
{}

// Contrôles principales

// 
void salle::pause()
{}

// 
void salle::menu()
{}

// 
void salle::balaye()
{}

// 
void salle::tetris()
{}

// 
void salle::compresse()
{}

// 
void salle::ferme()
{}

/* Déplacements et transformations du bloc actif */

// Bouge le bloc d'une distance en x si elle n'entre pas en conflit avec la salle
void salle::bougeX(int X)
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

// Bouge le bloc d'une distance en y si elle n'entre pas en conflit avec la salle 
bool salle::bougeY(int Y)
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

// 
void salle::tourneGauche()
{
	int angle = _actif.getAngle();
	if (--angle < 0)
		angle = 3;

	tourne(angle);
}

// 
void salle::tourneDroite()
{
	int angle = _actif.getAngle();
	if (--angle < 0)
		angle = 3;

	tourne(angle);
}

// 
void salle::tourne(int angle)
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

// 
void salle::tombe()
{

}

// 
void salle::arrete()
{

}

// 
void salle::colle()
{

}

/* Affichages */

// 
void salle::afficherInterface(RenderWindow &window)
{
	window.draw(_boite);
	window.draw(_information);
}



/*Autres cossins à tests*/
/*======================*/

struct teStruct	// Crée des formes pour les manipuler
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
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

#include <locale>			
#include <string>			
#include <sstream>			
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
CENTRECARRE = 19,			//Largeur du centre des carrés
BORDURECARRE = 1;			//Largeur des bordures des carrés
const Vector2f LARGUEURCARRE = //Largeur des carrés dans la fenêtre
Vector2f(CENTRECARRE + BORDURECARRE * 2, CENTRECARRE + BORDURECARRE * 2),
MILLIEUCARRE =				//Point central des carrés
Vector2f(LARGUEURCARRE.x / 2, LARGUEURCARRE.y / 2);
const Vector2i COIN(9, 0);	//Position par défaut des bloc dans la salle

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

//Forme rectangulaire des carrés avec les valeurs de base
const RectangleShape RECT = initRectangle(Vector2f(1, 1),
	Color(150, 150, 255, 100), Color(), 1, Vector2f(0, 0), Vector2f(0, 0));

const double PI = 4 * atan(1);            // Valeur de PI

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
		Color couleurBord, int bordure, float scaleX, float scaleY, int rang)
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
		Color couleurBord, int bordure, float scaleX, float scaleY)
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
	Vector2i _place = Vector2i(9, 0);	//Endroit situé dans la salle
	int _id = 0,			//
		_styleBloc = 0,		//
		_vitesse = 0,		//
		_etat = 0,			//
		_forme = 0,			//
		_angle = 0;			//Orientation actuelle de la forme
	vector<carre> _tours[4]{//Profil de la forme contenant tout les carrés
		{vector<carre>(4)}, {vector<carre>(4)},
		{vector<carre>(4)}, {vector<carre>(4)} };
	vector<Vector2i> _axes[4]{//Coordonnées du profil de la forme : nécessaire?
		{ vector<Vector2i>(4) },{ vector<Vector2i>(4) },
		{ vector<Vector2i>(4) },{ vector<Vector2i>(4) } };

public:
	bloc() {
		carre central(2, 2, _place, 1);
		//{ { vector<carre>(1),
		//	vector<int>(1),
		//	vector<int>(1),
		//	vector<int>(1), } }
		//array<vector<carre>, 4> tourTest = {};
		//tourTest.push_back(vector<carre>(5));		//
		//tourTest.push_back(vector<double>(6, 4));	//Ajoute une ligne de 3 cases (4) dans le vector
		//tourTest[0].push_back(8);	// Ajoute une case (8) à la première ligne
		//double i = tourTest.at(1).size();
		//tourTest.at(1).at(tourTest.at(1).size() - 1) = 9;
		//vector<array<carre, 4>> *_tours = { new vector<array<carre, 4>>(4, central) };
		//_tours.at(0) =
	}
	bloc(int forme, vector<carre> tours[4])
	{
		init(forme, tours);
	}
	bloc(int x, int y, int i, int j, int id, int styleBloc, int vitesse,
		int etat, int forme, int angle, vector<carre> tours[4])
	{
		init(x, y, id, styleBloc, vitesse, etat, forme, angle, tours);
	}
	~bloc()
	{
		_place.x = _place.y = _encrage.x = _encrage.y = _id =
			_styleBloc = _forme = _vitesse = _etat = 0;
		for (int i = 0; i < 4; i++)
			_tours[0].~vector;
	}

	////Test avec les vectors
	//void create(vector<RectangleShape> a)
	//{
	//	a.push_back(RectangleShape(Vector2f(100, 100)));
	//	//vector<RectangleShape>::iterator tempIt = a.end();
	//	a.end()->setFillColor(sf::Color::White);
	//};

	void init(int forme, vector<carre> tours[4])
	{
		setForme(forme);
		setTours(tours);
	}
	void init(int x, int y, int id, int styleBloc, int vitesse,
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
	void setPos(Vector2i pos)
	{
		_place = pos;
	}
	void setPosX(int x)
	{
		_place.x = x;
	}
	void setPosY(int y)
	{
		_place.y = y;
	}
	void setEncrage(Vector2f pos)
	{
		_encrage.x = pos.x + MILLIEUCARRE.x * BLOCMAX;
		_encrage.y = pos.y + MILLIEUCARRE.y * BLOCMAX;
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
	void setTours(vector<carre> tours[4])
	{
		//Chage profils
		for (int i = 0; i < 4; i++)
		{
			//carre().~carre	?
			_tours[i].resize(0);
			//Ajoute des carrés au profils
			for (auto const &element : tours[i])
				_tours[i].push_back(element);
		}
	}
	void ajouteCube(carre cube, int angle, int x, int y)
	{
		cube.setPos(x, y, _place);
		_tours[angle].push_back(cube);
	}
	void remplaceCube(carre cube, int angle, int position, int x, int y)
	{
		if (position < _tours[angle].size() && position >= 0)
		{
			cube.setPos(x, y, _place);
			_tours[angle].assign(position, cube);
		}
	}
	void enleveCube(int angle)
	{
		//Est-ce vraiment le dernier qu'on veut enlever?
		_tours[angle].pop_back();
	}
	Vector2i getPlace()
	{
		return _place;
	}
	Vector2f getEncrage()
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
	void getProfil(vector<carre> tours, int angle)
	{
		tours.resize(0);
		for (auto const &element : _tours[angle])
			tours[i].push_back(element);
	}
	void getAxes(vector<Vector2i> axes[4])
	{
		for (int i = 0; i < 4; i++)
		{
			axes[i].resize(0);
			for (auto &element : _tours[i])
				axes[i].push_back = element.getPos();
		}
	}
	//Bouge le bloc d'une distance en x si elle n'entre pas en conflit avec la salle
	void drawBloc(RenderWindow &window)
	{
		for (int i = 0; i < 4; i++)
			for (auto const &element : _tours[i])
				window.draw(element.vue);
	}
	//void ralenti()
	//{}
	//void detruit()
	//{}
	//void separe()
	//{}
	//void fusionne()
	//{}
	//void efface()
	//{}
};

const bloc TETRIS[7];		//Les septs pièces par défaut

//Construit les pièces par défaut à partir des coordonnées du tableau PIECES
void initTetris()
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

		TETRIS[f].init(x, y, -1, 1, 1, -1, f, 1, constructeur);
	}
}

class salle
{
private:
	string _nomJoueur = "Joueur";	//Nom du joueur
	Vector2f _pos = Vector2f(30, 30);//Position de la salle dans la fenêtre
	int _noNiveau = 1,				//Numéro du niveau actuel du jeu
		_noJoueur = 1,				//Si plus qu'un joueur (peut être utilisé pour enregistrer son score)
		_points = 0,				//Score que le joueur à accumulé
		_nbBombe = 1,				//Autre option lol
		_styleBlocs = 1,			//textures, couleurs, etc. : nécessaire?
		_orientation = 1,			//si on fait tourner la salle
		_vitesseBloc = 1,			//vitesse des blocs qu'on crée par défaut
		_occupation[20][20] = { 0 };//Zones où les blocs ne doivent pas pouvoir passer (murs) 
	bloc _typesBloc[7],				//La liste des blocs disponible
		_actif,						//Le bloc avec lequel on joue présentement
		_prochain;					//Le prochain bloc du jeu

public:
	salle()
	{
		initTypesBloc();
		placeMurs();
	}
	salle(string nomJoueur)
	{
		setNomJoueur(nomJoueur);
		initTypesBloc();
		placeMurs();
	}
	salle(Vector2f pos, int noNiveau, int orientation, int occupation[20][20], string nomJoueur,
		int noJoueur, int points, int nbBombe, int vitesse, bloc typesBloc[7], bloc actif, bloc prochain)
	{
		setPos(pos);
		setNoNiveau(noNiveau);
		setOrientation(orientation);
		setOccupation(occupation);
		setNoJoueur(noJoueur);
		setPoints(points);
		setNbBombe(nbBombe);
		setVitesse(vitesse);
		setTypesBloc(typesBloc);
		setActif(actif);
		setProchain(prochain);

		placeMurs();
	}
	~salle()
	{
		_nomJoueur = "";
		_pos.x = _pos.y = _noNiveau = _noJoueur = _points = _nbBombe =
			_styleBlocs = _orientation = _vitesseBloc = 0;
		videOccupation();
		videTypeBloc();
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
	void setOccupation(int occupation[20][20])
	{
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
				_occupation[i][j] = occupation[i][j];
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
			_typesBloc[i] = TETRIS[i];
	}
	void setTypesBloc(bloc typesBloc[7])
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
		{
			_typesBloc[i].setTours(vide);
		}
	}
	void videOccupation()
	{
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
				_occupation[i][j] = { 0 };
	}
	void setOccupation(vector<Vector2i> _axes[4], Vector2i place)
	{
		for (int i = 0; i < 4; i++)
		{
			for (auto const &element : _axes[i])
				_occupation[place.x + element.x][place.y + element.y] = 1;
		}
	}
	//Other methods:: void set() {}

	void placeMurs()
	{
		for (int i = 0; i < 20; i++)
			_occupation[i][19] = 1;
		for (int i = 0; i < 20; i++)
		{
			_occupation[0][i] = 1;
			_occupation[19][i] = 1;
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
	void getOccupation(int occupation[20][20])
	{
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
				occupation[i][j] = _occupation[i][j];
	}
	bloc getBloc()
	{
		return _actif;
	}
	bloc getProchains()
	{
		return _prochain;
	}

	void init(int noNiveau)
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

	void bougeX(int deplacement)
	{
		int x = _actif.getPlace().x,
			y = _actif.getPlace().y,
			angle = _actif.getAngle();
		vector<carre>profil[4];
		_actif.getProfil(profil[angle], angle);
		//[_actif.getAngle()]
		for (auto const &element : profil[angle])
			if (_occupation[x + element._i + deplacement][y + element._j] == 1)
				return;

		_actif.setPosX(x + deplacement);
		for (int i = 0; i < 4; i++)
			for (auto &element : profil[i])
				element.setPos(element._i + deplacement, element._j);
		_actif.setTours(profil);
	}
	//Bouge le bloc d'une distance en y si elle n'entre pas en conflit avec la salle
	bool bougeY(int y)
	{
		for (auto const &element : _axes[_angle])
			if (_occupation[_place.x + element.x][_place.y + element.y + y] == 1)
				return false;

		_place.y += y;
		for (int i = 0; i < 4; i++)
			for (auto &element : _tours[i])
				element.setPos(element._i, element._j + y, _place);
		setTours(_tours);
		return true;
	}
	void tourneGauche()
	{
		int angle = _angle;
		if (--angle < 0)
			angle = 3;

		for (auto const &element : _axes[angle])
			if (_occupation[_place.x + element.x][_place.y + element.y] == 1)
				return;

		_actif.setAngle(angle);
	}
	void tourneDroite()
	{
		int angle = _actif.getAngle();
		if (++angle > 3)
			angle = 0;

		for (auto const &element : _axes[angle])
			if (_occupation[_place.x + element.x][_place.y + element.y] == 1)
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


//Prototypes des fonctions
////


struct teStruct
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

int saisie()
{
	//A B C D E F G H I J K L M N O P Q R S T U V W X Y Z .20-46					1+26
	//Num0 Num1 Num2 Num3 Num4 Num5 Num6 Num7 Num8 Num9 .0-9									10
	//Numpad0 Numpad1 Numpad2 Numpad3 Numpad4 Numpad5 Numpad6 Numpad7 Numpad8 Numpad9 .10-19	10
	//LControl RControl LShift RShift LAlt RAlt LSystem RSystem .47-56						9
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

/* Programme principal.
===================== */
int main()
{
	srand(time_t(NULL));		//Trouve une source aléatoire
	RenderWindow window(VideoMode(1000, 800), "SFML works!");

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

	//vector<RectangleShape> exemple;
	//std::vector<carre> allAsteroids; //List of asteroid CLASS OBJECTS
	//allAsteroids.push_back(carre()); //New asteroid CLASS OBJECT
	//for (std::vector<carre>::iterator it = allAsteroids.begin(); it != allAsteroids.end(); ++it) { //Creating asteroids
	//	it->carre(exemple);
	//}
	//for (vector<ConvexShape>::iterator it = exemple.begin(); it != exemple.end(); ++it) {
	//	window.draw(*it);
	//}

	float p = atan(1) * 4;
	int occupations[20][20];
	bloc actif = TETRIS[rand() % (7) + 1];
	bloc prochain = TETRIS[rand() % (7) + 1];
	salle espace(Vector2f(30, 30), 1, 1, occupations, "Joueur", 1, 0, 0, 1, TETRIS, actif, prochain);

	//Tests
	teStruct test;

	//Essais de blocs
	carre formeX();
	vector<Vector2i> profil[BLOCMAX*BLOCMAX];
	espace.getBloc().getAxes(profil);
	bloc dud = espace.getBloc();
	Vector2i posDud = dud.getPlace();
	posDud.x -= 3;	//bloque de -2 ou -1 à 2
	espace.setOccupation(profil, posDud);
	espace.getOccupation(occupations);
	//formePiece(formeX, profil, l.getPlace());

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();
		testPackPlay(test, window);
		espace.getBloc().drawBloc(window);
		window.display();

		espace.tourne
			.tourneGauche(occupations);
		l.bougeX(1, occupations);
		l.getAxes(profil);
		formePiece(formeX, profil, l.getPlace());

		sleep(seconds(0.3));
	}

	return 0;
}

/*Fonctions
==========*/

// Première fonction.

RectangleShape initRectangle(Vector2f echelle, Color couleur,
	Color couleurBord, float bordure, Vector2f pos, Vector2f origine)
{
	RectangleShape rectangle(LARGUEURCARRE);
	rectangle.setScale(echelle);
	rectangle.setFillColor(couleur);
	rectangle.setOutlineColor(couleurBord);
	rectangle.setOutlineThickness(bordure);
	rectangle.setPosition(pos);
	rectangle.setOrigin(Vector2f(
		MOITIECARRE.x + origine.x,
		MOITIECARRE.y + origine.y));
	Transform test = rectangle.getTransform();
	return rectangle;
}
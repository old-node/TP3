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
using namespace std;
using namespace sf;

class salle
{
private:
	string _nomJoueur = "Joueur";	//Nom du joueur
	Vector2f _pos = Vector2f(30, 30);//Position de la salle dans la fenêtre
	int _occupations[20][20] = {0};	//Zones où les blocs ne doivent pas pouvoir passer (murs) 
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
	salle(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
		string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
		const bloc typesBloc[7], bloc actif, bloc prochain)
	{
		init(pos, noNiveau, orientation, occupation, nomJoueur,
			noJoueur, points, nbBombe, vitesse, typesBloc, actif, prochain);
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


/*Prototypes des fonctions*/
/*========================*/
int saisie();


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

		espace.tourneGauche();
		espace.bougeX(1);
		espace.setActif(espace.getBloc());

		sleep(seconds(0.3));
	}

	return 0;
}

/*Fonctions*/
/*=========*/
// Première fonction.
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


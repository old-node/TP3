/* En-tête du fichier
======================
Fichier:		pieces.h
Auteur:			Olivier Lemay Dostie & Jean-Allain Sainton
Date création:	27/04/2017
Description:	header pour le TP3 de C++ pour la création de carrés, de blocs et
.				de salles de jeu pour une version personnalisé de Tetris. */

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
#include <carre.h>
#include <bloc.h>
using namespace std;
using namespace sf;

const int NBCHARMAX = 30;				// Nb de caractère maximum dans un message
const Keyboard::Key CONTROLES[2][6] =	/* Contrôles des joueurs si en mode multijoueur*/{
	/// Haut,		bas,			gauche,			droite,				tourne gauche,	tourne droite
	{ Keyboard::W,	Keyboard::S,	Keyboard::A,	Keyboard::D,		Keyboard::F,	Keyboard::G },
	{ Keyboard::Up,	Keyboard::Down,	Keyboard::Left,	Keyboard::Right,	Keyboard::Num0,	Keyboard::Period } };


/* Objet salle pour afficher ouvrir, jouer et manipuler le jeu principal*/
/* ===================================================================== */
class salle
{
private:
	string _nomJoueur = "Joueur";	// Nom du joueur
	int _noJoueur = 1;				// Si plus qu'un joueur (peut être utilisé pour enregistrer son score)
	int _occupations[20][20] = { 0 };// Zones où les blocs ne doivent pas pouvoir passer (murs) 
	int _noNiveau = 1,				// Numéro du niveau actuel du jeu
		_points = 0,				// Score que le joueur à accumulé
		_nbBombe = 1,				// Autre option lol
		_styleBlocs = 1,			// Textures, couleurs, etc. : nécessaire?
		_orientation = 1,			// Si on fait tourner la salle
		_vitesseBloc = 1;			// Vitesse des blocs qu'on crée par défaut
	bloc _typesBloc[7],				// La liste des blocs disponible
		_actif,						// Le bloc avec lequel on joue présentement
		_prochain;					// Le prochain bloc du jeu
	bool _permis = true;			// Si vrai, laisse le joueur faire des actions
	Vector2f _pos = Vector2f(30, 30);// Position de la salle dans la fenêtre
	RectangleShape _boite;			// Lieu où l'on affiche l'espace de jeu
	RectangleShape _information;	// Section pour affichier les statistiques

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
		bloc pieceSuivante, vector<Vector2i> profil, string nomJoueur);
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
	//int actif();
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
	bool bouge(int X, int Y);
	bool tourneGauche();
	bool tourneDroite();
	bool tourne(int angle);
	void tombe();
	void arret(const bool & permis);
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
//	
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

	placeMurs();
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
		_typesBloc[i].setFormes(vide);
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
		_occupations[19][y] = 1;
	for (int x = 0; x < 20; x++)
	{
		_occupations[x][19] = 1;
		_occupations[x][0] = 1;
	}
}

/*Change les attributs de la salle*/

// 
void salle::modifierInterface(RenderWindow &window,
	bloc pieceSuivante, vector<Vector2i> profil, string nomJoueur)
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
		_typesBloc[i] = TETRIS[i];
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

//// 
//int salle::actif()
//{
//	return 1;
//}

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
bool salle::bouge(int X, int Y)
{
	int x = _actif.getPlace().x,
		y = _actif.getPlace().y;
	vector<carre>profil;
	_actif.getProfil(profil);

	for (auto & element : profil)
	{
		if (_occupations[x + element.getX() + X][y + element.getY() + Y] == 1)
			return 0;
	}

	_actif.deplace(X, Y);
	return 1;
}

// 
bool salle::tourneGauche()
{
	int angle = _actif.getAngle();
	if (--angle < 0)
		angle = 3;

	return tourne(angle);
}

// 
bool salle::tourneDroite()
{
	int angle = _actif.getAngle();
	if (--angle < 0)
		angle = 3;

	return tourne(angle);
}

// 
bool salle::tourne(int angle)
{
	int x = _actif.getPlace().x,
		y = _actif.getPlace().y;
	vector<Vector2i>profil;
	_actif.getAxes(profil);

	for (auto const &element : profil)
	{
		if (_occupations[x + element.x][y + element.y] == 1)
			return 0;
	}

	//_actif.setAngle(angle);
	_actif.tourneDroite();
	return 1;
}

// Fait dessendre le bloc jusqu'à ce qu'il colisione.
void salle::tombe()
{
	int minY = 19;
	vector<Vector2i> axes;
	_actif.getAxes(axes);

	for (auto const & element : axes)
	{
		int distanceY = 0;
		do distanceY++; while
			(_occupations[element.x][element.y + distanceY++] != 1 ||
				distanceY >= minY);

		if (distanceY < minY)
			minY = distanceY;
	}
	_actif.deplace(0, minY);
}

// _En travaux_ Empèche de bouger, de tourner, etc pendant jusqu'à réactivation.
void salle::arret(const bool & permis)
{
	_permis = permis;
}

// Aublige le bloc de rester en place, prenant place à l'occupation de la salle.
void salle::colle()
{

}


/* Affichages */
// Dessine les composantes de la salle.
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


// Création de salles
void initSalle()
{

}
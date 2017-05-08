/* En-tête du fichier */
///================== */
/* Fichier:			pieces.h
// Auteur:			Olivier Lemay Dostie & Jean-Allain Sainton
// Date création:	27/04/2017
// Description:		header pour le TP3 de C++ pour la création d'objets "salles" permettant
.					l'utilisateur de joueur à une version personnalisé de Tetris. */

/* Directives au pré-processeur */
///============================ */
#pragma once		
#include <string>				
#include <assert.h>				
#include <vector>				
#include <math.h>				
#include <cstdlib>				
using namespace std;
#include <SFML/Graphics.hpp>	
#include <bloc.h>				// Contient carre.h
using namespace sf;

/* Prototype des fonctions */
///======================= */
class salle;
bool initPieces(bloc * blocsJeu, const int & nbPiece, const vector<Vector2i> pieces[NBPIECE][4]);
int alleatoire(const int & max, const int & indice);


/* Constantes de blocs */
///=================== */
const bool ok = initPieces(tetris, NBPIECE, PIECES);	// Instancie les blocs 'tetris'


/* Constantes pour les salles */
///========================== */
const int LRGPOLICE = 24;
const int NBCHARMAX = 30;				// Nb de caractère maximum dans un message
const Vector2f DIMSALLE					// Dimmension par défaut de la salle
/**/(DIMCARRE.x * LRGJEU, DIMCARRE.y * HAUJEU);
const Vector2i PIVOTSALLE				// Point pivot des salles
/**/(DIMSALLE.x / 2, DIMSALLE.y / 2);
const Vector2f POSAFFICHE = Vector2f(DIMSALLE.x + POS.x - 40, LRGPOLICE);
const Keyboard::Key CONTROLES[2][6] =	// Contrôles des joueurs en mode Multijoueur
{// haut		bas				gauche			droite			tourne gauche	tourne droite
{ Keyboard::W,	Keyboard::S,	Keyboard::A,	Keyboard::D,	Keyboard::F,	Keyboard::G },		// 1er Joueur
{ Keyboard::Up,	Keyboard::Down,	Keyboard::Left,	Keyboard::Right,Keyboard::Num0,	Keyboard::Period }	// 2e Joueur
};



/* Objet salle pour afficher ouvrir, jouer et manipuler le jeu principal */
///===================================================================== */
class salle
{
private:
	int _occupations					// Zones où les blocs ne doivent pas pouvoir passer (murs)
		[HAUJEU][LRGJEU] = { 0 };
	vector<carre> _tuiles;				// Carrés formant les zones infranchissables
	vector<bloc> _blocsFixes;			// Tout les blocs qui d'ont le joueur à perdu le contrôle

	int _nbPiece = NBPIECE;				// Nombre de pièces disponible dans le niveau
	bloc *_blocsJeu[NBPIECE];			// La liste des blocs disponibles
	bloc _blocActif;  					// Le bloc avec lequel on joue présentement
	bloc _blocProchain; 				// Le prochain bloc du jeu

	int _vitesseBloc = 1;				// Vitesse du bloc actif
	int _noNiveau = 1;					// Numéro du niveau actuel du jeu
	int _styleBlocs = 1;				// Textures, couleurs, etc. (nécessaire?)
	int _orientation = 1;				// Si on fait tourner la salle

	// Utiliser la struct joueur
	string _nomJoueur = "Joueur";		// Nom du joueur
	int _noJoueur = 1;					// Si plus qu'un joueur (peut être utilisé pour enregistrer son score)
	int _points = 0;					// Score que le joueur à accumulé
	bool _permis = true;				// Si vrai, laisse le joueur faire des actions
	int _nbBombe = 1;					// Autre option lol

	Vector2f _pos = POS;				// Position de la salle dans la fenêtre
	RectangleShape _boite;				// Lieu	où l'on affiche l'espace de jeu
	RectangleShape _information;		// Section pour affichier les statistiques


	void videOccupations();
	void videBlocsJeu();

	void placeMurs();

	bool pivote(int angle);
	bool colision(const vector<Vector2i>& axes, const int & X, const int & Y);

	void prochain();
public:
	// Instanciatieurs
	salle(string nomJoueur);
	salle(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
		string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
		const bloc * blocsJeu);

	// Destructeurs
	~salle();

	// Préparations de la salle
	void init(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
		string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
		const bloc * blocsJeu);
	void initBlocsJeu(const bloc * pieces);
	void recharche(int noNiveau);
	void demare();

	// Changements des attributs de la salle
	void initStatistiques(RenderWindow & window, bloc pieceSuivante, string nomJoueur);
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

	void setOccupationAbsolue(const vector<Vector2i> & axes);
	void setOccupationRelative(const vector<Vector2i> & axes, Vector2i place);
	void creeObstacle();
	void marcheArriere();
	void brasse();

	// Contrôles principales
	void tourne();
	void pause();
	void menu();
	void balaye();
	void tetris();
	void compresse();
	void ferme();

	// Déplacements et transformations du bloc actif
	bool bouge(int X, int Y);
	void tombe();
	bool pivoteGauche();
	bool pivoteDroite();
	void colle();
	/// Rallentis la progression du bloc. (augmente le délais avant qu'il continu sa descente)
	//void bloc::ralenti()
	//{
	//}
	void arret(const bool & permis);


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
	bloc getProchain();

	void getOccupation(vector<Vector2i> & occupation);
	void getOccupationAbsolue(vector<int> occupation, const vector<Vector2i> & axes);
	void getOccupationRelative(vector<int> occupation,
		const vector<Vector2i> & axes, Vector2i place);
	bool checkOccupationAbsolue(const vector<Vector2i> & axes);
	bool checkOccupationRelative(const vector<Vector2i> & axes, Vector2i place);

	// Affichages
	// Affiche le bloc 
	void drawActif(RenderWindow & window)
	{
		_blocActif.draw(window);
	}
	// Affiche le prochain bloc dans la fenêtre des statistiques.
	void montreProchain(RenderWindow & window)
	{
		_blocProchain.montre(window, Vector2f(
			POSAFFICHE.x + PIVOTBLOC.x,
			POSAFFICHE.y + PIVOTBLOC.y * 2));
	}
	void afficherInterface(RenderWindow & window);
	void afficheBlocsSalle(RenderWindow & window);
};

// Instanciations de base */
///====================== */
// Instancie une salle avec seulement le nom du joueur.
salle::salle(string nomJoueur)
{
	setNomJoueur(nomJoueur);
	initBlocsJeu(TETRIS);
	placeMurs();
}

// Instancie la salle avec toutes ses valeurs.
salle::salle(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
	string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
	const bloc * blocsJeu)
{
	init(pos, noNiveau, orientation, occupation, nomJoueur,
		noJoueur, points, nbBombe, vitesse, blocsJeu);
}

/* Destructeurs */
///============ */
// Destructeur principal.
salle::~salle()
{
	Vector2f zero = Vector2f(0, 0);
	Color invisible = Color(0, 0, 0);
	Texture texture;

	_boite.setSize(zero);
	_boite.setOutlineThickness(0);
	_boite.setOutlineColor(invisible);
	_boite.setPosition(zero);

	_information.setSize(zero);
	_information.setOutlineThickness(0);
	_information.setOutlineColor(invisible);
	_information.setPosition(zero);

	/// Pourquoi est-ce qu'on load une texture quand on veux les enlevers?
	if (!texture.loadFromFile("Tetris-Background_3.jpg", IntRect(50, 25, 300, 100)));
	_boite.setTexture(&texture);
	_information.setTexture(&texture);

	_nomJoueur = "";
	_pos.x = _pos.y = _noNiveau = _noJoueur = _points = _nbBombe =
		_styleBlocs = _orientation = _vitesseBloc = 0;
	videOccupations();
	videBlocsJeu();
}

// Destructeur pour _occupations.
void salle::videOccupations()
{
	_tuiles.resize(0);
	_blocsFixes.resize(0);
	for (int i = 0; i < HAUJEU; i++)
		for (int j = 0; j < LRGJEU; j++)
			_occupations[i][j] = { 0 };
}

// Destructeur pour _blocsJeu.
void salle::videBlocsJeu()
{
	vector<carre> vide;
	vector<Vector2i> axes[4];
	for (int i = 0; i < NBPIECE; i++)
		(*_blocsJeu)[i].setFormes(vide, axes);
}

/* Modificateurs de la salle */
///========================= */
// Initialise la salle avec toutes ses valeurs.
void salle::init(Vector2f pos, int noNiveau, int orientation, vector<Vector2i> occupation,
	string nomJoueur, int noJoueur, int points, int nbBombe, int vitesse,
	const bloc * blocsJeu)
{
	setPos(pos);
	setNoNiveau(noNiveau);
	setOrientation(orientation);
	setOccupationAbsolue(occupation);
	setNoJoueur(noJoueur);
	setPoints(points);
	setNbBombe(nbBombe);
	setVitesse(vitesse);
	initBlocsJeu(blocsJeu);

	placeMurs();
}

// Remet la salle à ses valeurs de base en changeant son numéro.
void salle::recharche(int noNiveau)
{
	videOccupations();
	placeMurs();

	setNoNiveau(noNiveau);
	initBlocsJeu(TETRIS);
}

// Charge les blocs par défaut dans la salle puis choisi les premiers blocs du jeu.
void salle::initBlocsJeu(const bloc * pieces)
{
	bloc test;
	for (int i = 0; i < NBPIECE; i++)
	{
		*(_blocsJeu + i) = new bloc(*(pieces + i));
	}
	setActif(**(_blocsJeu + (alleatoire(NBPIECE, -1))));
	setProchain(_blocActif);
	prochain();
}

// Active les procédures du démarage du jeu.
void salle::demare()
{

}

// Ajoude des tuiles aux murs et à la base pour qu'ils deviennent infranchissables.
void salle::placeMurs()
{
	// Murs
	for (int y = 0; y < HAUJEU; y++)
	{
		_occupations[y][LRGJEU - 1] = 1;
		_tuiles.push_back(carre(_pos, Vector2i(LRGJEU - 1, y), COIN,
			PALETTES[2][rand() % 6], 3));
		_occupations[y][0] = 1;
		_tuiles.push_back(carre(_pos, Vector2i(0, y), COIN,
			PALETTES[2][rand() % 6], 3));
	}
	// Plancher
	for (int x = 0; x < LRGJEU; x++)
	{
		_occupations[HAUJEU - 1][x] = 1;
		_tuiles.push_back(carre(_pos, Vector2i(x, HAUJEU - 1), COIN,
			PALETTES[2][rand() % 6], 3));
	}
}

Text initText(Font police, string message, int mesure, 
	Color couleur, Vector2f & pos, Vector2f decalage)
{
	Text text(message, police, mesure);
	text.setFillColor(couleur);
	pos.x += decalage.x;
	pos.y += decalage.y;
	text.setPosition(pos);
	return text;
}

/*Change les attributs de la salle*/
/// On devrait séparer l'initialisation et l'affichage pur n'avoir qu'à réinitialiser les choses qui changent.
void salle::initStatistiques(RenderWindow & window, bloc pieceSuivante, string nomJoueur)
{
	Vector2f posAffiche(POSAFFICHE.x - 15, POSAFFICHE.y);
	Vector2f decalage = BASE;

	Font police;
	police.loadFromFile("font_arial.ttf");
	
	Color couleurBordStat = Color::Red;
	Color couleurStat = Color(255, 255, 255, 80);
	float bordure = 10;
	
	RectangleShape boiteStat(Vector2f(300, DIMSALLE.y + LRGPOLICE * 5));
	boiteStat.setFillColor(couleurStat);

	Text prochaine = initText(police, "Prochaine piece", LRGPOLICE, 
		Color::Black, posAffiche, decalage);

	decalage = Vector2f(LRGPOLICE, LRGPOLICE * 2);
	RectangleShape fenetrePiecesSuivante
		(Vector2f(PIVOTBLOC.x * 2.2, PIVOTBLOC.y * 2.2 + LRGPOLICE));
	fenetrePiecesSuivante.setFillColor(Color::Black);

	/// Utiliser ici une fonction pour afficher la prochaine pièce au bon endroit (à réutiliser)


	decalage = BASE;
	decalage.y = PIVOTBLOC.y * 2.2 + LRGPOLICE * 2;
	Text textLevel = initText(police, "Votre level : ", LRGPOLICE,
		Color::Black, posAffiche, decalage);

	decalage.y = LRGPOLICE;
	Text textNom = initText(police, _nomJoueur, LRGPOLICE,
		Color::Black, posAffiche, decalage);
	
	Text textLigne = initText(police, "Nombre de ligne \nReussi : ", LRGPOLICE,
		Color::Black, posAffiche, decalage);

	decalage.y = LRGPOLICE * 2;
	Text textScore = initText(police, "Score : ", LRGPOLICE,
		Color::Black, posAffiche, decalage);

	decalage.y = LRGPOLICE * 3;
	Text textAide = initText(police, 
		"     Commande \n Z : Tourne a gauche \n X : Tourne a droite \n P : Pause \n M : Mute \n T : Prochaine musique \n Q : Unmute \n Esc : Menu", 
		LRGPOLICE, Color::Black, posAffiche, decalage);
	
	// window.draw(text);
	// window.draw(boiteStatistiques);
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
	_blocActif = actif;
}

// 
void salle::setProchain(bloc prochain)
{
	_blocProchain = prochain;
}

// 
void salle::setOccupationAbsolue(vector<Vector2i> const& axes)
{
	for (auto const &element : axes)
		_occupations[element.x][element.y] = 1;
}

// 
void salle::setOccupationRelative(vector<Vector2i> const & axes, Vector2i place)
{
	for (Vector2i const & element : axes)
		_occupations[place.x + element.x][place.y + element.y] = 1;
}

// Alterne le bloc actif par le prochain et trouve aléatoirement le prochain différent bloc.
void salle::prochain()
{
	setActif(_blocProchain);
	int piece = alleatoire(NBPIECE, _blocProchain.getPiece());
	while (piece == _blocActif.getPiece())
		piece = alleatoire(NBPIECE, _blocProchain.getPiece());

	setProchain(**(_blocsJeu + piece));
}

/* Récupérations des attributs de la salle */
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
	return _blocActif;
}

// 
bloc salle::getProchain()
{
	return _blocProchain;
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

/* Contrôles principales */
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
// Bouge le bloc d'une distance en x si elle n'entre pas en conflit avec la salle.
bool salle::bouge(int X, int Y)
{
	int x = _blocActif.getPlace().x,
		y = _blocActif.getPlace().y;
	vector<Vector2i> axes;		/// Trouver un moyen d'accéder au profil plus facilement
	_blocActif.getAxes(axes, _blocActif.getAngle());

	for (Vector2i const & element : axes)
	{	/// Question: Pourquoi quand "element" est constant, on ne peut utiliser de méthodes ?
		if (_occupations[y + element.y + Y][x + element.x + X] == 1)
			return 0;
	}

	_blocActif.deplace(X, Y);
	return colision(axes, X, Y);
}

// Vérifie si le bloc colisionnerais dans une dirrection
bool salle::colision(const vector<Vector2i> & axes, const int & X, const int & Y)
{
	int x = _blocActif.getPlace().x,
		y = _blocActif.getPlace().y;

	for (Vector2i const & element : axes)
	{	/// Question: Pourquoi quand "element" est constant, on ne peut utiliser de méthodes ?
		if (_occupations[y + element.y + Y][x + element.x + X] == 1)
			return 1;
	}
	return 0;
}

// Trouve l'angle au sens antihoraire cible et effectue la maneuvre si possible.
bool salle::pivoteGauche()
{
	int angle = _blocActif.getAngle();
	if (++angle > 3)
		angle = 0;

	if (pivote(angle))
	{
		_blocActif.tourneGauche();
		return 1;
	}
	return 0;
}

// Trouve l'angle au sens horaire cible et effectue la maneuvre si possible.
bool salle::pivoteDroite()
{
	int angle = _blocActif.getAngle();
	if (--angle < 0)
		angle = 3;

	if (pivote(angle))
	{
		_blocActif.tourneDroite();
		return 1;
	}
	return 0;
}

// Vérifie si le bloc peux tourner dans la salle.
bool salle::pivote(int angle)
{
	int x = _blocActif.getPlace().x,
		y = _blocActif.getPlace().y;
	vector<Vector2i>profil;
	_blocActif.getAxes(profil, angle);

	for (Vector2i const & element : profil)
	{
		if (_occupations[x + element.x][y + element.y] == 1)
			return 0;
	}

	_blocActif.setAngle(angle);
	return 1;
}

// Fait dessendre le bloc jusqu'à ce qu'il colisione.
void salle::tombe()
{
	int minY = HAUJEU;
	vector<Vector2i> axes;
	_blocActif.getAxes(axes, _blocActif.getAngle());

	for (Vector2i const & element : axes)
	{
		int distanceY = 0;
		do distanceY++; while
			(_occupations[element.x][element.y + distanceY++] != 1 ||
				distanceY >= minY);

		if (distanceY < minY)
			minY = distanceY;
	}
	_blocActif.deplace(0, minY);
}

// _En travaux_ Empèche de bouger, de tourner, etc pendant jusqu'à réactivation.
void salle::arret(const bool & permis)
{
	_permis = permis;
}

// Aublige le bloc de rester en place, prenant place à l'occupation de la salle.
void salle::colle()
{
	vector<Vector2i> axes;
	_blocActif.getAxes(axes, _blocActif.getAngle());
	/// À cause de la redondance, mémoriser les axes dans la salle su lieu?
	setOccupationRelative(axes, _blocActif.getPlace());
	_blocsFixes.push_back(_blocActif);
	prochain();
}

// Dessine les composantes de la salle.
void salle::afficherInterface(RenderWindow & window)
{

	//window.draw(_boite);
	//window.draw(_information);

	_blocProchain.montre(window, Vector2f(POSAFFICHE.x, POSAFFICHE.y + 30));
}

// 
void salle::afficheBlocsSalle(RenderWindow & window)
{
	for (carre & element : _tuiles)
		element.draw(window);
	for (bloc & element : _blocsFixes)
		element.draw(window);
}


/* Autres structures */
///================= */

// Données relative à un joueur.
struct identite
{
	string nomJoueur;
	bool nouveauJoueur = true;
	int level = 1,
		score = 0,
		rang = 0;
};


// Crée des formes pour les manipuler des essais, des tests etc.
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
	Vector2f position = test.shape.getPosition();
	// = (15, 55)

	// rotate the entity relatively to its current orientation
	test.shape.rotate(10);

	// retrieve the absolute rotation of the entity
	float rotation = test.shape.getRotation();
	// = 55

	// scale the entity relatively to its current scale
	test.shape.scale(0.8f, 0.4f);

	// retrieve the absolute scale of the entity
	Vector2f scale = test.shape.getScale();
	// = (2, 0.8)
}

/* Fonctions */
///========= */
// Création de salles (nécessaire?)
void initSalle()
{

}

// Construit les pièces par défaut à partir des coordonnées du tableau PIECES.
bool initPieces(bloc * blocsJeu, const int & nbPiece, const vector<Vector2i> pieces[NBPIECE][4])
{
	vector<carre> formes;		// Carrés de la forme
	vector<Vector2i> axes[4];	// Coordonnées des carrés sous chaque angles

	// Crée un bloc avec les coordonnés du tableau donné
	for (int f = 0; f < nbPiece; f++)
	{
		// Forme les carrés de la pièce (selon l'angle 1)
		for (int c = 0; c < 4; c++)
			formes.push_back(carre(POS, PLACE, pieces[f][1][c], PALETTES[2][f], 2));

		// Ajoute les coordonnées des carrés de pièce aux axes
		for (int a = 0; a < 4; a++)
			for (int c = 0; c < 4; c++)
				axes[a].push_back(pieces[f][a][c]);

		// Initialise des blocs avec les vectors formés
		*(blocsJeu + f) = bloc(PLACE, -3, 1, -1, f, 1, formes, axes);

		// Vide la mémoire des vectors avant la prochaine pièce
		formes.resize(0);
		for (int i = 0; i < 4; i++)
			axes[i].resize(0);
	}
	return 1;
}

// Obtien un nomre aléatoire différent du précédent.
int alleatoire(const int & max, const int & indice)
{
	int nouveau = rand() % max;
	while (nouveau == indice)
		nouveau = rand() % max;

	return nouveau;
}
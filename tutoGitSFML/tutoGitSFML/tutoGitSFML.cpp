
/* En-tete du programme		(version d'Olivier) ++
>>>>>>> refs/remotes/origin/master
=======================
Programme:      tutoGitsSFML.cpp
Auteur:			Jean-Alain Sainton & Olivier Lemay Dostie
Date crÃ©ation:	16/04/2017
Description:	Programme des essais SFML et de connection Git pour le projet final en C++
				Espace de développement pour l'application. */


				/* Directives au prÃ©-processeur.
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
#include <SFML/Audio.hpp>
				//#include <SDL.h>			//Pas inclus
				//#include <SDL_ttf.h>		//Pas inclus
				//#include <saisieSDL.h>	//Non fonctionnel si pas inclus
#include <SFML\Graphics.hpp>
using namespace sf;	//P-e que std serait plus appropriÃ©, mais on peut alterner

//Constantes des objets
const int NBCHARMAX = 30,	//Nombre de caractÃ¨re maximum dans un message
PROCHAINS = 3;		//Nombre de prochai bloc visible

struct coord
{
	int x = 5,		//CoordonnÃ©e du message en x dans la fenÃªtre
		y = 5;		//CoordonnÃ©e du message en y dans la fenÃªtre
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
//	coord pos,		//CoordonnÃ©es
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
	coord _pos = initCoord(40, 20),	//Dans la fenÃªtre
		_place = initCoord(3, 10),	//Dans la salle
		_encrage = initCoord(2, 2);	//Point pivot du bloc
	int _id = 0,			//
		_styleBloc = 1,		//
		_vitesse = 1,		//
		_etat = 1,			//
		_forme = 1,			//
		_angle = 1,			//Orientation actuelle de la forme
		_axes[4][5][5] = {	//Composition de la forme selon chaque angles
			//270Â° vers la gauche (couchÃ© sur la droite)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
			},
			//Angle par dÃ©faut (debout)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
			},
			//90Â° vers la gauche (couchÃ© sur sa gauche)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
			},
			//180Â° sur elle-mÃªme (sur Ã§a tÃªte)
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
	int getX()
	{
		return _pos.x;
	}
	int getY()
	{
		return _pos.y;
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
	int bougeY(int &y, int &x, int salle[20][20])
	{

		bool vide = true;
		int sens = 1;
		coord depart ;
		depart.x = 205;
		depart.y = 0;
		coord fin = depart;
		fin.x += 5;
		fin.y = 20;
		if (y < 0)
			sens *= swapCoord(depart, fin);

		int i = y;
		int j = x;

		if (salle[i][j + 1] == 1)
		{
			vide = false;
			return 0;

		}


		_place.y += y * 2;

	}
	void bougeX(int &x, int salle[20][20])
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
	string _nomJoueur = "Joueur";	//
	sf::RectangleShape _boite;
	sf::RectangleShape _information;	//
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


public:
	salle()
	{
		_boite.setSize(sf::Vector2f(600, 763));
		
		_boite.setOutlineThickness(10);
		_boite.setOutlineColor(sf::Color::Red);
		_boite.setPosition(20, 20);
		sf::Texture texture;
		if (!texture.loadFromFile("Tetris-Background_3.jpg", sf::IntRect(50, 25, 300, 100)));
		_boite.setTexture(&texture);

		_information.setTexture(&texture);
		_information.setSize(sf::Vector2f(300, 760));

		_information.setOutlineThickness(10);
		_information.setOutlineColor(sf::Color::Red);
		_information.setPosition(650, 20);

	}
	~salle()
	{
		_nomJoueur = "";

		_noNiveau = _noJoueur = _points = _nbBombe =
			_styleBlocs = _orientation = _vitesseBloc = /*_bloc = */0;
		/*_procains[PROCHAINS] = _posBlocs[200][3] = */_occupation[20][20] = { 0 };
		_boite.setSize(sf::Vector2f(0, 0));
		
		_boite.setOutlineThickness(0);
		_boite.setOutlineColor(sf::Color(0, 0, 0));
		_boite.setPosition(0, 0);

		sf::Texture texture;
		if (!texture.loadFromFile("Tetris-Background_3.jpg", sf::IntRect(50, 25, 300, 100)));
		_boite.setTexture(&texture);

		_information.setTexture(&texture);
		_information.setSize(sf::Vector2f(0, 0));
		
		_information.setOutlineThickness(0);
		_information.setOutlineColor(sf::Color(0, 0, 0));
		_information.setPosition(0, 0);
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
	void afficherInterface(sf::RenderWindow &window)
	{
		window.draw(_boite);
		window.draw(_information);



	}

	void modifierInterface(sf::RenderWindow &window, RectangleShape forme[][5], int profil[][5], string nomJoueur)
	{

		sf::RectangleShape interfacePieceSuivante(sf::Vector2f(150, 150));
		sf::RectangleShape afficherPieceSuivante(sf::Vector2f(300, 750));
		sf::Font font;
		font.loadFromFile("font_arial.ttf"); // choix de la police à utiliser
		sf::Text text;
		text.setFont(font);

		afficherPieceSuivante.setFillColor(sf::Color::White);
		afficherPieceSuivante.setOutlineThickness(10);
		afficherPieceSuivante.setOutlineColor(sf::Color::Red);
		afficherPieceSuivante.setPosition(685, 25);
		window.draw(afficherPieceSuivante);

		interfacePieceSuivante.setFillColor(sf::Color::Black);
		interfacePieceSuivante.setOutlineThickness(10);
		interfacePieceSuivante.setOutlineColor(sf::Color::Red);
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
					forme[i][j].setSize(sf::Vector2f(24, 24));
					forme[i][j].setPosition(745 + i * 26, 80 + j * 26);
					forme[i][j].setFillColor(sf::Color::Red);
					forme[i][j].setOutlineThickness(1);
					forme[i][j].setOutlineColor(sf::Color(0, 0, 0));
					window.draw(forme[i][j]);
				}
			}
		}
		




		
		text.setString("Prochaine piece"); 	// choix de la chaîne de caractères à afficher
		text.setCharacterSize(24); // choix de la taille des caractères exprimée en pixels, pas en points !						 
		text.setColor(sf::Color::Black);   // choix de la couleur du texte
		text.setStyle(sf::Text::Bold); 	// choix du style du texte
		text.setPosition(720, 25);		// position du texte
		window.draw(text);

		sf::Text textLevel;
		textLevel.setFont(font);  // choix de la police à utiliser				
		textLevel.setString("Votre level : "); // choix de la chaîne de caractères à afficher
		textLevel.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textLevel.setColor(sf::Color::Black);    // choix de la couleur du texte
		textLevel.setStyle(sf::Text::Bold); 	// choix du style du texte
		textLevel.setPosition(700, 300);		// position du texte
		window.draw(textLevel);

		sf::Text textNom;
		textLevel.setFont(font);  // choix de la police à utiliser				
		textLevel.setString(nomJoueur); // choix de la chaîne de caractères à afficher
		textLevel.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textLevel.setColor(sf::Color::Black);    // choix de la couleur du texte
		textLevel.setStyle(sf::Text::Bold); 	// choix du style du texte
		textLevel.setPosition(700, 350);		// position du texte
		window.draw(textLevel);


		sf::Text textLigne;
		textLigne.setFont(font);  // choix de la police à utiliser				
		textLigne.setString("Nombre de ligne \nReussi : "); // choix de la chaîne de caractères à afficher
		textLigne.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textLigne.setColor(sf::Color::Black);    // choix de la couleur du texte
		textLigne.setStyle(sf::Text::Bold); 	// choix du style du texte
		textLigne.setPosition(700, 400);		// position du texte
		window.draw(textLigne);

		sf::Text textScore;
		textScore.setFont(font);  // choix de la police à utiliser				
		textScore.setString("Score : "); // choix de la chaîne de caractères à afficher
		textScore.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textScore.setColor(sf::Color::Black);    // choix de la couleur du texte
		textScore.setStyle(sf::Text::Bold); 	// choix du style du texte
		textScore.setPosition(700, 500);		// position du texte
		window.draw(textScore);

		sf::Text textAide;
		textAide.setFont(font);  // choix de la police à utiliser				
		textAide.setString("     Commande \n Z : Tourne a gauche \n X : Tourne a droite \n P : Pause \n M : Mute \n T : Prochaine musique \n Q : Unmute \n Esc : Menu"); // choix de la chaîne de caractères à afficher
		textAide.setCharacterSize(24); //choix de la taille des caractères exprimée en pixels, pas en points !
		textAide.setColor(sf::Color::Black);    // choix de la couleur du texte
		textAide.setStyle(sf::Text::Bold); 	// choix du style du texte
		textAide.setPosition(700, 550);		// position du texte
		window.draw(textAide);
		
	}

	void load()
	{}
	void demare()
	{}
	void pause()
	{}
	void menu()
	{}
	int prochain()
	{
		srand(time(NULL));
		int numBlock = rand() % 6 + 1;
		return numBlock;
	}
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
				forme[i][j].setSize(sf::Vector2f(24, 24));
				forme[i][j].setPosition(pos.x + i * 26, pos.y + j * 26);
				forme[i][j].setFillColor(sf::Color::Green);
				forme[i][j].setOutlineThickness(1);
				forme[i][j].setOutlineColor(sf::Color(0, 0, 0));
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
int menuOption(sf::RenderWindow &window, Music &music, string &nomJoueur)
{

	window.clear(sf::Color::Black);
	window.display();
	sf::Texture texture;

	sf::RectangleShape btnMusique(sf::Vector2f(300, 100));
	sf::RectangleShape btnMusiquePowerOn(sf::Vector2f(50, 50));
	sf::RectangleShape btnMusiquePowerOff(sf::Vector2f(50, 50));
	sf::RectangleShape btnMusiqueChoix(sf::Vector2f(200, 50));
	sf::RectangleShape btnMenu(sf::Vector2f(100, 50));
	sf::RectangleShape btnNomJoueur(sf::Vector2f(300, 125));
	sf::RectangleShape btnAutre(sf::Vector2f(300, 100));

	sf::Font font;
	sf::Text text;


	if (!texture.loadFromFile("Tetris-Background.jpg"));

	sf::Sprite background(texture);


	window.draw(background);
	font.loadFromFile("font_arial.ttf"); // choix de la police à utiliser
	text.setFont(font);
	text.setCharacterSize(30); // choix de la taille des caractères exprimée en pixels, pas en points !					
	text.setStyle(sf::Text::Bold); 	// choix du style du texte


	btnMusique.setFillColor(sf::Color::White);
	btnMusique.setOutlineThickness(0);
	btnMusique.setOutlineColor(sf::Color::Red);
	btnMusique.setPosition(350, 150);

	if (!texture.loadFromFile("button.png", sf::IntRect(50, 25, 300, 100)));
	btnMusique.setTexture(&texture);
	texture.setSmooth(true);

	text.setString("Musique"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(450, 188);		// position du texte
	text.setColor(sf::Color::Black);   // choix de la couleur du texte


	window.draw(btnMusique);
	window.draw(text);


	btnMusiquePowerOn.setFillColor(sf::Color::Magenta);
	btnMusiquePowerOn.setOutlineThickness(0);
	btnMusiquePowerOn.setOutlineColor(sf::Color::Red);
	btnMusiquePowerOn.setPosition(350, 275);

	/*if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));*/
	btnMusiquePowerOn.setTexture(&texture);
	texture.setSmooth(true);

	text.setString("On"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(352, 278);		// position du texte
	text.setColor(sf::Color::Black);   // choix de la couleur du texte
	window.draw(btnMusiquePowerOn);
	window.draw(text);

	btnMusiquePowerOff.setFillColor(sf::Color::White);
	btnMusiquePowerOff.setOutlineThickness(0);
	btnMusiquePowerOff.setOutlineColor(sf::Color::Red);
	btnMusiquePowerOff.setPosition(600, 275);

	//if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));
	btnMusiquePowerOff.setTexture(&texture);


	text.setString("Off"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(605, 278);		// position du texte
	text.setColor(sf::Color::Black);   // choix de la couleur du texte
	window.draw(btnMusiquePowerOff);
	window.draw(text);


	btnMusiqueChoix.setFillColor(sf::Color::White);
	btnMusiqueChoix.setOutlineThickness(0);
	btnMusiqueChoix.setOutlineColor(sf::Color::Red);
	btnMusiqueChoix.setPosition(350, 355);
	/*if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));*/
	btnMusiqueChoix.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 362);		// position du texte
	text.setColor(sf::Color::Black);   // choix de la couleur du texte


	window.draw(btnMusiqueChoix);
	window.draw(text);

	btnMusiqueChoix.setFillColor(sf::Color::White);
	btnMusiqueChoix.setOutlineThickness(0);
	btnMusiqueChoix.setOutlineColor(sf::Color::Red);
	btnMusiqueChoix.setPosition(350, 420);
	/*if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));*/
	btnMusiqueChoix.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 422);		// position du texte
	text.setColor(sf::Color::Black);   // choix de la couleur du texte


	window.draw(btnMusiqueChoix);
	window.draw(text);


	btnMusiqueChoix.setFillColor(sf::Color::White);
	btnMusiqueChoix.setOutlineThickness(0);
	btnMusiqueChoix.setOutlineColor(sf::Color::Red);
	btnMusiqueChoix.setPosition(350, 485);
	/*if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));*/
	btnMusiqueChoix.setTexture(&texture);
	texture.setSmooth(true);
	text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(355, 487);		// position du texte
	text.setColor(sf::Color::Black);   // choix de la couleur du texte

	window.draw(btnMusiqueChoix);
	window.draw(text);

	btnMenu.setFillColor(sf::Color::White);
	btnMenu.setOutlineThickness(0);
	btnMenu.setOutlineColor(sf::Color::Red);
	btnMenu.setPosition(10, 10);

	/*if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));*/
	btnMenu.setTexture(&texture);
	texture.setSmooth(true);

	text.setString("Menu"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(15, 15);		// position du texte
	text.setColor(sf::Color::Black);   // choix de la couleur du texte
	window.draw(btnMenu);
	window.draw(text);


	btnNomJoueur.setFillColor(sf::Color::White);
	btnNomJoueur.setOutlineThickness(0);
	btnNomJoueur.setOutlineColor(sf::Color::Red);
	btnNomJoueur.setPosition(350, 550);

	//if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));
	btnNomJoueur.setTexture(&texture);
	texture.setSmooth(true);

	text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 565);		// position du texte
	window.draw(btnNomJoueur);
	window.draw(text);

	text.setString(nomJoueur); 	// choix de la chaîne de caractères à afficher
	text.setPosition(375, 625);		// position du texte
	window.draw(text);
	window.display();





	font.loadFromFile("font_arial.ttf");
	text.setFont(font);
	
	sf::Event event;
	while (window.isOpen())
	{

		while (window.pollEvent(event))
		{
			
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				return -1;
				break;
			case sf::Event::MouseButtonPressed:


				Mouse::getPosition(window);
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 400) && (event.mouseButton.y > 275 && event.mouseButton.y < 325))
				{
					//Option musique on
					window.clear(sf::Color::Black);
					window.display();
					music.setVolume(100);
					btnMusiquePowerOn.setFillColor(sf::Color::Magenta);
					btnMusiquePowerOn.setOutlineThickness(0);
					btnMusiquePowerOn.setOutlineColor(sf::Color::Red);
					btnMusiquePowerOn.setPosition(350, 275);

					/*if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));*/
					btnMusiquePowerOn.setTexture(&texture);
					texture.setSmooth(true);

					text.setString("On"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(352, 278);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiquePowerOn);
					window.draw(text);


					btnMusiquePowerOff.setFillColor(sf::Color::White);
					btnMusiquePowerOff.setOutlineThickness(0);
					btnMusiquePowerOff.setOutlineColor(sf::Color::Red);
					btnMusiquePowerOff.setPosition(600, 275);

					/*if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));*/
					btnMusiquePowerOff.setTexture(&texture);


					text.setString("Off"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(605, 278);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiquePowerOff);
					window.draw(text);


					window.display();

				}

				else if ((event.mouseButton.x > 600 && event.mouseButton.x < 650) && (event.mouseButton.y > 275 && event.mouseButton.y < 325))
				{
					window.clear(sf::Color::Black);
					window.display();
					music.setVolume(0);
					btnMusiquePowerOn.setFillColor(sf::Color::White);
					btnMusiquePowerOn.setOutlineThickness(0);
					btnMusiquePowerOn.setOutlineColor(sf::Color::Red);
					btnMusiquePowerOn.setPosition(350, 275);

					/*if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));*/
					btnMusiquePowerOn.setTexture(&texture);
					texture.setSmooth(true);

					text.setString("On"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(352, 278);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiquePowerOn);
					window.draw(text);



					btnMusiquePowerOff.setFillColor(sf::Color::Magenta);
					btnMusiquePowerOff.setOutlineThickness(0);
					btnMusiquePowerOff.setOutlineColor(sf::Color::Red);
					btnMusiquePowerOff.setPosition(600, 275);

					/*if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 300, 100)));*/
					btnMusiquePowerOff.setTexture(&texture);


					text.setString("Off"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(605, 278);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiquePowerOff);
					window.draw(text);

					window.display();

				}

				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 355 && event.mouseButton.y < 405))
				{
					window.clear(sf::Color::Black);
					window.display();
					music.openFromFile("music.ogg");
					music.play();

					btnMusiqueChoix.setPosition(350, 355);
					btnMusiqueChoix.setFillColor(sf::Color::Magenta);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);

					btnMusiqueChoix.setPosition(350, 420);
					btnMusiqueChoix.setFillColor(sf::Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);

					btnMusiqueChoix.setPosition(350, 485);
					btnMusiqueChoix.setFillColor(sf::Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);

					window.display();

				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 420 && event.mouseButton.y < 470))
				{
					window.clear(sf::Color::Black);
					window.display();
					music.openFromFile("music2.ogg");
					music.play();

					btnMusiqueChoix.setPosition(350, 355);
					btnMusiqueChoix.setFillColor(sf::Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);


					btnMusiqueChoix.setPosition(350, 420);
					btnMusiqueChoix.setFillColor(sf::Color::Magenta);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);

					btnMusiqueChoix.setPosition(350, 485);
					btnMusiqueChoix.setFillColor(sf::Color::White);
					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);

					window.display();

				}
				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 550) && (event.mouseButton.y > 485 && event.mouseButton.y < 535))
				{
					window.clear(sf::Color::Black);
					window.display();
					music.openFromFile("music3.ogg");
					music.play();

					btnMusiqueChoix.setPosition(350, 355);
					btnMusiqueChoix.setFillColor(sf::Color::White);
					text.setString("Musique 1"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 357);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);

					btnMusiqueChoix.setPosition(350, 420);
					btnMusiqueChoix.setFillColor(sf::Color::White);
					text.setString("Musique 2"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 422);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte
					window.draw(btnMusiqueChoix);
					window.draw(text);


					btnMusiqueChoix.setPosition(350, 485);
					btnMusiqueChoix.setFillColor(sf::Color::Magenta);

					text.setString("Musique 3"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(355, 487);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte

					window.draw(btnMusiqueChoix);
					window.draw(text);

					window.display();


				}

				else if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 550 && event.mouseButton.y < 700))
				{
					sf::RenderWindow window2(sf::VideoMode(500, 500), "TETRIS v1.2 Nom du joueur");
					font.loadFromFile("font_arial.ttf");
					if (!texture.loadFromFile("Tetris-Background_2.jpg"));
					sf::Sprite background(texture);
					window2.draw(background);
					font.loadFromFile("font_arial.ttf");
					text.setFont(font);
					text.setString("Quel est votre nom?"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(100, 15);		// position du texte
					text.setColor(sf::Color::Black);   // choix de la couleur du texte

					window2.draw(text);

					window2.display();

					std::string s;
					while (window2.isOpen())
					{

						//Event processing.
						sf::Event event;
						while (window2.pollEvent(event)) {
							if (event.type == sf::Event::Closed)
								window2.close();
							if (event.type == sf::Event::TextEntered) {
								if (event.key.code == 8 && s.size() != 0)
								{
									s.pop_back();


								}
								else if (event.key.code == 13 && s.size() != 0)
								{
									nomJoueur = s;
									window2.close();
								}
								else if (event.text.unicode < 128)
								{
									s.push_back((char)event.text.unicode);

								}
								btnMusiqueChoix.setPosition(100, 50);
								btnMusiqueChoix.setFillColor(sf::Color::White);
								btnMusiqueChoix.setOutlineColor(sf::Color::Transparent);



								text.setString(s); 	// choix de la chaîne de caractères à afficher
								text.setPosition(100, 50);		// position du texte
								text.setColor(sf::Color::Black);   // choix de la couleur du texte
								window2.display();
								window2.draw(btnMusiqueChoix);
								window2.draw(text);
								window2.display();



							}
						}
					}
					if (!texture.loadFromFile("button.png", sf::IntRect(50, 25, 300, 100)));
					btnNomJoueur.setFillColor(sf::Color::White);
					btnNomJoueur.setOutlineThickness(0);
					btnNomJoueur.setOutlineColor(sf::Color::Red);
					btnNomJoueur.setPosition(350, 550);


					btnNomJoueur.setTexture(&texture);
					texture.setSmooth(true);
					font.loadFromFile("font_arial.ttf");
					text.setFont(font);
					text.setString("Nom du joueur :"); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 565);		// position du texte

					window.display();
					window.draw(btnNomJoueur);
					window.draw(text);
					window.display();


					text.setString(nomJoueur); 	// choix de la chaîne de caractères à afficher
					text.setPosition(375, 625);		// position du texte
					window.display();
					window.draw(text);
					window.display();


				}


				else if ((event.mouseButton.x > 15 && event.mouseButton.x < 105) && (event.mouseButton.y > 15 && event.mouseButton.y < 65))
				{
					return 0;

				}
				break;

			default:
				break;
			}

		}

	}


}
int choixMenu(sf::RenderWindow &windowMenu, Music &music, string &nomJoueur)
{
	sf::Event event;
	while (windowMenu.isOpen())
	{


		while (windowMenu.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				windowMenu.close();
				return -1;
				break;
			case sf::Event::MouseButtonPressed:


				Mouse::getPosition(windowMenu);
				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 150 && event.mouseButton.y < 250))
				{
					return 1;
				}

				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 300 && event.mouseButton.y < 400))
				{
					menuOption(windowMenu, music, nomJoueur);
					return 0;
				}

				if ((event.mouseButton.x > 350 && event.mouseButton.x < 650) && (event.mouseButton.y > 600 && event.mouseButton.y < 700))
				{
					windowMenu.close();
					return -1;
				}
				break;

			default:
				break;
			}

		}

	}
}

void afficherMenu(Music &music, string &nomJoueur)
{
	sf::RenderWindow windowMenu(sf::VideoMode(1000, 800), "TETRIS v1.2");
	sf::Texture texture;

	sf::RectangleShape btnCommencer(sf::Vector2f(300, 100));
	sf::RectangleShape btnOption(sf::Vector2f(300, 100));
	sf::RectangleShape btnScore(sf::Vector2f(300, 100));
	sf::RectangleShape btnQuitter(sf::Vector2f(300, 100));

	sf::Font font;
	sf::Text text;




	
	do
	{
		

		if (!texture.loadFromFile("Tetris-Background.jpg"));

		sf::Sprite background(texture);
		windowMenu.draw(background);

		font.loadFromFile("font_arial.ttf"); // choix de la police à utiliser
		text.setFont(font);
		text.setCharacterSize(60); // choix de la taille des caractères exprimée en pixels, pas en points !					
		text.setStyle(sf::Text::Bold); 	// choix du style du texte

		text.setString("T"); 	// choix de la chaîne de caractères à afficher
		text.setPosition(350, 10);		// position du texte
		text.setColor(sf::Color::Cyan);   // choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("E"); 	// choix de la chaîne de caractères à afficher
		text.setPosition(385, 10);		// position du texte
		text.setColor(sf::Color::Red);   // choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("T"); 	// choix de la chaîne de caractères à afficher
		text.setPosition(420, 10);		// position du texte
		text.setColor(sf::Color::Green);   // choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("R"); 	// choix de la chaîne de caractères à afficher
		text.setPosition(455, 10);		// position du texte
		text.setColor(sf::Color::Cyan);   // choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("I"); 	// choix de la chaîne de caractères à afficher
		text.setPosition(490, 10);		// position du texte
		text.setColor(sf::Color::White);   // choix de la couleur du texte
		windowMenu.draw(text);
		text.setString("S"); 	// choix de la chaîne de caractères à afficher
		text.setPosition(500, 10);		// position du texte
		text.setColor(sf::Color::Yellow);   // choix de la couleur du texte
		windowMenu.draw(text);

		text.setColor(sf::Color::Black);   // choix de la couleur du texte
		text.setCharacterSize(30); // choix de la taille des caractères exprimée en pixels, pas en points !			

		btnCommencer.setFillColor(sf::Color::White);
		btnCommencer.setOutlineThickness(5);
		btnCommencer.setOutlineColor(sf::Color::Red);
		btnCommencer.setPosition(350, 150);

		if (!texture.loadFromFile("image.jpg", sf::IntRect(0, 0, 200, 100)));
		btnCommencer.setTexture(&texture);
		texture.setSmooth(true);

		windowMenu.draw(btnCommencer);






		text.setString("Play :D"); 	// choix de la chaîne de caractères à afficher


		text.setPosition(450, 188);		// position du texte

		windowMenu.draw(text);


		btnOption.setFillColor(sf::Color::White);
		btnOption.setOutlineThickness(5);
		btnOption.setOutlineColor(sf::Color::Red);
		btnOption.setPosition(350, 300);

		texture.~Texture();
		if (!texture.loadFromFile("image.jpg", sf::IntRect(200, 500, 300, 100)));

		btnOption.setTexture(&texture);
		texture.setSmooth(true);

		windowMenu.draw(btnOption);

		text.setString("Options "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 333);		// position du texte

		windowMenu.draw(text);

		btnScore.setFillColor(sf::Color::White);
		btnScore.setOutlineThickness(5);
		btnScore.setOutlineColor(sf::Color::Red);
		btnScore.setPosition(350, 450);
	
		if (!texture.loadFromFile("image.jpg", sf::IntRect(300, 300, 300, 100)));

		btnScore.setTexture(&texture);
		texture.setSmooth(true);
		windowMenu.draw(btnScore);

		text.setString("Scores "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 488);		// position du texte

		windowMenu.draw(text);



		btnQuitter.setFillColor(sf::Color::White);
		btnQuitter.setOutlineThickness(5);
		btnQuitter.setOutlineColor(sf::Color::Red);
		btnQuitter.setPosition(350, 600);

		if (!texture.loadFromFile("image.jpg", sf::IntRect(100, 500, 300, 100)));

		btnQuitter.setTexture(&texture);
		texture.setSmooth(true);
		windowMenu.draw(btnQuitter);

		text.setString("Quitter "); 	// choix de la chaîne de caractères à afficher
		text.setPosition(450, 625);		// position du texte

		windowMenu.draw(text);


		windowMenu.display();


		// check the type of the event...


	} while (choixMenu(windowMenu, music, nomJoueur) != 1);

}
void saisie(sf::RenderWindow &window, bloc &actif, int occupations[][20], Music &music, int &i, string &nomJoueur)
{
	//Unknown  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 				
	//Num0 Num1 Num2 Num3 Num4 Num5 Num6 Num7 Num8 Num9 .0-9									
	//LControl RControl LShift RShift LAlt RAlt LSystem RSystem Menu 					
	//LBracket RBracket SemiColon Comma Period Quote Slash BackSlash Tilde 				
	//^ âˆž Ã— Ã· â‰¥ â‰¤ â‰  Â± LParentesis RParentesis â€¢ $ â‚¬ Â¥ Â£ & % # @ ! ? _ < > Ã© Ã  Ã§ Ã¯ Ã¹ Ã« (MAJ) Â© Â® â„¢	
	//Play Pause2 Previous Next Mute Sound- Sound+ MuteMic Refresh MousePadLock PlaneMode CameraLock
	//Lock Display Contrast- Contrast+ 
	//Equal Dash Space Return BackSpace Tab PageUp PageDown End Home Insert Delete				
	//Add Substract Multiply Divide Left Right Up Down											
	//Numpad0 Numpad1 Numpad2 Numpad3 Numpad4 Numpad5 Numpad6 Numpad7 Numpad8 Numpad9 	
	//F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12 F13 F14 F15 Pause KeyCount (toujours 102?)			
	//
	sf::Event event;

	while (window.pollEvent(event))
	{
		// check the type of the event...
		switch (event.type)
		{
			// window closed
		case sf::Event::Closed:
		{
			window.close();
			break;
		}
		// key pressed
		case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Unknown))
			{

			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				afficherMenu(music, nomJoueur);
			}
			else if (event.key.code == sf::Keyboard::Z)
			{
				actif.tourneGauche(occupations);
			}
			else if (event.key.code == sf::Keyboard::X || event.key.code == sf::Keyboard::Up)
			{
				actif.tourneDroite(occupations);
			}
			else if (event.key.code == sf::Keyboard::Left)
			{
				//actif.bougeGauche(occupations);
			}
			else if (event.key.code == sf::Keyboard::Right)
			{
				actif.bougeDroite(occupations);
			}
			else if (event.key.code == sf::Keyboard::Pause)
			{
				//Pause
				//Afficher menu
			}
			else if (event.key.code == sf::Keyboard::M)
			{
				//Met la musique sur stop
				music.setVolume(0);
			}
			else if (event.key.code == sf::Keyboard::Q)
			{
				//Met la musique sur stop
				music.setVolume(75);


			}
			else if (event.key.code == sf::Keyboard::T)
			{
				music.stop();
				//Change la musique 
				i++;
				switch (i)
				{
				case 1:
					if (music.openFromFile("music.ogg"))
						music.play();
					;
					break;

				case 2:
					if (music.openFromFile("music2.ogg"))
						music.play();
					;
					break;
				case 3:
					if (music.openFromFile("music3.ogg"))
						music.play();
					i = 0;
					break;

				default:
					break;
				}

			}
			// we don't process other types of events
		default:
			break;
		}
	}
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
	int occupations[30][20] = { '\0' };
	for (size_t i = 0; i < 20; i++)
	{
		occupations[30][i] = { 1 };
	}
	int blocL[4][5][5] = {
		//270Â° vers la gauche (couchÃ© sur la droite)
		{
			{ 0,0,0,0,0 },
			{ 0,0,0,0,0 },
			{ 0,1,1,1,0 },
			{ 0,1,0,0,0 },
			{ 0,0,0,0,0 },
		},
		//Angle par dÃ©faut (debout)
		{
			{ 0,0,0,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,1,1,0 },
			{ 0,0,0,0,0 },
		},
		//90Â° vers la gauche (couchÃ© sur sa gauche)
		{
			{ 0,0,0,0,0 },
			{ 0,0,0,1,0 },
			{ 0,1,1,1,0 },
			{ 0,0,0,0,0 },
			{ 0,0,0,0,0 },
		},
		//180Â° sur elle-mÃªme (sur Ã§a tÃªte)
		{
			{ 0,0,0,0,0 },
			{ 0,1,1,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,0,0,0 },
		}, },

		blocT[4][5][5] = {
		//270Â° vers la gauche (couchÃ© sur la droite)
		{
			{ 0,0,0,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,1,1,0 },
			{ 0,0,1,0,0 },
			{ 0,0,0,0,0 },
		},
		//Angle par dÃ©faut (debout)
		{
			{ 0,0,0,0,0 },
			{ 0,0,0,0,0 },
			{ 0,1,1,1,0 },
			{ 0,0,1,0,0 },
			{ 0,0,0,0,0 },
		},
		//90Â° vers la gauche (couchÃ© sur sa gauche)
		{
			{ 0,0,0,0,0 },
			{ 0,0,1,0,0 },
			{ 0,1,1,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,0,0,0 },
		},
		//180Â° sur elle-mÃªme (sur Ã§a tÃªte)
		{
			{ 0,0,0,0,0 },
			{ 0,0,1,0,0 },
			{ 0,1,1,1,0 },
			{ 0,0,0,0,0 },
			{ 0,0,0,0,0 },
		}, },

		blocLigne[4][5][5] = {
		//270Â° vers la gauche (couchÃ© sur la droite)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,1,1,1,1 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
			},
			//Angle par dÃ©faut (debout)
			{
				{ 0,0,0,0,0 },
				{ 0,0,1,0,0 },
				{ 0,0,1,0,0 },
				{ 0,0,1,0,0 },
				{ 0,0,1,0,0 },
			},
			//90Â° vers la gauche (couchÃ© sur sa gauche)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 1,1,1,1,0 },
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
			},
			//180Â° sur elle-mÃªme (sur Ã§a tÃªte)
			{
				{ 0,0,1,0,0 },
				{ 0,0,1,0,0 },
				{ 0,0,1,0,0 },
				{ 0,0,1,0,0 },
				{ 0,0,0,0,0 },
			}, },

			blocLInverser[4][5][5] = {
		//270Â° vers la gauche (couchÃ© sur la droite)
		{
			{ 0,0,0,0,0 },
			{ 0,0,0,0,0 },
			{ 0,1,1,1,0 },
			{ 0,0,0,1,0 },
			{ 0,0,0,0,0 },
		},
		//Angle par dÃ©faut (debout)
		{
			{ 0,0,0,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,1,0,0 },
			{ 0,1,1,0,0 },
			{ 0,0,0,0,0 },
		},
		//90Â° vers la gauche (couchÃ© sur sa gauche)
		{
			{ 0,0,0,0,0 },
			{ 0,0,0,0,0 },
			{ 0,1,1,1,0 },
			{ 0,0,0,1,0 },
			{ 0,0,0,0,0 },
		},
		//180Â° sur elle-mÃªme (sur Ã§a tÃªte)
		{
			{ 0,0,0,0,0 },
			{ 0,0,1,1,0 },
			{ 0,0,1,0,0 },
			{ 0,0,1,0,0 },
			{ 0,0,0,0,0 },
		}, },

		blocCarrer[4][5][5] = {
		//270Â° vers la gauche (couchÃ© sur la droite)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,1,1,0 },
				{ 0,0,1,1,0 },
				{ 0,0,0,0,0 },
			},
			//Angle par dÃ©faut (debout)
			{
				{ 0, 0, 0, 0, 0 },
				{ 0,0,0,0,0 },
				{ 0,0,1,1,0 },
				{ 0,0,1,1,0 },
				{ 0,0,0,0,0 },
			},
			//90Â° vers la gauche (couchÃ© sur sa gauche)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,1,1,0 },
				{ 0,0,1,1,0 },
				{ 0,0,0,0,0 },
			},
			//180Â° sur elle-mÃªme (sur Ã§a tÃªte)
			{
				{ 0,0,0,0,0 },
				{ 0,0,0,0,0 },
				{ 0,0,1,1,0 },
				{ 0,0,1,1,0 },
				{ 0,0,0,0,0 },
			}, },

			blocZ[4][5][5] = {
		//270Â° vers la gauche (couchÃ© sur la droite)
	{
		{ 0,0,0,0,0 },
		{ 0,0,1,0,0 },
		{ 0,1,1,0,0 },
		{ 0,1,0,0,0 },
		{ 0,0,0,0,0 },

	},
	//Angle par dÃ©faut (debout)
	{


		{ 0,0,0,0,0 },
		{ 0,0,0,0,0 },
		{ 0,1,1,0,0 },
		{ 0,0,1,1,0 },
		{ 0,0,0,0,0 },
	},
	//90Â° vers la gauche (couchÃ© sur sa gauche)
	{
		{ 0,0,0,0,0 },
		{ 0,0,1,0,0 },
		{ 0,1,1,0,0 },
		{ 0,1,0,0,0 },
		{ 0,0,0,0,0 },

	},
	//180Â° sur elle-mÃªme (sur Ã§a tÃªte)
	{


		{ 0,0,0,0,0 },
		{ 0,0,0,0,0 },
		{ 0,1,1,0,0 },
		{ 0,0,1,1,0 },
		{ 0,0,0,0,0 },
	}, },

	blocZInverser[4][5][5] = {
		//270Â° vers la gauche (couchÃ© sur la droite)
				{
					{ 0,0,0,0,0 },
					{ 0,0,1,0,0 },
					{ 0,0,1,1,0 },
					{ 0,0,0,1,0 },
					{ 0,0,0,0,0 },

				},
				//Angle par dÃ©faut (debout)
				{


					{ 0,0,0,0,0 },
					{ 0,0,0,0,0 },
					{ 0,0,1,1,0 },
					{ 0,1,1,0,0 },
					{ 0,0,0,0,0 },
				},
				//90Â° vers la gauche (couchÃ© sur sa gauche)
				{
					{ 0,0,0,0,0 },
					{ 0,0,1,0,0 },
					{ 0,0,1,1,0 },
					{ 0,0,0,1,0 },
					{ 0,0,0,0,0 },

				},
				//180Â° sur elle-mÃªme (sur Ã§a tÃªte)
				{

					{ 0,0,0,0,0 },
					{ 0,0,0,0,0 },
					{ 0,0,1,1,0 },
					{ 0,1,1,0,0 },
					{ 0,0,0,0,0 },
				}, };

	salle espace;

	bloc l(2, 2, 30, 30, 1, 1, 1, 1, 1, 1, blocL);
	bloc t(2, 2, 30, 30, 2, 1, 1, 1, 1, 1, blocT);
	bloc Carrer(2, 2, 30, 30, 3, 1, 1, 1, 1, 1, blocCarrer);
	bloc ligne(2, 2, 30, 30, 3, 1, 1, 1, 1, 1, blocLigne);
	bloc lInverser(2, 2, 30, 30, 4, 1, 1, 1, 1, 1, blocLInverser);
	bloc Z(2, 2, 30, 30, 5, 1, 1, 1, 1, 1, blocZ);
	bloc ZInverser(2, 2, 30, 30, 6, 1, 1, 1, 1, 1, blocZInverser);
	bloc pieces[7] = { l,l,l,l,l,l,l };
	bloc blocActif = { 2, 2, 30, 30, 1, 1, 1, 1, 1, 1, blocL };
	bloc blocSuivant = { 2, 2, 30, 30, 1, 1, 1, 1, 1, 1, blocL };
	string nomJoueur = "Nouveau Joueur";
	setlocale(LC_CTYPE, "fra");

	sf::RenderWindow window(sf::VideoMode(1000, 800), "TETRIS v1.2");


	//Tests
	teStruct test;

	//Essais de blocs
	sf::Music music;
	music.stop();
	music.openFromFile("music3.ogg");


	afficherMenu( music, nomJoueur);

	if (music.getStatus() == false)
		music.play();

	int i = 0;

	sf::RectangleShape formeL[5][5];
	sf::RectangleShape formeT[5][5];
	sf::RectangleShape formeLInverser[5][5];
	sf::RectangleShape formeZ[5][5];
	sf::RectangleShape formeZInverser[5][5];
	sf::RectangleShape formeLigne[5][5];
	sf::RectangleShape formeCarrer[5][5];
	sf::RectangleShape formeActif[5][5];
	sf::RectangleShape piecePlacer;
	sf::RectangleShape pieceSuivante[5][5];
	int profil[5][5];

	int prochainePiece;
	int piece ;
	int pieceActive;

	coord posL2 = l.getPlace();
	coord posT2 = t.getPlace();
	coord posLInverser2 = lInverser.getPlace();
	coord posZ2 = Z.getPlace();
	coord posZinverser2 = ZInverser.getPlace();
	coord posLigne2 = ligne.getPlace();
	coord posCarrer2 = Carrer.getPlace();
	coord posActif2 = blocActif.getPlace();
	coord posSuivant2 = blocSuivant.getPlace();

	//bloque de -2 ou -1 Ã  2

	espace.getOccupation(occupations);


	while (window.isOpen()) {
		
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		sf::Texture texture;
		if (!texture.loadFromFile("Tetris-Background.jpg"));
		sf::Sprite background(texture);
		window.draw(background);
		piece = espace.prochain();
		
		pieceActive = piece;

		switch (piece)
		{
		case 1:
			l.getProfil(profil);
			posL2.x -= 3;
			espace.setOccupation(profil, posL2);
			formePiece(formeL, profil, l.getPlace());

			blocActif.setId(1);
			blocActif.setAxes(blocL);

			piece = 8;
			break;

		case 2:
			t.getProfil(profil);


			posT2.x -= 3;
			espace.setOccupation(profil, posT2);
			formePiece(formeT, profil, t.getPlace());
			blocActif.setId(2);
			blocActif.setAxes(blocT);

			piece = 8;
			break;
		case 3:
			Carrer.getProfil(profil);
			posCarrer2.x -= 3;
			espace.setOccupation(profil, posCarrer2);
			formePiece(formeCarrer, profil, Carrer.getPlace());
			blocActif.setId(3);
			blocActif.setAxes(blocCarrer);

			piece = 8;
			break;
		case 4:
			ligne.getProfil(profil);


			posLigne2.x -= 3;
			espace.setOccupation(profil, posLigne2);
			formePiece(formeLigne, profil, ligne.getPlace());
			blocActif.setId(4);
			blocActif.setAxes(blocLigne);

			piece = 8;
			break;
		case 5:
			lInverser.getProfil(profil);


			posLInverser2.x -= 3;
			espace.setOccupation(profil, posLInverser2);
			formePiece(formeLInverser, profil, lInverser.getPlace());
			blocActif.setId(5);
			blocActif.setAxes(blocLInverser);

			piece = 8;
			break;
		case 6:
			Z.getProfil(profil);

			posZ2.x -= 3;
			espace.setOccupation(profil, posZ2);
			formePiece(formeZ, profil, Z.getPlace());
			blocActif.setId(6);
			blocActif.setAxes(blocZ);

			piece = 8;
			break;
		case 7:
			ZInverser.getProfil(profil);

			posZinverser2.x -= 3;
			espace.setOccupation(profil, posZinverser2);
			formePiece(formeZInverser, profil, ZInverser.getPlace());
			blocActif.setId(7);
			blocActif.setAxes(blocZInverser);

			piece = 8;
			break;
		default:
			break;
		}
		/*t.getProfil(profil);*/
		espace.afficherInterface(window);
		blocActif.getProfil(profil);
		posActif2.x = -3;
		espace.setOccupation(profil, posActif2);
		formePiece(formeActif, profil, blocActif.getPlace());
		
		window.display();
		do
		{
			prochainePiece = espace.prochain();
			while (prochainePiece == piece)
			{
				prochainePiece = espace.prochain();
			}
			switch (prochainePiece)
			{
			case 1:
				l.getProfil(profil);
				blocSuivant.setId(1);
				blocSuivant.setAxes(blocL);
				espace.modifierInterface(window, formeL, profil, nomJoueur);
				piece = 8;
				break;
			case 2:
				t.getProfil(profil);
				blocSuivant.setId(2);
				blocSuivant.setAxes(blocT);
				espace.modifierInterface(window, formeT, profil, nomJoueur);
				piece = 8;
				break;
			case 3:
				Carrer.getProfil(profil);
				blocSuivant.setId(3);
				blocSuivant.setAxes(blocCarrer);
				espace.modifierInterface(window, formeCarrer, profil, nomJoueur);
				piece = 8;
				break;
			case 4:
				ligne.getProfil(profil);
				blocSuivant.setId(4);
				blocSuivant.setAxes(blocLigne);
				espace.modifierInterface(window, formeLigne, profil, nomJoueur);
				piece = 8;
				break;
			case 5:
				lInverser.getProfil(profil);
				blocSuivant.setId(5);
				blocSuivant.setAxes(blocLInverser);
				espace.modifierInterface(window, formeLInverser, profil, nomJoueur);
				piece = 8;
				break;
			case 6:
				Z.getProfil(profil);
				blocSuivant.setId(6);
				blocSuivant.setAxes(blocZ);
				espace.modifierInterface(window, formeZ, profil, nomJoueur);
				piece = 8;
				break;
			case 7:
				ZInverser.getProfil(profil);
				blocSuivant.setId(7);
				blocSuivant.setAxes(blocZInverser);
				espace.modifierInterface(window, formeZInverser, profil, nomJoueur);
				piece = 8;
				break;
			default:
				break;
			}
			blocSuivant.getProfil(profil);
			posSuivant2.x = -3;
			espace.setOccupation(profil, posSuivant2);
			formePiece(pieceSuivante, profil, blocSuivant.getPlace());
			

			
			

			//testPackPlay(test, window);

					//l.tourneGauche(occupations);
					//l.getProfil(profil);

			int posy, posx;
			int nbY = 0;
			coord debut;
			debut.x = 205;
			debut.y = 0;
			posx = blocActif.getX();
			posy = blocActif.getY();
			blocActif.setPosX(debut);
			blocActif.setPosY(debut);
			espace.afficherInterface(window);
			espace.modifierInterface(window, pieceSuivante, profil, nomJoueur);
		
			do {
				posx = blocActif.getX();
				posy = blocActif.getY();
				
				saisie(window, l, occupations, music, i, nomJoueur);
				if (!texture.loadFromFile("Tetris-Background.jpg"));
				sf::Sprite background(texture);
				window.draw(background);
				

				blocActif.getProfil(profil);
				formePiece(formeActif, profil, blocActif.getPlace());
				espace.afficherInterface(window);
				espace.modifierInterface(window, pieceSuivante, profil, nomJoueur);
				

				drawPiece(window, formeActif, profil);
				formePiece(formeActif, profil, blocActif.getPlace());

				
			
			
				blocActif.bougeY(posy, posx, occupations);
			
				
			
				nbY++;
				window.display();
				sf::sleep(sf::seconds(0.2));
			
				
				
				
				
				

			} while (nbY < 18);
			
			/*sf::RectangleShape piecePlacer;
			piecePlacer.setSize(sf::Vector2f(600, 763));
			piecePlacer.setFillColor(sf::Color::Transparent);
			piecePlacer.setOutlineThickness(0);
			piecePlacer.setOutlineColor(sf::Color::Red);
			piecePlacer.setPosition(20,20);


			window.draw(piecePlacer);*/
			/*window.display();*/
			blocActif.setId(blocSuivant.getId());
			blocActif.getProfil(profil);
			posActif2.x = -3;;
			piece = blocSuivant.getId();

			blocActif.setPosX(debut);
			blocActif.setPosY(debut);
			formePiece(pieceSuivante, profil, blocActif.getPlace());
			window.display();
		} while (true);
	}

	return 0;
}

/*MÃ©thodes
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


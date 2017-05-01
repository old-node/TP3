/* En-t�te du fichier */
/* ================== */
/*Fichier:		carre.h
Auteur:			Olivier Lemay Dostie
Date cr�ation:	30/04/2017
Description:	header pour le TP3 de C++ pour la cr�ation de carr�s SFML. */

/* Directives au pr�-processeur */
/* ============================ */
#pragma once								
#include <stdlib.h>			//Pk d�j�?
#include <assert.h>			
#include <SFML\Graphics.hpp>
#include <carre.h>
using namespace std;
using namespace sf;

/* Prototypes des fonctions */

class bloc;
bool initTetris(bloc tetris[7]);


/* Constantes des blocs */
/* ==================== */

// Les coordonn�es de chaque carr�s de chaque angles des 7 pi�ces par d�faut du jeu
const Vector2i PIECES[7][4][4] =
{		// 7 formes, 4 angles, 4 carr�s, pour chaque coordonn�es {{{{Vector2i}*4}*4}*7}
	{	// Les angles sont: droite (0), debout (1), gauche (2), renverse (3)
		{ Vector2i{ 1,2 }, Vector2i{ 2,2 }, Vector2i{ 3,2 }, Vector2i{ 3,1 } },
		{ Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 1,1 } },
		{ Vector2i{ 1,2 }, Vector2i{ 2,2 }, Vector2i{ 3,2 }, Vector2i{ 1,3 } },
		{ Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,3 } }
	},	// Sept (L)
	{
		{ Vector2i{ 1,2 }, Vector2i{ 2,2 }, Vector2i{ 3,2 }, Vector2i{ 3,3 } },
		{ Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,1 } },
		{ Vector2i{ 1,2 }, Vector2i{ 2,2 }, Vector2i{ 3,2 }, Vector2i{ 1,1 } },
		{ Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 1,3 } }
	},	// Pendu (Mirroir du Sept)
	{
		{ Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 }, Vector2i{ 3,3 } },
		{ Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 }, Vector2i{ 3,3 } },
		{ Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 }, Vector2i{ 3,3 } },
		{ Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 }, Vector2i{ 3,3 } }
	},	// Carr�
	{
		{ Vector2i{ 1,2 }, Vector2i{ 2,2 }, Vector2i{ 3,2 }, Vector2i{ 4,2 } },
		{ Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 2,0 } },
		{ Vector2i{ 1,2 }, Vector2i{ 2,2 }, Vector2i{ 3,2 }, Vector2i{ 0,2 } },
		{ Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 2,4 } }
	},	// Ligne
	{
		{ Vector2i{ 1,2 }, Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 2,3 } },
		{ Vector2i{ 1,2 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 } },
		{ Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 } },
		{ Vector2i{ 1,2 }, Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 3,2 } }
	},	// Plateau
	{
		{ Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 }, Vector2i{ 1,3 } },
		{ Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 3,2 }, Vector2i{ 3,3 } },
		{ Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 }, Vector2i{ 1,3 } },
		{ Vector2i{ 2,1 }, Vector2i{ 2,2 }, Vector2i{ 3,2 }, Vector2i{ 3,3 } }
	},	//	Croche (S)
	{
		{ Vector2i{ 1,2 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,3 } },
		{ Vector2i{ 1,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 }, Vector2i{ 3,1 } },
		{ Vector2i{ 1,2 }, Vector2i{ 2,2 }, Vector2i{ 2,3 }, Vector2i{ 3,3 } },
		{ Vector2i{ 1,2 }, Vector2i{ 2,3 }, Vector2i{ 3,2 }, Vector2i{ 3,1 } } } };//	Pli� (Z)


/* Objets � manipuler compos� de carr�s */
/* ==================================== */
class bloc
{
private:
	Vector2f _encrage = MILLIEUBLOC;	// Point pivot du bloc
	Vector2i _place = Vector2i(1, 9);	// Endroit situ� dans la salle
	int _id = -2;						// No. associ� � son utilisation, 
		// -2 pour fixe, -1 pour prochain, 0 pour actif et les autres sont incr�ment�s au jeu
	int _styleBloc = 0;					// (Si n�cessaire) Pour qu'on sache facillement quels attributs ont ses carr�s (couleur, dimenssions, etc.)
	//int _vitesse = 0;					// Vitesse de d�placement vertical (gravit�)
	int _etat = 0;						// (Si n�cessaire) 1 pour normal (fixe), 2 pour indestructible, 3 pour fragile, 4 pour mobile
	int _morceau = -1;					// Pi�ce du lot des blocs accessible du jeu (de 0 � 6)
	int _angle = 0;						// Orientation actuelle de la forme (quatre angles)
	vector<carre> _formes[4]{			// Les quatres profils en carr�s de la forme
		{ vector<carre>(4) },{ vector<carre>(4) },
		{ vector<carre>(4) },{ vector<carre>(4) } };
	vector<Vector2i> _axes[4]{			// Coordonn�es des carr�s des profils
		{ vector<Vector2i>(4) },{ vector<Vector2i>(4) },
		{ vector<Vector2i>(4) },{ vector<Vector2i>(4) } };

public:
	// Instanciations
	bloc();
	bloc(const int & forme, const vector<carre> tours[4]);
	bloc(const Vector2i & place, const int & id, const int & styleBloc, /*const int & vitesse,*/
		const int & etat, const int & forme, const int & angle, const vector<carre> tours[4]);
	// Destructeur
	~bloc();

	// Initialisations
	void initBloc(const Vector2i & place, const int & id, const int & styleBloc, const int & etat, const int & forme,
		const int & angle, const vector<carre> tours[4]/*, const int & vitesse*/);
	void initForme(const int & forme, const vector<carre> tours[4]);

	// Coordonn�es
	void setPlace(const Vector2i & place);
	void setEncrage(const Vector2f & pos);
	void deplace(const int & x, const int & y);

	// Attributs du bloc
	void setId(const int & id);
	void setStyleBloc(const int & styleBloc);
	void setPiece(const int & piece);
	//void setVitesse(const int & vitesse);
	void setEtat(const int & etat);

	// Profils du bloc
	void setAngle(const int & angle);
	void setFormes(const vector<carre> tours[4]);

	// Modification du profil
	void ajouteCube(const Vector2f & coin, const Vector2i axe[4], carre rect[4]);
	void remplaceCube(carre rect, const Vector2f & pos, const Vector2i axes[4], const int & position);
	void enleveCube(const int & angle);

	//// Transforme les attributs du bloc

	//void ralenti();
	//void detruit();
	//void separe();
	//void fusionne();
	//void efface();

	// Retourne les attributs du bloc
	Vector2i getPlace();
	Vector2f getEncrage();
	int getId();
	int getStyleBloc();
	int getPiece();
	//int getVitesse();
	int getEtat();
	int getAngle();
	void getProfil(vector<carre> & tours);
	void getAxes(vector<Vector2i> & axes);

	// Rotate
	void tourneGauche()
	{
		for (auto & element : _formes[_angle])
		{
			element.rotate(-90);
		}
	}
	void tourneDroite()
	{
		for (auto & element : _formes[_angle])
			element.rotate(90);
	}

	// Dessine le bloc
	void draw(RenderWindow & window);
};



/*M�thodes des blocs*/
/*==================*/

// Instancie un bloc par d�faut
bloc::bloc() {}

// Instancie un bloc selon une forme sp�cifique
bloc::bloc(const int & forme, const vector<carre> tours[4])
{
	initForme(forme, tours);
}

// Instancie un bloc avec toutes les attributs qu'il peut contenir
bloc::bloc(const Vector2i & place, const int & id, const int & styleBloc, /*const int & vitesse,*/
	const int & etat, const int & forme, const int & angle, const vector<carre> tours[4])
{
	initBloc(place, id, styleBloc,/* vitesse,*/ etat, forme, angle, tours);
}

// Destructeur des blocs
bloc::~bloc()
{
	_place.x = _place.y = _encrage.x = _encrage.y = _id =
		_styleBloc = _morceau = /*_vitesse = */_etat = _angle = 0;
	for (int i = 0; i < 4; i++)
		_formes[i].~vector();
}

// Donne des valeurs de base au bloc.
void bloc::initBloc(const Vector2i & place, const int & id, const int & styleBloc, const int & etat, const int & piece,
	const int & angle, const vector<carre> tours[4]/*, const int & vitesse*/)
{
	setPlace(place);
	setId(id);
	setStyleBloc(styleBloc);
	//setVitesse(vitesse);
	setEtat(etat);
	setPiece(piece);
	setAngle(angle);
	setFormes(tours);
}

// Change le profil et le num�ro de pi�ce d'un bloc.
void bloc::initForme(const int & piece, const vector<carre> tours[4])
{
	setPiece(piece);
	setFormes(tours);
}

// Change les coordonn�es du bloc dans la pi�ce.
void bloc::setPlace(const Vector2i & place)
{
	assert(place.x >= 0 && place.x <= 19 &&
		place.y >= 0 && place.y <= 19);
	_place = place;
}

//// Modifie la position d'un carr� du bloc � une autre coordonn�e.
//void bloc::setPosX(const Vector2f & coin, const int & x)
//{
//	for (auto & element : _formes[_angle])
//		element.setPos(coin, _place, element.getPos());
//}
//
//// 
//void bloc::setPosY(const int & y)
//{
//	_place.y += y;
//	for (auto & element : _formes[_angle])
//		element.setPos(element._i, element._j, _place);
//}

// Change le point de pivot pour toutes les modifications
void bloc::setEncrage(const Vector2f & encrage)
{
	_encrage = encrage;
	//Changer aussi le point _origine de tout les carr�s du bloc... sinon �a fait rien.
}

// Change l'ID du bloc.
void bloc::setId(const int & id)
{
	_id = id;
}

// Change le num�ro du style du bloc.
void bloc::setStyleBloc(const int & styleBloc)
{
	_styleBloc = styleBloc;
}

// Change le num�ro de pi�ce du bloc.
void bloc::setPiece(const int & piece)
{
	_morceau = piece;
}

// 
//void bloc::setVitesse(int vitesse)
//{
//	_vitesse = vitesse;
//}

// Change l'�tat du bloc.
void bloc::setEtat(const int & etat)
{
	_etat = etat;
}

// Change l'angle du bloc.
void bloc::setAngle(const int & angle)
{
	assert(angle >= 0 && angle <= 3);
	_angle = angle;
}

// Change les carr�s du bloc.
void bloc::setFormes(const vector<carre> tours[4])
{
	//Ajoute des carr�s au profils
	for (int i = 0; i < 4; i++)
	{
		int size = tours[i].size();

		_formes[i].resize(0);
		_axes[i].resize(0);

		for (int j = 0; j < size; j++)
		{
			_formes[i].push_back(tours[i].at(j));
			_axes[i].push_back(_formes[i].at(j).getPos());
		}
	}
}

// Modifie l'emplacement du bloc dans la salle.
void bloc::deplace(const int & x, const int & y)
{
	setPlace(Vector2i(_place.x + x, _place.y + y));
	for (auto & element : _formes[_angle])
		element.deplace(Vector2i(x, y));
}

// _En construction_, Trouve automatiquement les axes d'un carr� pour chaque angle.
bool trouveAxesRotation(Vector2i axes[4], const Vector2i & a, const int & angle)
{
	assert(angle >= 0 && angle <= 3);

	int gabarit[5][5] = { {1,2,3,4,1}, {4,5,6,5,2}, {3,6,0,6,3}, {2,5,6,5,4}, {1,4,3,2,1} };
	Vector2i autres;

	if (a == Vector2i(2, 2))
		return 0;
	else if (gabarit[a.x][a.y] == 1)
		;//axes = { {0,0},{},{},{} };
	for (int i = 0; i < 4; i++)
	{
		//rotate()
		axes[i] = autres;
	}

	return 1;
}

// Ajoute un cube dans la liste.
void bloc::ajouteCube(const Vector2f & coin, const Vector2i axe[4], carre rect[4])
{
	for (int i = 0; i < 4; i++)
	{
		rect[i].setPos(coin, _place, axe[i]);
		_formes[i].push_back(rect[i]);
		_axes[i].push_back(axe[i]);
	}
}

// 
void bloc::remplaceCube(carre rect, const Vector2f & pos, const Vector2i axes[4], const int & position)
{
	assert(position < _formes[_angle].size() && position >= 0);
	
	for (int i = 0; i < 4; i++)
	{
		rect.setPos(pos, _place, axes[i]);
		_formes[i].at(position) = rect;
		//_formes[angle].assign(position, rect);
	}
}

// 
void bloc::enleveCube(const int & angle)
{
	//Est-ce vraiment le dernier qu'on veut enlever?
	_formes[angle].pop_back();
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
int bloc::getPiece()
{
	return _morceau;
}

// 
//int bloc::getVitesse()
//{
//	return _vitesse;
//}

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
void bloc::getProfil(vector<carre> & tours)
{
	tours.resize(0);
	for (auto const &element : _formes[_angle])
		tours.push_back(element);
}

// 
void bloc::getAxes(vector<Vector2i> & axes)
{
	axes.resize(0);
	for (auto &element : _formes[_angle])
		axes.push_back(element.getPos());
}

// 
void bloc::draw(RenderWindow & window)
{
	for (auto & element : _formes[_angle])
		element.draw(window);
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


/* Fonctions aux blocs */
/* =================== */

// Construit les pi�ces par d�faut � partir des coordonn�es du tableau PIECES.
bool initTetris(bloc tetris[7])
{
	int x = PLACE.x,
		y = PLACE.y;
	vector<carre> formes[4];	// Carr�s des blocs

	for (int f = 0; f < 7; f++)
	{
		for (int a = 0; a < 4; a++)
			for (int c = 0; c < 4; c++)
				formes[a].push_back(carre(COIN, PLACE, PIECES[f][a][c]));

		tetris[f].initBloc(PLACE, -3, 1, -1, f, 1, formes);
		for (int i = 0; i < 4; i++)
			formes[i].resize(0);
	}
	return 0;
}


/* Constantes en bloc */
/* ================== */
bloc tetris[7];					// Liste de sept blocs � instancier � partir de PIECES
bool ok = initTetris(tetris);	/// Instancie les bloc
								// Les septs blocs par d�fauts du jeu: Sept(L), Pendu (Mirroir du Sept), Carr�, Ligne, Plateau (T), Croche (S), Pli� (Z)
const bloc TETRIS[7] = { tetris[0], tetris[1], tetris[2], tetris[3], tetris[4], tetris[5], tetris[6] };
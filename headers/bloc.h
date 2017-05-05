/* En-t�te du fichier */
///================== */
/* Fichier:			carre.h
// Auteur:			Olivier Lemay Dostie
// Date cr�ation:	30/04/2017
// Description:		header pour le TP3 de C++ pour la cr�ation de blocs manipulables. */


/* Directives au pr�-processeur (voir suite) */
///========================================= */
#pragma once								
#include <assert.h>				
#include <vector>				
using namespace std;
#include <SFML/Graphics.hpp>	
#include <carre.h>			// Doit �tre plac� apr�s certaines constantes (on peut mettre les constantes dans 
using namespace sf;			// carre.h, mais selon moi ils n'ont pas raison d'�tre incluent dedant vu leur nature)


/* Prototypes des fonctions */
///======================== */
class bloc;


/* Objets � manipuler compos� de carr�s */
///==================================== */
class bloc
{
private:
	Vector2f _encrage = PIVOTBLOC;	// Point pivot du bloc
	Vector2i _place = PLACE;		// Endroit situ� dans la salle
	int _id = -2;					// No. associ� � son utilisation 
		// -2 pour fixe, -1 pour prochain, 0 pour actif et les autres sont incr�ment�s au jeu
	int _styleBloc = 0;				// Pour qu'on sache facillement quels attributs ont ses carr�s 
		// (Si n�cessaire) couleur, dimenssions, etc.
	///int _vitesse = 0;			// Vitesse de d�placement vertical (gravit�) :: G�r� plust�t par la salle
	int _etat = 0;					// Indice de l'�tat de l'utilisation d bloc
		// (Si n�cessaire) 1 pour normal (fixe), 2 pour indestructible, 3 pour fragile, 4 pour mobile
	int _morceau = -1;				// Pi�ce du lot des blocs accessible du jeu (de 0 � 6)
	int _angle = 0;					// Orientation actuelle de la forme (quatre angles)
	vector<carre> _formes;/*{*/		// Les quatres profils en carr�s de la forme
		//{ vector<carre>(4) },{ vector<carre>(4) },
		//{ vector<carre>(4) },{ vector<carre>(4) } };
	vector<Vector2i> _axes[4]		// Coordonn�es des carr�s des profils sous les quatres angles
	{ { vector<Vector2i>(4) },			// Couch� sur la droite
	  { vector<Vector2i>(4) },			// Debout
	  { vector<Vector2i>(4) },			// Couch� sur la gauche
	  { vector<Vector2i>(4) }			// � l'envers
	};

public:
	// Instanciatieurs
	bloc();
	bloc(const int & forme, const vector<carre> tours, const vector<Vector2i> axes[4]);
	bloc(const Vector2i & place, const int & id, const int & styleBloc, const int & etat, const int & forme,
		const int & angle, const vector<carre> tours, const vector<Vector2i> axes[4]);
	bloc(const bloc & copie)
	{
		setEncrage(copie._encrage);
		setPlace(copie._place);
		setId(copie._id);
		setStyleBloc(copie._styleBloc);
		setEtat(copie._etat);
		setPiece(copie._morceau);
		setAngle(copie._angle);
		setFormes(copie._formes, copie._axes);
	}
	bloc& operator=(bloc rhs)
	{
		rhs.echange(*this);
		/// http://stackoverflow.com/questions/255612/dynamically-allocating-an-array-of-objects
		return *this;
	}
	void echange(bloc& s) noexcept
	{
		//swap(this._encrage, s._encrage);
		/// Marche pas... ?
	}

	// Destructeur
	~bloc();

	// Initialisateurs
	void initBloc(const Vector2i & place, const int & id, const int & styleBloc, const int & etat, const int & forme,
		const int & angle, const vector<carre> tours, const vector<Vector2i> axes[4]/*, const int & vitesse*/);
	void initForme(const int & forme, const vector<carre> tours, const vector<Vector2i> axes[4]);

	// Modificateur de coordon�es
	void setPlace(const Vector2i & place);
	void setEncrage(const Vector2f & pos);
	void deplace(const int & x, const int & y);

	// Attributs du bloc
	void setId(const int & id);
	void setStyleBloc(const int & styleBloc);
	void setPiece(const int & piece);
	void setEtat(const int & etat);

	// Profils du bloc
	void setAngle(const int & angle);
	void setFormes(const vector<carre> & tours, const vector<Vector2i> axes[4]);

	// Modification du profil
	void ajouteCube(carre rect, const Vector2f & coin, const Vector2i axe[4]);
	void remplaceCube(carre rect, const Vector2f & pos, const Vector2i axes[4], const int & position);
	void enleveCube(const int & angle);

	/// Transforme les attributs du bloc
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
	int getEtat();
	int getAngle();
	void getAxes(vector<Vector2i> & axes, const int & angle);
	void getProfil(vector<carre> & profil);

	// Rotate
	void tourneGauche()
	{
		for (auto & element : _formes)
			element.rotate(-90);
	}
	void tourneDroite()
	{
		for (auto & element : _formes)
			element.rotate(90);
	}

	// Dessine le bloc
	void draw(RenderWindow & window);
	void montre(RenderWindow & window, const Vector2f & coin);
};


/* M�thodes des blocs */
///================== */

// Instancie un bloc par d�faut.
bloc::bloc() {}

// Instancie un bloc selon une forme sp�cifique.
bloc::bloc(const int & forme, const vector<carre> tours, const vector<Vector2i> axes[4])
{
	initForme(forme, tours, axes);
}

// Instancie un bloc avec toutes les attributs qu'il peut contenir.
bloc::bloc(const Vector2i & place, const int & id, const int & styleBloc, const int & etat,
	const int & forme, const int & angle, const vector<carre> tours, const vector<Vector2i> axes[4])
{
	initBloc(place, id, styleBloc, etat, forme, angle, tours, axes);
}

// Destructeur des blocs.
bloc::~bloc()
{
	_place.x = _place.y = _encrage.x = _encrage.y = _id =
		_styleBloc = _morceau = _etat = _angle = 0;
	for (int i = 0; i < 4; i++)
		_formes.~vector();
}

// Donne des valeurs de base au bloc.
void bloc::initBloc(const Vector2i & place, const int & id, const int & styleBloc, const int & etat, const int & piece,
	const int & angle, const vector<carre> tours, const vector<Vector2i> axes[4]/*, const int & vitesse*/)
{
	setPlace(place);
	setId(id);
	setStyleBloc(styleBloc);
	setEtat(etat);
	setPiece(piece);
	setAngle(angle);
	setFormes(tours, axes);
}

// Change le profil et le num�ro de pi�ce d'un bloc.
void bloc::initForme(const int & piece, const vector<carre> tours, const vector<Vector2i> axes[4])
{
	setPiece(piece);
	setFormes(tours, axes);
}

// Change les coordonn�es du bloc dans la pi�ce.
void bloc::setPlace(const Vector2i & place)
{
	assert(place.x > -1 && place.x <= LRGJEU - 1 &&
		place.y >= 0 && place.y < HAUJEU - 1);
	_place = place;
}

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

// Change les carr�s du bloc. (n�cessaire? on peut utiliser initPieces p-e)
void bloc::setFormes(const vector<carre> & tours, const vector<Vector2i> axes[4])
{
	int size = tours.size();

	//Ajoute des carr�s au profils
	_formes.resize(0);
	for (int j = 0; j < size; j++)
	{
		_formes.push_back(tours.at(j));
		_formes.at(j).setPos(POS, _place, axes[0].at(j));
	}

	//Ajoute les axes des carr�s au bloc
	for (int i = 0; i < 4; i++)
	{
		_axes[i].resize(0);
		for (int j = 0; j < size; j++)
			_axes[i].push_back(axes[i].at(j));
	}
}

// Modifie l'emplacement du bloc dans la salle.
void bloc::deplace(const int & x, const int & y)
{
	setPlace(Vector2i(_place.x + x, _place.y + y));
	for (carre & element : _formes)
		element.deplace(Vector2i(x, y));
}

/// _En construction_, Trouve automatiquement les axes d'un carr� pour chaque angle.
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
void bloc::ajouteCube(carre rect, const Vector2f & coin, const Vector2i axe[4])
{
	rect.setPos(coin, _place, axe[0]);
	_formes.push_back(rect);
	for (int i = 0; i < 4; i++)
		_axes[i].push_back(axe[i]);
}

// 
void bloc::remplaceCube(carre rect, const Vector2f & pos, const Vector2i axes[4], const int & position)
{
	//assert(position < _formes[_angle].size() && position >= 0);

	//for (int i = 0; i < 4; i++)
	//{
	//	rect.setPos(pos, _place, axes[i]);
	//	_formes[i].at(position) = rect;
	//	//_formes[angle].assign(position, rect);
	//}
}

// 
void bloc::enleveCube(const int & angle)
{
	//Est-ce vraiment le dernier qu'on veut enlever?
	//_formes[angle].pop_back();
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
int bloc::getEtat()
{
	return _etat;
}

// 
int bloc::getAngle()
{
	return _angle;
}

// Retourne les coordonn�es des carr�s d'un angle de la forme.
void bloc::getAxes(vector<Vector2i> & axes, const int & angle)
{
	axes = _axes[angle];
}

// Retourne le profil en carr� de la forme.
void bloc::getProfil(vector<carre> & profil)
{
	profil = _formes;
}

// Affiche la forme du bloc dans l'�cran.
void bloc::draw(RenderWindow & window)
{
	for (auto & element : _formes)
		element.draw(window);
}

// Affiche la forme du bloc dans l'�cran � une position pr�cise.
void bloc::montre(RenderWindow & window, const Vector2f & coin)
{
	for (auto & element : _formes)
		element.montre(window, coin);
}

/// Rallentis la progression du bloc. (augmente le d�lais avant qu'il continu sa descente)
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
///=================== */


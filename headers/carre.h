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
using namespace std;
using namespace sf;


/* Prototypes des fonctions */

void setDimRect(RectangleShape & rect,
	const Vector2f & dimmensions, const float & bordure, const Vector2f & echelle);
void setPosRect(RectangleShape & rect,
	const Vector2f & pos, const Vector2i & place, const Vector2i & origine);
void setCouleurRect(RectangleShape & rect,
	const Color & couleur, const Color & couleurBord);
RectangleShape initRectangle(const Vector2f & dimmensions, const float & bordure,
	const Vector2f & echelle, const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
	const Color & couleur, const Color & couleurBord);

/* Constantes des carr�s */
/* ===================== */
const int MAXCARRE = 5;			// Nb maximal en x et en y de carr�s dans un bloc
const Vector2f CENTREDECARRE(18, 18); // Dimmensions du centre des carr�s
const int BORDUREDECARRE = 1;	// Largeur des bordures des carr�s
const Vector2f LARGUEURCARRE =	// Largeur des carr�s dans la fen�tre
/**/		Vector2f(CENTREDECARRE.x + BORDUREDECARRE * 2, CENTREDECARRE.y + BORDUREDECARRE * 2);
const Vector2f MILLIEUCARRE =	// Point central des carr�s
/**/		Vector2f(LARGUEURCARRE.x / 2, LARGUEURCARRE.y / 2);
const Vector2f MILLIEUBLOC =	// Point central des blocs
/**/		Vector2f((LARGUEURCARRE.x * MAXCARRE) / 2, (LARGUEURCARRE.y * MAXCARRE) / 2);
const Vector2f COIN = Vector2f(30, 30); // Position par d�faut de la salle
const Vector2i PLACE(9, 0);		// Position par d�faut des bloc dans la salle
const Vector2i BASE(0, 0);		// Valeur par d�faut de certaines composantes
const double PI = atan(1) * 4;	// Valeur de pi
/* Forme rectangulaire des carr�s avec les valeurs de base */
const RectangleShape RECT = initRectangle(CENTREDECARRE, BORDUREDECARRE, Vector2f(1, 1),
	COIN, PLACE, Vector2i(2, 2), Color(150, 150, 255, 125), Color());


/* Objets carr�s d'un bloc ou d'une salle */
/* ====================================== */
struct carre
{
private:
	Color _couleur = RECT.getFillColor();		// Couleur du carr� avant sa transparence
	Color _couleurBord = RECT.getOutlineColor();// Couleur de la bordure avant sa transparence
	RectangleShape _vue = RECT;			// La forme SFML du carr�
	Vector2i _origine = Vector2i(2, 2);	// Les coordonn�es du carr� dans le bloc
	int _etat = 0;						// L'�tat du carr� (selon son utilisation)

public:
	/// dim		= dimenssion du bloc
	/// pos		= position de la salle de son bloc
	/// place	= coordon�es du bloc dans la salle
	/// origine = coordon�es du carr� dans le bloc

	// Initialisateurs.
	carre();
	carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
		const Vector2f & dim, const float & bordure, const Vector2f & echelle,
		const Color & couleur, const Color & couleurBord, const int & etat);
	carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
		const Vector2f & dim, const float & bordure, const Vector2f & echelle, const int & etat);
	carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine);
	carre(const Color & couleur, const Color & couleurBord, const int & etat);
	// Destructeurs.
	~carre();

	// Change les propri�t�s du rectangle.
	void setRect(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
		const Vector2f & dim, const float & bordure, const Vector2f & echelle,
		const Color & couleur, const Color & couleurBord, const int & etat);
	void setPos(const Vector2f & pos, const Vector2i & place, const Vector2i & origine);
	void setDim(const Vector2f & dim, const float & bordure, const Vector2f & echelle);
	void setCouleur(const Color & couleur, const Color & couleurBord);
	void setVue(const RectangleShape & rect);
	void setEtat(const int & etat);
	void cache();
	void montre();

	// Change la position du carr� dans la salle.
	void deplace(const Vector2i axe);
	void rotate(const int & degree)
	{
		_vue.rotate(degree);
	}

	// Retourne les attributs du carr�.
	RectangleShape getVue();
	Color getColor();
	Vector2i getPos();
	int getX();
	int getY();
	int getEtat();

	// Dessine le carr� dans l'�cran
	void draw(RenderWindow & window);
};

/* M�thodes des carr�s */
/* =================== */

// Instanciation sans personalisation.
carre::carre() {}

// Instancie un carr� avec toute les attributs qu'il peut contenir.
carre::carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
	const Vector2f & dim, const float & bordure, const Vector2f & echelle,
	const Color & couleur, const Color & couleurBord, const int & etat)
{
	setRect(pos, place, origine, dim, bordure, echelle, couleur, couleurBord, etat);
}

// Instancie un carr� avec ses dimmenssions et sa position.
carre::carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
	const Vector2f & dim, const float & bordure, const Vector2f & echelle, const int & etat)
{
	setPosRect(_vue, pos, place, origine);
	setDimRect(_vue, dim, bordure, echelle);
	setEtat(etat);
}

// 
carre::carre(const Vector2f & pos, const Vector2i & place, const Vector2i & origine)
{
	setPosRect(_vue, pos, place, origine);
}

// Instancie un carr� avec ses couleurs.
carre::carre(const Color & couleur, const Color & couleurBord, const int & etat)
{
	setCouleur(couleur, couleurBord);
	setEtat(etat);
}

// Destructeur de carr�s.
carre::~carre()
{
	setVue(RectangleShape());
	_etat = 0;
}


// Change toules les valeurs du carr�.
void carre::setRect(const Vector2f & pos, const Vector2i & place, const Vector2i & origine,
	const Vector2f & dim, const float & bordure, const Vector2f & echelle,
	const Color & couleur, const Color & couleurBord, const int & etat)
{
	setVue(initRectangle(dim, bordure, echelle, pos, place, origine, couleur, couleurBord));
	setEtat(etat);
}

// Change la position du carr� dans son bloc et dans la salle.
void carre::setPos(const Vector2f & pos, const Vector2i & place, const Vector2i & origine)
{
	setPosRect(_vue, pos, place, origine);
}

// Change les dimmenssions du carr�.
void carre::setDim(const Vector2f & dim, const float & bordure, const Vector2f & echelle)
{
	setDimRect(_vue, dim, bordure, echelle);
}

// Change les couleurs du carr�.
void carre::setCouleur(const Color & couleur, const Color & couleurBord)
{
	setCouleurRect(_vue, couleur, couleurBord);
	setVue(_vue);
}

// Change le carr� pour un autre.
void carre::setVue(const RectangleShape & rect)
{
	_vue = rect;
	_couleur = rect.getFillColor();
	_couleurBord = rect.getOutlineColor();
}

// Change l'�tat du carr�.
void carre::setEtat(const int & etat)
{
	_etat = etat;
}

// Met transparent les couleurs du carr�.
void carre::cache()
{
	_vue.setFillColor(Color::Transparent);
	_vue.setOutlineColor(Color::Transparent);
}

// Remet les couleurs du carr� d'avant s'�tre cach�.
void carre::montre()
{
	_vue.setFillColor(_couleur);
	_vue.setOutlineColor(_couleurBord);
}

// D�place le carr�.
void carre::deplace(const Vector2i axe)
{
	_vue.move(Vector2f(axe.x * LARGUEURCARRE.x, axe.y * LARGUEURCARRE.y));
}

// Rotate


// Retourne la forme du carr�.
RectangleShape carre::getVue()
{
	return _vue;
}

// Retourne la couleur du centre du carr�.
Color carre::getColor()
{
	return _vue.getFillColor();
}

// Retourne les coordonn�es du carr� dans son bloc
Vector2i carre::getPos()
{
	return _origine;
}

// Retourne la position du carr� en x dans son bloc.
int carre::getX()
{
	return _origine.x;
}

// Retourne la position du carr� en y dans son bloc.
int carre::getY()
{
	return _origine.y;
}


// Retourne l'�tat du carr�.
int carre::getEtat()
{
	return _etat;
}

// Dessine le carr� dans l'�cran
void carre::draw(RenderWindow & window)
{
	window.draw(_vue);
}


/* Fonctions pour les carr�s */
/* ========================= */

// Modifie la dimmension, la bordure et l'�chelle du carr�.
void setDimRect(RectangleShape & rect,
	const Vector2f & dimmensions, const float & bordure, const Vector2f & echelle)
{
	rect.setSize(dimmensions);			// Dimmenssion du centre du carr�
	rect.setOutlineThickness(bordure);	// �paisseur de la bordure
	rect.setScale(echelle);				// Transforme ses dimmensions selon une �chelle
}

/// Modifie la position et le point pivot du carr�.
// / pos =		position de la salle dans la fen�tre
// / place =		coordon�es du bloc dans la salle
// / origine =	coordon�es du carr� dans le bloc
void setPosRect(RectangleShape & rect,
	const Vector2f & pos, const Vector2i & place, const Vector2i & origine)
{
	assert(origine.x >= 0 && origine.x < MAXCARRE &&
		origine.y >= 0 && origine.y < MAXCARRE &&
		place.x >= 0 && place.x <= 19 &&
		place.y >= 0 && place.y <= 19);

	rect.setPosition(Vector2f(			// Position dans la salle
		pos.x + (LARGUEURCARRE.x * place.x),
		pos.y + (LARGUEURCARRE.y * place.y)));
	rect.setOrigin(Vector2f(			// Point d'encrage du carr� dans le bloc
		MILLIEUBLOC.x - (LARGUEURCARRE.x * origine.x),
		MILLIEUBLOC.y - (LARGUEURCARRE.y * origine.y)));
}

// Modifie les couleurs du carr�.
void setCouleurRect(RectangleShape & rect,
	const Color & couleur, const Color & couleurBord)
{
	rect.setFillColor(couleur);			// Couleur principale
	rect.setOutlineColor(couleurBord);	// Couleur de la bordure
}

// Forme un carr� selon les crit�res voulu.
RectangleShape initRectangle(const Vector2f & dimmensions, const float & bordure,
	const Vector2f & echelle, const Vector2f & pos, const Vector2i & place,
	const Vector2i & origine, const Color & couleur, const Color & couleurBord)
{
	RectangleShape rect;
	setDimRect(rect, dimmensions, bordure, echelle);
	setPosRect(rect, pos, place, origine);
	setCouleurRect(rect, couleur, couleurBord);

	return rect;
}

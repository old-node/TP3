/* En-t�te du fichier
======================
Fichier:	    rectangle.h
Auteur:         Olivier Lemay Dostie
Date cr�ation:  11/04/2017
Description:    Librairie du lab 16 pour la cr�ation,
.				manipulation et affichage de rectangles.

Directives au pr�-processeur.
==========================*/
#pragma once
#include <assert.h>
#include <iostream>
using namespace std;

//d�claration de la classe
class rectangle {

private:
	double	_x = 0,		//Position en x le point sup�rieur gauche
		_y = 0,		//Position en y le point sup�rieur gauche
		_w = 0,		//largeur du rectangle
		_h = 0;		//hauteur du rectangle 

public:
	rectangle();		//Initialisation des valeurs � z�ro
	rectangle(double x, double y);//Initialisation des valeurs en coordonn�es
	rectangle(double x, double y, double w, double h);
	~rectangle();		//Initialisation des valeurs � z�ro

	double getX();		//Retourne a valeur de x
	double getY();		//ibid pour y
	double getW();		//ibid pour w
	double getH();		//ibid pour h

	void setX(double x);	//Change la valeur x du rectangle
	void setY(double y);	//ibid pour y
	void setW(double w);	//ibid pour w
	void setH(double h);	//ibid pour h
	void setPosition(double x, double y);	//Change les coordonn�es 
	void setDimension(double w, double h);	//Change les dimmentions
	void init(double x, double y, double w, double h);	//Change tout

	void affiche(ostream &sortie);	//�crit les donn�es du rectangle
	void trace(ostream &sortie);	//Dessine le p�rim�tre du rectangle

	double aire();			//Calcul l'aire du rectangle
	double perimetre();		//Calcul le p�rim�tre du rectangle	
};

//Constructeur sans param�etre qui initialise � 0
rectangle::rectangle() {
	_x = _y = _w = _h = 0;	//permet de nettoyer la m�moire
}

//Initialisation des coordonn�es
rectangle::rectangle(double x, double y) {
	setX(x);
	setY(y);
	_w = _h = 0;			//permet de nettoyer la m�moire
}

//Initialisation des donn�es au complet
rectangle::rectangle(double x, double y, double w, double h) {
	setX(x);
	setY(y);
	setW(w);
	setH(h);
}
//Initialisation vide
rectangle :: ~rectangle() {
	_x = _y = _w = _h = 0;	//permet de nettoyer la m�moire
}

//Retourne a valeur de x
double rectangle::getX() {
	return _x;
}

//ibid pour y
double rectangle::getY() {
	return _y;
}

//ibid pour w
double rectangle::getW() {

	return _w;
}

//ibid pour h
double rectangle::getH() {
	return _h;
}

//Change la valeur x du rectangle
void rectangle::setX(double x) {
	_x = x;
}

//ibid pour y
void rectangle::setY(double y) {
	_y = y;
}

//ibid pour w
void rectangle::setW(double w) {
	assert(w >= 0);
	_w = w;
}

//ibid pour h
void rectangle::setH(double h) {
	assert(h >= 0);
	_h = h;
}

//Change les coordonn�es 
void rectangle::setPosition(double x, double y) {
	setX(x);
	setY(y);
}

//Change les dimmentions
void rectangle::setDimension(double w, double h) {
	setW(w);
	setH(h);
}

//Change toute les valeurs du rectangle
void rectangle::init(double x, double y, double w, double h) {
	setX(x);
	setY(y);
	setW(w);
	setH(h);
}

//�crit les donn�es du rectangle
void rectangle::affiche(ostream &sortie) {
	sortie << "(" << _x << "," << _y << ") de " << _h << " x " << _w;
}

//Dessine le p�rim�tre du rectangle
void rectangle::trace(ostream &sortie) {
	sortie << endl;
	for (int i = 0; i < _w; i++) {	//Premi�re ligne
		sortie << (char)219 << (char)219;
	}
	sortie << endl;
	for (int i = 2; i < _h; i++) {	//Chaque ligne avec un corps vide
		sortie << (char)219 << (char)219;
		for (int i = 2; i < _w; i++) {//Corps vide
			sortie << "  ";
		}
		sortie << (char)219 << (char)219 << endl;
	}
	for (int i = 0; i < _w; i++) {	//�crit derni�re ligne
		sortie << (char)219 << (char)219;
	}
	sortie << endl;
}

//Calcul l'aire du rectangle
double rectangle::aire() {
	return _w * _h;
}

//Calcul le p�rim�tre du rectangle
double rectangle::perimetre() {
	return (_w + _h) * 2;
}
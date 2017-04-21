/* En-tête du fichier
======================
Fichier:	    rectangle.h
Auteur:         Olivier Lemay Dostie
Date création:  11/04/2017
Description:    Librairie du lab 16 pour la création,
.				manipulation et affichage de rectangles.

Directives au pré-processeur.
==========================*/
#pragma once
#include <assert.h>
#include <iostream>
using namespace std;

//déclaration de la classe
class rectangle {

private:
	double	_x = 0,		//Position en x le point supérieur gauche
		_y = 0,		//Position en y le point supérieur gauche
		_w = 0,		//largeur du rectangle
		_h = 0;		//hauteur du rectangle 

public:
	rectangle();		//Initialisation des valeurs à zéro
	rectangle(double x, double y);//Initialisation des valeurs en coordonnées
	rectangle(double x, double y, double w, double h);
	~rectangle();		//Initialisation des valeurs à zéro

	double getX();		//Retourne a valeur de x
	double getY();		//ibid pour y
	double getW();		//ibid pour w
	double getH();		//ibid pour h

	void setX(double x);	//Change la valeur x du rectangle
	void setY(double y);	//ibid pour y
	void setW(double w);	//ibid pour w
	void setH(double h);	//ibid pour h
	void setPosition(double x, double y);	//Change les coordonnées 
	void setDimension(double w, double h);	//Change les dimmentions
	void init(double x, double y, double w, double h);	//Change tout

	void affiche(ostream &sortie);	//Écrit les données du rectangle
	void trace(ostream &sortie);	//Dessine le périmètre du rectangle

	double aire();			//Calcul l'aire du rectangle
	double perimetre();		//Calcul le périmètre du rectangle	
};

//Constructeur sans paramèetre qui initialise à 0
rectangle::rectangle() {
	_x = _y = _w = _h = 0;	//permet de nettoyer la mémoire
}

//Initialisation des coordonnées
rectangle::rectangle(double x, double y) {
	setX(x);
	setY(y);
	_w = _h = 0;			//permet de nettoyer la mémoire
}

//Initialisation des données au complet
rectangle::rectangle(double x, double y, double w, double h) {
	setX(x);
	setY(y);
	setW(w);
	setH(h);
}
//Initialisation vide
rectangle :: ~rectangle() {
	_x = _y = _w = _h = 0;	//permet de nettoyer la mémoire
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

//Change les coordonnées 
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

//Écrit les données du rectangle
void rectangle::affiche(ostream &sortie) {
	sortie << "(" << _x << "," << _y << ") de " << _h << " x " << _w;
}

//Dessine le périmètre du rectangle
void rectangle::trace(ostream &sortie) {
	sortie << endl;
	for (int i = 0; i < _w; i++) {	//Première ligne
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
	for (int i = 0; i < _w; i++) {	//Écrit dernière ligne
		sortie << (char)219 << (char)219;
	}
	sortie << endl;
}

//Calcul l'aire du rectangle
double rectangle::aire() {
	return _w * _h;
}

//Calcul le périmètre du rectangle
double rectangle::perimetre() {
	return (_w + _h) * 2;
}
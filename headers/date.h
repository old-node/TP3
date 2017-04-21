/* En-t�te du programme
=======================
Programme:      date.h
Auteur:         Olivier Lemay Dostie
Date cr�ation:  17/04/2017
Description:    Lab 14. Cr�ation d'un objet date et les m�thodes de celle-ci.

/* Directives au pr�-processeur.
==============================*/
#pragma once
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <time.h>				//Pour obtenir la date du syst�me
#include <ctype.h>
#include <saisieSecurisee.h>
using namespace std;

//Constances li�s � l'objet date

const int _MA = 1900;			//Valeur de base des ans

struct evenement {	//Structure pour m�moriser des dates importantes
	int _jour = 1,					//Jour de l'�v�nement
		_mois = 1,					//Mois de l'�v�nement 
		_annee = _MA;				//Ann�e d'inauguration de l'�v�nement
	bool _aniversaire = false;	//Si la date est un aniversaire
};

//Dates importantes

//const evenement noel = { 25, 12, -1, false };	//Date de No�l


class date {	//Objet contenant les attributs et les m�thodes des dates

private:
	int _jour = 1,					//Jour de la date
		_mois = 1,					//Mois de la date
		_annee = _MA;				//Ann�e de la date

	char mois[12];					//Le nom du mois
	bool _bissextile = false;		//Si l'ann�e est bisextile
	const char nomMois[12][12] = {	//Nom des mois
		{ "Janvier\0" },{ "F�vrier\0" },{ "Mars\0" },
		{ "Avril\0" },{ "Mai\0" },{ "Juin\0" },
		{ "Juillet\0" },{ "Ao�t\0" },{ "Septembre\0" },
		{ "Octobre\0" },{ "Novembre\0" },{ "D�cembre\0" } };

public:

	//Initialisation
	date();							//Aux valeurs de bases
	date(int jour, int mois, int annee);		//Aux valeurs des param�tres

	~date();						//Remise � 0

	//Chande les valeurs de la date
	void setJour(int j);
	void setMois(int m);
	void setAnnee(int annee);
	void init(int jour, int mois, int annee);

	void aujourdhui();				//� celles du syst�me
	void saisir(ostream &sortie);	//� celles saisies

	//Obtien l'�l�ment de la date
	int getJour();
	int getMois();
	void getNomMois(char mois[]);					/**/
	int getAnnee();

	//�crit les donn�es de la date
	void afficheLong(ostream &sortie);			/**/
	void afficheCourt(ostream &sortie);

	//Augmente ou r�duit l'�l�ment de la date	/**/
	void ajouteAnnee(date v, int nbAnnee);
	void ajouteMois(date v, int nbMois);
	void ajouteJour(date v, int nbJour);

	void reduitAnnee(date v, int nbAnnee);
	void reduitMois(date v, int nbMois);
	void reduitJour(date v, int nbJour);
};


//Prototype des fonctions li�es � l'objet 

void saisir(date &v, ostream &sortie);
void valider(int &saisie, ostream &sortie,
	int min, int max, char message[]);
int diffJour(date v, date w);
int diffMois(date v, date w);
int diffAnnee(date v, date w);


//M�thodes de l'objet date

//Initialise la date avec les valeurs de base
date::date() {}

//Initialise la date avec les valeurs entr�es en param�tre
date::date(int jour, int mois, int annee) {
	setJour(jour);
	setMois(mois);
	setAnnee(annee);
}

//R�initialise les valeurs de la date � 0 (lib�re la m�moire)
date::~date() {
	_jour = _mois = _annee = 0;
}

//Change le jour de la date
void date::setJour(int j) {
	assert(j >= 1 && j <= 31);
	_jour = j;
}

//Change le mois de la date
void date::setMois(int m) {
	assert(m >= 1 && m <= 12);
	_mois = m;
	strcpy_s(mois, 12, nomMois[_mois - 1]);
	mois[0] = tolower(mois[0]);
}

//Change l'ann�e de la date
void date::setAnnee(int annee) {
	assert(annee>= _MA && annee <= 9999);
	_annee = annee;

	if ((annee% 4 == 0 && annee % 10 != 0) || annee % 400 == 0)
		_bissextile = true;
	else
		_bissextile = false;
}

//Initialise toute les �l�ments de la date
void date::init(int jour, int mois, int annee) {
	setJour(jour);	setMois(mois);	setAnnee(annee);
}

//Initialise la date selon celle du syst�me si le param�tre est vrai
void date::aujourdhui() {
	struct tm newtime;				//Structure tm qui contiendra la date du jours
	time_t now = time(NULL);		//Now contient le nombre de secondes depuis le 01-01-1970.

	localtime_s(&newtime, &now);	//Fait la conversion de now en structure tm

	_jour = newtime.tm_mday;		//Jours (1 - 31)
	_mois = newtime.tm_mon + 1;		//Mois  (0 - 11) + 1
	_annee = newtime.tm_year + _MA;	//Nombre d'ann�e depuis 1900 + 1900
}

//Saisie et v�rifie la saisie d'une date
void date::saisir(ostream &sortie) {

	int jour, mois, annee;

	//Saisie chaque �l�ment jusqu'� ce qu'ils soient valident
	sortie << "Saisissez le jour, le mois et l'ann�e de votre naissance : ";

	valider(jour, sortie, 1, 31,
		"\nLe jour saisi n'est pas valide, entrez une valeur entre 1 et 31 :");
	setJour(jour);

	valider(mois, sortie, 1, 12,
		"\nLe mois saisi n'est pas valide, entrez une valeur entre 1 et 12 :");
	setMois(mois);

	valider(annee, sortie, _MA, 9999,
		"\nL'ann�e saisie n'est pas valide, entrez une valeur entre 1900 et 9999 :");
	setAnnee(annee);
}

//Obtien le jour de la date
int date::getJour() {
	return _jour;
}

//Obtien le mois de la date
int date::getMois() {
	return _mois;
}

//Obtien le nom du mois
void date::getNomMois(char mois[]) {
	strcpy_s(mois, 12, mois);
}

//Obtien l'ann�e de la date
int date::getAnnee() {
	return _annee;
}

//�crit la date au long format
void date::afficheLong(ostream &sortie) {

	if (_jour == 1)
		sortie << "1er";
	else
		sortie << _jour;
	sortie << " " << nomMois[_mois - 1] << " " << _annee;
}

//�crit la date en format court
void date::afficheCourt(ostream &sortie) {
	sortie << setfill('0') << setw(2) << _jour << "/";
	sortie << setfill('0') << setw(2) << _mois << "/" << _annee;
}

//Augmente d'un nombre choisi le jour de la date
void date::ajouteJour(date v, int nbJour) {	//Calcul le p�rim�tre du rectangle	

	bool maxJ = false;		//Si la date du jour est le plus grand du mois
	int dateJ = v.getJour(),	//Jour actuel
		dateM = v.getMois();	//Mois actuel

	for (int i = 0; i < nbJour; i++) {

		//Si le mois est f�vrier, v�rifie selon l'ann�e bissextile
		if (dateM == 2) {
			if (dateJ >= 29 || (!_bissextile && dateJ >= 28)) {
				maxJ = true;
			}
		}
		//Pour les autres mois
		else {
			if (dateJ >= 31) {
				maxJ = true;
			}
			else if (dateJ == 30) {
				//V�rifie si le nombre de jour maximum du mois actuel est 30
				if ((dateM % 2 == 0 && dateM <= 7) ||
					(dateM % 2 != 0 && dateM >= 8)) {
					maxJ = true;
				}
			}
		}

		if (maxJ) {	//Change le mois et r�initialise la date du jour
			v.ajouteMois(v, 1);
			v.setJour(1);
		}
		else
			v.setJour(v.getJour() + 1);
	}
}

//Augmente d'un nombre choisi le mois de la date
void date::ajouteMois(date v, int nbMois) {

	for (int i = 0; i < nbMois; i++) {

		if (v.getMois() == 12) {
			v.setMois(1);
			ajouteAnnee(v, 1);
		}
		else {
			v.setMois(v.getMois() + 1);
		}
	}
}

//Augmente d'un nombre choisi l'ann�e de la date
void date::ajouteAnnee(date v, int nbAnnee) {
	for (int i = 0; i < nbAnnee; i++) {
		v.setAnnee(v.getAnnee() + 1);
	}
}

//R�duit d'un nombre choisi le jour de la date
void date::reduitJour(date v, int nbJour) {	//Calcul le p�rim�tre du rectangle	

	int dateJ = v.getJour(),	//Jour actuel
		dateM = v.getMois();	//Mois actuel
	int nbJourMois;			//Nombre de jours du mois pr�c�dant

	for (int i = 0; i < nbJour; i++) {

		//Change le mois et la date selon le mois pr�c�dant
		if (dateJ == 1) {

			//Si le mois pr�c�dant est f�vrier, change selon l'ann�e bissextile
			if (dateM == 3) {
				if (_bissextile)
					nbJourMois = 29;
				else
					nbJourMois = 28;
			}
			//Pour les autres mois
			else {
				if ((dateM % 2 != 0 && dateM <= 7) ||
					(dateM % 2 == 0 && dateM >= 8)) {
					nbJourMois = 31;
				}
				else
					nbJourMois = 30;
			}

			v.reduitMois(v, 1);
			v.setJour(nbJourMois);
		}
		else
			v.setJour(v.getJour() - 1);
	}
}

//R�duit d'un nombre choisi le mois de la date
void date::reduitMois(date v, int nbMois) {

	for (int i = 0; i < nbMois; i++) {

		if (v.getMois() == 1) {
			reduitAnnee(v, 1);
			v.setMois(12);
		}
		else {
			v.setMois(v.getMois() - 1);
		}
	}
}

//R�duit d'un nombre choisi l'ann�e de la date
void date::reduitAnnee(date v, int nbAnnee) {
	for (int i = 0; i < nbAnnee; i++) {
		v.setAnnee(v.getAnnee() - 1);
	}
}


//Fonctions li�es � l'objet date

//Saisie et v�rifie l'�l�ment d'une date selon une plage
void valider(int &saisie, ostream &sortie,
	int min, int max, char message[]) {

	cin >> saisie;			//Premi�re saisie

	do {
		//Valide la saisie
		while (cin.fail()) {
			viderBuffer();
			sortie << "\nN'entrez que des nombres pour saisir la date.";
			cin >> saisie;
		}
		//Met la saisie dans la bonne plage
		while (saisie < min || saisie > max) {
			viderBuffer();
			sortie << message;
			cin >> saisie;
		}
		//Rev�rifie la validit� de la saisie
	} while (cin.fail());
}

//Modifie l'enssemble des �l�ments par saisies
void saisir(date &v, ostream &sortie) {
	int jour, mois, annee;

	//Saisie chaque �l�ment jusqu'� ce qu'ils soient valident
	sortie << "Saisissez le jour, le mois et l'ann�e de votre naissance : ";

	valider(jour, sortie, 1, 31,
		"Le jour saisi n'est pas valide, entrez une valeur entre 1 et 31 :");
	v.setJour(jour);

	valider(mois, sortie, 1, 12,
		"Le mois saisi n'est pas valide, entrez une valeur entre 1 et 12 :");
	v.setMois(mois);

	valider(annee, sortie, _MA, 9999,
		"L'ann�e saisie n'est pas valide, entrez une valeur entre 1900 et 9999 :");
	v.setAnnee(annee);
}

//Calcul le nombre de jour de diff�rence entre deux dates
int diffJour(date v, date w) {

	int nbJours = diffMois(v, w) * 31;		//Nombre de mois * 31
	nbJours += abs(v.getJour() - w.getJour());//La diff�rence entre les deux jours

	return nbJours;
}

//Calcul le nombre de mois de diff�rence entre deux dates
int diffMois(date v, date w) {

	int nbMois = diffAnnee(v, w) * 12;		//Nombre d'ann�e * 12
	nbMois += abs(v.getMois() - w.getMois());	//La diff�rence entre les deux mois

	return nbMois;
}

//Calcul le nombre d'ann�e de diff�rence entre deux dates
int diffAnnee(date v, date w) {

	//	  Ann�es des dates � comparer
	int VA = v.getAnnee(), WA = w.getAnnee(),
		//Mois des dates � comparer
		VM = v.getMois(), WM = w.getMois(),
		//Jours des dates � comparer
		VJ = v.getJour(), WJ = w.getJour();

	//Si la premi�re date est plus petite que la deuxi�me
	if (VA < WA) {	//Inverse les valeurs pour faire les bonnes comparaisons
		swap(VA, WA);
		swap(VM, WM);
		swap(VJ, WJ);
	}

	int age = VA - WA;

	//Si le mois est avant celui du mois
	if (VM < WM) {
		age--;
	}

	else if (VM == WM) {
		//Si le jour est avant ou �gale � celui du jour
		if (VJ < WJ) {
			age--;
		}
	}

	return age;
}

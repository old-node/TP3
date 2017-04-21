/* En-t�te du fichier
======================
Fichier:	    utile.h
Auteur:         Olivier Lemay Dostie
Date cr�ation:  15/03/2017
Description:    Fichier qui regroupe toutes les fonctions personellement cr��s. 

Directives au pr�-processeur.
==========================*/
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <saisieSecurisee.h>
using namespace std;

/* Fonctions.
==========*/
//Construit et affiche une ligne.
string ligne(int longeur, char caractere) {
	string trait = "";
	//char macron = '\u00AF';
	for (int i = 0; i < longeur; i++)
		trait += caractere;
	return trait;
}
//Change la position du pointeur du fichier vers la ligne sp�cifi�.
void versLaLigne(ifstream& entree, int nb) {
	entree.seekg(ios::beg);	//Am�ne la lecture au d�but du fichier

	for (int i = 0; i < nb - 1; ++i) {
		entree.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}
//Tente d'ouvrir un fichier de lecture.
bool ouvertureLecture(ifstream& entree, string fichier, int nb) {
	//entree.open(fichier.c_str());			??
	if (!ouvrirFichier(entree, fichier, cout))
		return false;

	versLaLigne(entree, nb);
	if (siFichierVide(entree, cout))
		return false;

	return true;
}
//Effectue toute les v�rifications de lecture d'un fichier.
ifstream lectureCorrecte(string fichier, int nb, bool differentNom) {
	bool correct = true;	//Pour demander un autre nom de fichier
	ifstream entree;		//Fichier de lecture

	system("cls");
	correct = ouvertureLecture(entree, fichier, nb);

	while (correct == false && differentNom == true) {
		correct = true;

		//R�initialise le nom du fichier et la position de lecture de d�part
		fichier = saisirStringNonVide("Entrez de nouveau un nom pour votre fichier : ", cout);
		if (saisirCharValide("Est-ce que la ligne num�ro " + to_string(nb) +
			" est bien la position de lecture de d�part ? : ", 'o', 'n', cout) == 'n')
			nb = saisirIntPositif("� partir de quelle ligne voulez-vous lire votre fichier ? : ", cout);

		correct = ouvertureLecture(entree, fichier, nb);
	}

	entree.close();		//Recommand�s ?
	if (!correct)
		return entree;	//Peut causer des probl�mes s'il n'est pas ouvert ?
	entree.open(fichier.c_str());
	versLaLigne(entree, nb);
	return entree;
}
//Recursive chromatic sort
//int rechercheDichotomique(tab3, plage, nbRecherche) {
//	int pos = plage / 2;
//	if (tab3[pos] == nbRecherche)
//		return pos;
//	if (pos == 0 || pos == plage)
//		return plage;
//	if (tab3[pos] > nbRecherche)
//		rechercheDichotomique(tab3, pos, nbRecherche, pos);
//	else
//		rechercheDichotomique(tab3 + pos, plage - pos, nbRecherche, pos);
//}
/*Sort selection
gestion de la m�moire
	affiche le contenu abbr�g� des variables, demande le nombre � 
	traiter

//
//
//
//
//

/*
entree = lectureCorrecte("Notes.txt", 1, 1);


string ligne;
int noLigne = saisirIntPositif(
"� partir de quelle ligne voulez-vous lire votre fichier ? : ", 0);
ifstream entree;
entree = lectureCorrecte("fichier.txt", noLigne, true);
getline(entree, ligne);
cout << ligne << endl;
noLigne += 2;
versLaLigne(entree, noLigne);
getline(entree, ligne);
cout << ligne;
entree.close();
pause();*/
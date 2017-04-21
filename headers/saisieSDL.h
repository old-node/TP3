/* En-t�te du fichier
======================
Fichier:	    saisieSecurisee.h
Auteur:         Olivier Lemay Dostie & Jean-Alain Sainton
Date cr�ation:  16/02/2017
Description:    Fichier qui regroupe toutes les fonctions des laboratoires de
.				saisie et de validation en SDL, l'initialisation de fichiers.
.				Tout sans la librairie string. [+Fonctions sont en d�veloppement]
.				Non fonctionnel dans le projet pr�sent. */

/*Directives au pr�-processeur.
==========================*/
#pragma once
#include <fstream>
#include <iostream>
#include <sstream>	
#include <cctype>
#include <SDL.h>	
#include <SDL_ttf.h>
using namespace std;

/*Constantes, structures des variables et prototypes des fonctions
=================================================================*/

//Constantes globales des structures
const int NBCHARMAX = 15,	//Nombre de caract�res maximale d'une chaine
DIMLABYMAX = 30,			//Dimenssions des tableaux n�cessaires aux labyrinthes
CARRE = 20;					//Optionnel?

const SDL_Color noir = { 0, 0, 0 },//Palettes de couleurs disponibles
gris = { 100, 100, 100 },
blanc = { 255, 255, 255 },
bleu = { 50, 50, 200 };

const char nomsPolices[9][30] = {	//Noms des fichiers de polices disponibles
	{ "font_blockman-solid.ttf\0" }, { "font_Blokhed.ttf\0" },
	{ "font_Cubesity.ttf\0" }, { "font_orthogon.ttf\0" },
	{ "font_SHD_Technotype.ttf\0" }, { "font_subsquare.ttf\0" },
	{ "font_angelina.ttf\0" }, { "font_arial.ttf\0" },
	{ "font_Sharpz.ttf\0" } };

struct interaction {//Informations sur l'�cran d�fini.
	SDL_Color fond = { 0, 0, 0 };//Couleur du fond de l'�cran
	SDL_Window *fenetre = NULL;	//C�est pour �a qu�on l�initialise � NULL	
	SDL_Surface *ecran = NULL;	//L��toile signifie qu'ils sont des pointeurs
};

struct contenu {	//Information d'un �l�ment dans un �cran.
	SDL_Rect posText = { 15, 15, CARRE, CARRE };//Position du texte � afficher
	char police[30] = { "" };	//Nom de la police du texte par d�fault
	int mesure = 28;			//Grandeur de la police par d�faut
	SDL_Color encre = { 255, 255, 255 };//Couleur de la police par d�faut
};

struct affichage {	//Donn�es relatives � l'affichage de base dans un �cran.
	contenu w;					//Donn�es pour l'affichage d'un �l�ment
	interaction v;				//Donn�es pour les propri�t�s de l'�cran
};

struct coord {		//Coordo�es d'un objet dans un tableau.
	int x = 0,				//Pixels � partir de la gauche
		y = 0;				//Pixels � partir du haut
};

struct tuile {		//Donn�es associ�s � la case du labyrinthe.
	int id = 0;				//Entier lui correspond dans les fichiers .txt
	contenu w;				//Informations relative � l'affichage de l'�l�ment
	coord tab;				//Indexes de sa ligne et de sa colonne dans element
	char nom[NBCHARMAX],	//Nom ou description de la tuile
		corps[3] = { " \0" };//Lettre qu'on affiche si voulu
	bool vue = false;		//Son �tat de visibilit� dans l'affichage final
};

struct perso {		//Le personnage d'un.e joeur.euse et ses donn�es auxiliaires. 
	int vision = 9,			//Plage (diam�tre en tuile) de la visibilit� du joueur
		sauvegarde = 0,		//Le dernier niveau compl�t� du mode campagne
		dirrection = 0,		//Derni�re dirrection utilis�e
		nbActions = 0,		//Nombres d'actions que le joueur � fait
		nbBombe = 1,		//Nombre de bombe que le personnage poss�de
		delais = 0;			//Actions restantes avant l'explosion d'une bombe
	bool nouveau;			//pour savoir si le joueur reprend une ancienne partie
	int rang;				//le rang du joueur dans la liste de score
	tuile info;				//Donn�es du joueur
};

struct laby {		//Donn�es d'un labyrinthe.
	char nom[NBCHARMAX];	//Nom ou description du laby
	int no = -1;			//Son num�ro associ� dans la campagne
	tuile element[DIMLABYMAX][DIMLABYMAX];//Le tableau des tuiles du niveau
	SDL_Rect posText = { 0,0,0,0 };//Position et dimm. du texte dans l'�cran
	SDL_Color ton = { 200, 200, 200 };//Couleur des tuiles par d�faut
	tuile liste[10];		//Liste de tout les types de tuiles du niveau
};

/* Fonctions.
==========*/

//Concat�nne un nombre � un tableau de char suivi d'une autre chaine si voulu.
void concatenne(char ligne[], int nombre, char fin[]) {
	stringstream ss;
	if (nombre >= 0) {	//Si le param�tre entr� n'est pas n�gatif
		ss << nombre;	//Ajoute celui-ci dans le tableau de caract�re 
	}
	ss << fin;

	const string& tmp = ss.str();
	const char* cstr = tmp.c_str();

	strcat_s(ligne, strlen(ligne) + strlen(cstr) + 1, cstr);
	stringstream().swap(ss);
}

//Vide le buffer avant chaque saisie.
void viderBuffer() {
	cin.clear();
	cin.seekg(0, ios::end);

	//Si le flux est dans un �tat invalide, on le remet valide
	if (!cin.fail())	//Flux valide, donc le buffer est non vide
		cin.ignore(numeric_limits<streamsize>::max());
	else				//Flux invalide, donc le buffer est vide
		cin.clear();
}

//Attend que l'utilisateur p�se une touche.
int pause() {
	SDL_Event event;			//�v�nement capt� par SDL_WaitEvent

	while (true) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:			//Sur le x de la fen�tre
			return -2;
		case SDLK_ESCAPE:
			return -1;
		case SDL_KEYDOWN:		//Pour les autres touches
			if (event.key.keysym.sym)
				return 0;
		}
	}
}

//Attend que l'utilisateur p�se Enter.
int enter() {
	SDL_Event event;			//�v�nement capt� par SDL_WaitEvent

	while (true) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:			//Sur le x de la fen�tre
			return -2;
		case SDLK_ESCAPE:
			return -1;
		case SDL_KEYDOWN:		//Pour les autres touches
			if ((int)event.key.keysym.sym == SDLK_RETURN)
				return 0;
			break;
		}
	}
}

//Retourne 1 si la position d'un click  est � l'int�rieur d'un rectangle
int in_rect(int x, int y, SDL_Rect *r) {
	return (x >= r->x) && (y >= r->y) &&
		(x < r->x + r->w) && (y < r->y + r->h);
}

//Retourne vrai si le clic s'est fait � l'int�rieur d'un des rectangles.
bool boutonSouriSDL(SDL_Event &event, int &x, int &y, SDL_Rect *r[], int nbR) {
	x = event.button.x;	//...
	y = event.button.y;
	//SDL_GetMouseState(x, y);	une autre option?

	for (int i = 0; i < nbR; i++) {
		if (in_rect(x, y, r[i])) {
			return true;	//Si le click � �t� fait � l'int�rieur d'un rectangle
		}
	}
	return false;
}

//Saisir le click de la souri.
int saisirClick(int &x, int &y, SDL_Rect *r[], int nbR) {

	bool continuer = true;			//flag pour la fin de la boucle
	SDL_Event event;				//�v�nement capt� par SDL_WaitEvent
	SDL_WaitEvent(&event);

	while (continuer) {
		switch (event.type) {
		case SDL_QUIT:				//X de la fen�tre, on arr�te le programme
			continuer = false;
			return -2;
		case SDLK_ESCAPE:
			return (char)-1;
		case SDL_MOUSEBUTTONDOWN:
			continuer = !boutonSouriSDL(event, x, y, r, nbR);
			break;
		}
	}
	return 0;
}

//Ouvre la police et affiche le text � la position souhait�.
void afficheTexte(char message[], affichage &jeu) {

	//Pointeurs de cr�ation du texte
	TTF_Font *police = NULL;
	SDL_Surface	*texte = NULL;
	char fichier[30] = { "\0" };
	strcpy_s(fichier, 30, jeu.w.police);
	//Nous devons loader la police et celle-ci doit �tre trouv� par le programme. 
	//Vous devez mettre les polices dans le m�me r�pertoire que les images
	police = TTF_OpenFont(fichier, jeu.w.mesure);

	//Nous devons cr�er une image avec le texte, la police et la couleur
	texte = TTF_RenderText_Blended(police, message, jeu.w.encre);

	//Dessine l�image du texte cr�er avec TTF_RenderText_Blended
	SDL_BlitSurface(texte, NULL, jeu.v.ecran, &jeu.w.posText);
	//SDL_UpdateWindowSurface(fenetre);

	//On lib�re le pointeur de l�image du texte et de sa police
	SDL_FreeSurface(texte);
	TTF_CloseFont(police);
}

//Saisir charact�re
char saisirCharSDL() {
	char caractere = NULL;		//Saisie � retourner
	SDL_Event event;			//�v�nement capt� par SDL_WaitEvent
	bool manip = true;

	while (manip) {
		SDL_WaitEvent(&event);

		switch (event.type) {
		case SDL_QUIT:			//Sur le x de la fen�tre, on arr�te le programme
			SDL_Quit();
			exit(0);
			break;

		case SDL_KEYDOWN:		//Pour les autres touches
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				return ' ';
			}
			if (event.key.keysym.sym) {
				caractere = (char)event.key.keysym.sym;
				if (!isspace(caractere) && (int)caractere != -31)
					return caractere;
			}
			break;
		}
	}
}

//Retourne la saisie d'une touche en SDL.		//Tester juste avec cin aussi
void saisirTabCharSDL(char saisies[], int &longeur, 
					  char message[], affichage jeu) {

	char resultat[20] = { "\0" };//Ce qu'on affiche � l'�cran
	bool continuer = true;		//flag pour la fin de la boucle
	int posChar = 0;			//Compteur de nombre de caract�re saisis
	SDL_Event event;			//�v�nement capt� par SDL_WaitEvent
	bool majuscule = false;		//Change si Shift est pes� pour convertir les char

	afficheTexte(message, jeu);
	jeu.w.posText.y += jeu.w.mesure;
	SDL_UpdateWindowSurface(jeu.v.fenetre);

	do {
		SDL_WaitEvent(&event);

		switch (event.type) {
		case SDL_QUIT:			//Sur le x de la fen�tre
			posChar = -2;
			continuer = false;
			break;

		case SDL_KEYDOWN:		//Pour les autres touches

			if (event.key.keysym.sym == SDLK_ESCAPE) {
				posChar = -1;
				continuer = false;
				break;			//Pour la touche Escape
			}

			char caractere = (char)event.key.keysym.sym;
			switch (caractere) {

			case '\r': case '\X'://Les Enter (Return)
				continuer = false;
				break;
			case '\b':			//Backspace
				if (posChar > 0) {
					//Efface le dernier caract�re de la chaine
					posChar--;
					saisies[posChar] = ' ';
					resultat[posChar] = { saisies[posChar] };

					SDL_FillRect(jeu.v.ecran, &jeu.w.posText, 
						SDL_MapRGB(jeu.v.ecran->format, 0, 0, 0));
					afficheTexte(resultat, jeu);
					SDL_UpdateWindowSurface(jeu.v.fenetre);
				}
				break;
			case -31:			//Shift
				if (majuscule == false) {
					majuscule = true;
				}
				else {
					majuscule = false;
				}
				break;
			default:
				if (((int)caractere >= 97 && (int)caractere <= 122) 
					|| (int)caractere == 32
					|| ((int)caractere >= 48 && (int)caractere <= 57)) {

					if (posChar < longeur) {
						if (majuscule == true) {
							caractere -= 32;//Nous devrons nous assurer que les 
						}					//majuscules, eux, ne changent pas

						saisies[posChar] = caractere;
						resultat[posChar] = { saisies[posChar] };

						posChar++;
						SDL_FillRect(jeu.v.ecran, &jeu.w.posText, 
							SDL_MapRGB(jeu.v.ecran->format, 0, 0, 0));
						afficheTexte(resultat, jeu);
						SDL_UpdateWindowSurface(jeu.v.fenetre);
					}
					//Pour prendre en compte le shift peser ou non
					else if ((char)caractere == 'S0' || (char)caractere == 'SI') {
						SDL_UpdateWindowSurface(jeu.v.fenetre);
					}
				}
				saisies[posChar + 1] = '\0';
				break;
			}

		}

		if (continuer == false && posChar == 0) {

			SDL_FillRect(jeu.v.ecran, &jeu.w.posText, 
				SDL_MapRGB(jeu.v.ecran->format, 0, 0, 0));
			afficheTexte("Entrez des caract�res avant d'appuirer sur Enter.", jeu);
			SDL_UpdateWindowSurface(jeu.v.fenetre);
			continuer = true;
		}
	} while (continuer);

	longeur = posChar;
}

//Saisir un num�ro entre deux plages.
int saisirNumero(char message[], affichage &jeu, 
	int min, int dessus, int dessous, int max) {

	int naturel = 0;
	char saisie[2] = { "\0" };
	char incorrect[40] = { "Veillez entrer un num�ro entre " };
	concatenne(incorrect, min, " et ");
	concatenne(incorrect, max, ".");

	afficheTexte(message, jeu);
	SDL_UpdateWindowSurface(jeu.v.fenetre);
	jeu.w.posText.y += jeu.w.mesure;

	saisie[0] = saisirCharSDL();

	if (saisie[0] == ' ') {
		return -1;
	}
	naturel = atoi(saisie);
	afficheTexte(saisie, jeu);
	SDL_UpdateWindowSurface(jeu.v.fenetre);

	while (naturel < min || (naturel < dessus && naturel > dessous) ||
		naturel > max || !isdigit(saisie[0])) {

		afficheTexte(incorrect, jeu);
		SDL_FillRect(jeu.v.ecran, &jeu.w.posText,
			SDL_MapRGB(jeu.v.ecran->format, 
				jeu.v.fond.r, jeu.v.fond.g, jeu.v.fond.b));
		jeu.w.posText.y += jeu.w.mesure;
		afficheTexte(message, jeu);
		jeu.w.posText.y += jeu.w.mesure;
		SDL_UpdateWindowSurface(jeu.v.fenetre);

		saisie[0] = saisirCharSDL();
		naturel = atoi(saisie);
		afficheTexte(saisie, jeu);
		SDL_UpdateWindowSurface(jeu.v.fenetre);
	}

	naturel = atoi(saisie);
	return naturel;
}

//Saisie un nombre entier.
int saisirInt(char message[], affichage &jeu) {

	bool invalide = false;
	int naturel = 0,
		nbChar = 20;
	viderBuffer();

	afficheTexte(message, jeu);
	SDL_UpdateWindowSurface(jeu.v.fenetre);
	jeu.w.posText.h += jeu.w.mesure;

	char saisie[20];
	saisirTabCharSDL(saisie, nbChar, "", jeu);
	for (int i = 0; i < nbChar; i++) {
		if (!isdigit(saisie[i])) {
			invalide = true;
			break;
		}
	}

	while (invalide) {
		viderBuffer();
		afficheTexte("Veuillez entrer un nombre entier.", jeu);
		jeu.w.posText.y += jeu.w.mesure;
		afficheTexte(message, jeu);
		jeu.w.posText.y += jeu.w.mesure;
		SDL_UpdateWindowSurface(jeu.v.fenetre);

		saisirTabCharSDL(saisie, nbChar, "", jeu);
		for (int i = 0; i < nbChar; i++) {
			if (isalnum(saisie[i])) {
				invalide = true;
				break;
			}
		}
	}

	naturel = atoi(saisie);
	return naturel;
}

//V�rifie que la saisie est entier et positive.
int saisirIntPositif(char message[], affichage &jeu) {

	int naturel;

	naturel = saisirInt(message, jeu);
	while (naturel < 0) {

		afficheTexte(
			"Veuillez entrer une entier positif, plus grand que 0.", jeu);
		SDL_UpdateWindowSurface(jeu.v.fenetre);

		naturel = saisirInt(message, jeu);
	}

	return naturel;
}

//Saisie un nombre r�el.
float saisirFloat(char message[]) {
	char saisie[1000] = { '\0' };	//Valeur saisie
	float rationnel;		//� retourner

	viderBuffer();
	cout << message;
	cin >> rationnel;

	while (cin.fail()) {
		viderBuffer();
		cout << "\nVeuillez entrer un nombre r�el.\n" << message;
		cin >> rationnel;
	}

	return rationnel;
}

//V�rifie que la saisie est r�el et positive.
float saisirFloatPositif(char message[]) {
	float rationnel;

	rationnel = saisirFloat(message);
	while (rationnel < 0) {
		cout << "\nVeuillez entrer une valeur positive et non nulle.";
		rationnel = saisirFloat(message);
	}

	return rationnel;
}

//Retourne la r�ponse � la question.
char saisirCharValide(char question[], 
	char choix1, char choix2, affichage jeu) {
	char choix;			//R�ponse saisie

	afficheTexte(question, jeu);
	SDL_UpdateWindowSurface(jeu.v.fenetre);

	viderBuffer();
	choix = tolower(saisirCharSDL());

	while (choix != choix1 && choix != choix2) {
		char reprise[50] = { "Choisissez entre " };
		reprise[strlen(reprise)] = choix1;
		strcat_s(reprise, 51, " ou ");
		reprise[strlen(reprise)] = choix2 + '.';
		afficheTexte(reprise, jeu);
		afficheTexte(question, jeu);
		SDL_UpdateWindowSurface(jeu.v.fenetre);

		viderBuffer();
		choix = tolower(saisirCharSDL());
	}

	return choix;
}

//Demande � l'utilisateur s'il veut rejouer
char questionOuiNon(char question[], affichage &jeu) {
	char choix;			//R�ponse saisie

	afficheTexte(question, jeu);
	SDL_UpdateWindowSurface(jeu.v.fenetre);

	viderBuffer();
	choix = tolower(saisirCharSDL());

	while (choix != 'n' && choix != 'o') {

		afficheTexte("Choisissez entre 'o' ou 'n'.", jeu);
		afficheTexte(question, jeu);
		SDL_UpdateWindowSurface(jeu.v.fenetre);

		viderBuffer();
		choix = tolower(saisirCharSDL());
	}

	return choix;
}

//Retourne le stream si le fichier de lecture trouv�.
bool ouvrirFichier(ifstream &entree, char fichier[]) {
	entree.open(fichier);
	if (entree.is_open())
		return true;

	//Si le nom n'est pas ad�quat
	cout << "Le fichier '" << fichier << "' n�a pas �t� trouv�.\n";
	//pause();
	return false;
}

//Retourne le stream si le fichier de lecture est non vide.
bool siFichierVide(ifstream& entree) {
	if (entree.eof()) { 	//Si la fin du fichier est atteinte
		cout << "Le fichier s�lectionn� ne contient aucune valeur.\n";
		//pause();
		return true;
	}
	return false;
}
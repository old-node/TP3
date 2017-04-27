/* En-tête du programme
=======================
Programme:      theTwist.cpp
Auteur:			Jean-Alain Sainton & Olivier Lemay Dostie
Date création:	21/04/2017
Description:	Projet final en C++. Version personalisé du jeu Tetris en SFML.
.				.*/

/* Directives au pré-processeur.
==============================*/
#include <iostream>	//Console
#include <iomanip>	
#include <stdlib.h>	
#include <math.h>	//permet de trouver le minimun ou le maximum entre 2 valeurs
#include <saisieSecurisee.h>
#include <locale>	
#include <vector>	
#include <pieces.h>	
using namespace std;


//Prototypes des fonctions
enum class Coords { X, Y, Id };
struct Entity
{
	string name;
	int x, y;
	char team;
	int health;
};

/* Programme principal.
===================== */
int main() {
	vector<Entity> entity;
	vector<int> array2 = { 9, 7, 5, 3, 1 };	// use initializer list to initialize array
	vector<int> array3{ 9, 7, 5, 3, 1 };		// use uniform initialization to initialize array (C++11 onward)

	array2 = *new vector<int>(6, 1);// initialize all members to 0
	array2.resize(3);
	array2.resize(6, 99);
	static const int arr[] = { 10,20,30 };
	vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

	for (auto const &element : array3)
		cout << element << ' ';		//Boucle pour toute le vector
	cout << endl;
	double map[3] = { 10.1, 2.1, 9.6 };
	
	vector<vector<double>> positionList;
	positionList.push_back(vector<double>(5));		//
	positionList.push_back(vector<double>(6, 4));	//Ajoute une ligne de 3 cases (4) dans le vector
	positionList[0].push_back(8);	// Ajoute une case (8) à la première ligne
	double i = positionList.at(1).size();
	positionList.at(1).at(positionList.at(1).size() - 1) = 9;
	//positionList[2][3] = 9;	// Change la valeur (si la case existe ???)

	vector<vector<int>> vector2d{ { 1, 2, 3 },{ 4, 5, 6 } };
	for (const auto &v : vector2d)	// iterate through each inner vector
		for (const int x : v)		// iterate through each int in that inner vector
			cout << x << " ";
	pause();
}
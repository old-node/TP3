/* En-t�te du programme
=======================
Programme:      SFML.cpp
Auteur:         Olivier Lemay Dostie & Jean-Alain Sainton
Date cr�ation:  18/04/2017
Description:    Projet final en C++. Tests de la SFML. */

/* Directives au pr�-processeur.
==============================*/
#include <locale>

#include <date.h>
#include <rectangle.h>
#include <saisieSecurisee.h>
#include <utile.h>
//#include <saisiesSDL.h>	//non fonctionnel tant qu'on n'a pas la librairie d'install�

#include <SFML/Graphics.hpp>
using namespace sf;			//Si n�cessaire, on peut sinon garder les 'sf::' avant les composantes. �a d�pend, 
//p-e qu'on peut alterner entre les deux namespace ou ben on ajoute le suffixe si on utilise un autre

/* Programme principal.
==================== */
int main() {
	RenderWindow window(VideoMode(200, 200), "SFML works!");
	CircleShape shape(100.f);
	shape.setFillColor(Color::Green);

	while (window.isOpen()) {

		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	pause();

	return 0;
}
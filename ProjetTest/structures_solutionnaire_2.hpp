#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include "gsl/span"
#include <memory>
using gsl::span;
using namespace std;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const std::string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	Acteur* trouverActeur(const std::string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

// struct ListeActeurs {
// 	int capacite, nElements;
// 	unique_ptr<Acteur*>elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
// };
class ListeActeurs {
public:	
	ListeActeurs() {};
	ListeActeurs(int capacite, int nElements)
		: capacite_(capacite), nElements_(nElements)
	{
		elements_ = make_unique<Acteur * []>(capacite) ;
	}

	ListeActeurs(const ListeActeurs& liste1) { capacite_ = liste1.capacite_; nElements_ = liste1.nElements_; }

	int getCapacite() { return capacite_; }
	int getNElements() { return nElements_; }
	
	Acteur** getElements() { 
		return elements_.get();
	};
	span<Acteur*> spanListeActeurs() const { return span(elements_.get(), nElements_); }
	void ajouterActeurListe(Acteur* nom) { elements_[nElements_] = nom; }
	 // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
	void setNElements(int nElements) {
		this->nElements_ = nElements;
	}
	void enleverActeur(const Acteur* acteur)
	{	
		for (Acteur*& element : spanListeActeurs()) {  // Doit être une référence au pointeur pour pouvoir le modifier.
			if (element == acteur) {
				if (getNElements() > 1)
					element = elements_[nElements_ - 1];
				nElements_--;
				return;
			}
	}
}
private:
	int capacite_ = 0;
	int nElements_ = 0;
	unique_ptr<Acteur* []> elements_ = make_unique<Acteur* []>(1);
};


struct Film
{
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
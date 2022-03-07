#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include "gsl/span"
#include <memory>
#include <functional>
using gsl::span;
using namespace std;
struct Acteur; 
class Film; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const std::string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const std::string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; };
	Film* operator[] (int index) const { return elements[index]; };
	Film* trouverFilm(const function<bool(const Film*)>critere) {
		for(Film* film: enSpan()) {
			if(critere(film))
			return film;
		}
		return nullptr;
	};

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
template <typename T>
class Liste {
public:	
	Liste() {};
	Liste(int capacite, int nElements)
		: capacite_(capacite), nElements_(nElements)
	{
		elements_ = make_unique<shared_ptr<T>[]>(capacite) ;
	};
	Liste(int nActeurs)
		: capacite_(nActeurs), nElements_(nActeurs)
	{
		elements_ = make_unique<shared_ptr<T>[]>(nActeurs) ;
	};
	Liste<T>(const Liste<T>& liste1) { 
		capacite_ = liste1.capacite_; 
		nElements_ = liste1.nElements_; 
		elements_ = make_unique<shared_ptr<T>[]>(capacite_) ;
		for (int i = 0; i < liste1.nElements_ ; i++) {
			elements_[i]= liste1.elements_[i];
		}
	};

	Liste<T>& operator= (Liste<T>&& copie) noexcept = default;
	int getCapacite() const { return capacite_; }
	int getNElements() const { return nElements_; }
	
	const shared_ptr<T>*  getElements() { 
		return elements_.get();
	};

	shared_ptr<T>& operator[] (int index) const { return elements_[index]; }

	span<shared_ptr<T>> spanListe() const
	{ return span(elements_.get(), nElements_); }

	void ajouterElementListe(shared_ptr<T> nom) { elements_[nElements_++] = move(nom); }
	 // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
	void setNElements(int nElements) {
		this->nElements_ = nElements;
	};


private:
	int capacite_ = 0;
	int nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_ = make_unique<shared_ptr<T>[]>(1);
};

using ListeActeurs = Liste<Acteur>;

// struct Film
// {
// 	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
// 	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
// 	ListeActeurs acteurs;
// };

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};

class Item
{
public:
	Item() {};
	Item(string titre, int annee):titre_(titre), anneePublication_(annee) {};
	string getTitre() const { return titre_; };
	int getAnnee() const { return anneePublication_; };
	void setAnneePublication(int nouvelleAnnee) {this->anneePublication_ = nouvelleAnnee;};
	void setTitre( string nouveauTitre) {this->titre_ = nouveauTitre; };

protected:
	string titre_;
	int anneePublication_ = 0;
};


class Livre : public Item
{
public:
	Livre():Item() {};
	Livre(std::string titre, int annee, std::string auteur, int ventes, int pages):
		Item(titre,annee), 
		auteur_(auteur), 
		ventes_(ventes), 
		pages_(pages) 
	{};
	string getAuteur() const {return auteur_; };
	int getNombreVentes() const {return ventes_;};
	int lirePages() const { return pages_; };
	void setAuteur(string nouveau) { this->auteur_ = nouveau;};
	void modifierVentes(int nouvelle) { this->ventes_ = nouvelle; };
	void modifierPages(int nouvelle) { pages_ = nouvelle; };

private:
	string auteur_;
	int ventes_ = 0;
	int pages_ = 0;
};

class Film : public Item
{

public:
	std::string realisateur_;
	int recette_ = 0;
	ListeActeurs acteurs_;

	Film() :Item() {};
	Film(std::string titre, int annee, std::string realisateur, int recette, ListeActeurs acteurs) :
		Item(titre, annee),
		realisateur_(realisateur),
		recette_(recette),
		acteurs_(acteurs)
	{};


	const shared_ptr<Acteur>* getActeurs()  { return acteurs_.getElements();};
	span<shared_ptr<Acteur>> spanListeActeurs() const	{ return acteurs_.spanListe(); };
	shared_ptr<Acteur>& operator[] (int index) const { return acteurs_[index]; }
	string getRealisateur() const { return realisateur_; };
	int getRecette() const { return recette_; };
	void setRealisateur(std::string nouveau) 
	{ this->realisateur_ = nouveau; };
	void setRecette(int nouvelle) { this->recette_ = nouvelle;}
	void setActeurs(ListeActeurs nouvelle) {this->acteurs_ = ListeActeurs(nouvelle); };
};
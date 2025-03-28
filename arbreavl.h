
/* here just as needed to test Journal.h et journal.cpp 
 * but the professor will test with a non provided 
 * implementation the only methods or attribut that 
 * can be added should be in the private
 */
#if !defined(__ARBREAVL_H__)
#define __ARBREAVL_H__

#include <assert.h>
#include <stdio.h>
#include "pile.h"

template <class T>
class ArbreAVL {
  public:
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();

    void inserer(const T&);
    void enlever(const T&);
    bool contient(const T&) const;
    bool vide();
    bool vide() const; /////should not be here
    void vider();
    int  hauteur() const;

    ArbreAVL& operator = (const ArbreAVL&);

    class Iterateur;
    
    Iterateur debut() const;
    Iterateur fin() const;
    
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);

  private:
    class Noeud;
    Noeud* racine;

    bool inserer(const T&, Noeud*&);

    bool enlever(const T& e, Noeud*& n);

    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);

    void vider(Noeud*&);
    void copier(const Noeud*, Noeud*&) const;
    
    const T& max(Noeud*) const;

  public:
    class Iterateur{
      public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
        
        const T& operator*() const;

        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
      private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        
      friend class ArbreAVL;
    };

  private:
    class Noeud{
      public:
        Noeud(const T&);
        Noeud *gauche;
        Noeud *droite;
        int equilibre;
        T element;
    };
};

//-----------------------------------------------------------------------------

template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c):
gauche(NULL), droite(NULL), equilibre(0), element(c)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL() : racine(NULL)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL& autre) : racine(NULL)
{
    copier(autre.racine, racine);
}

template <class T>
ArbreAVL<T>::~ArbreAVL()
{
    vider(racine);
}

template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(element, racine);
}

template <class T>
bool ArbreAVL<T>::inserer(const T& element, Noeud*& noeud)
{
    if(noeud==nullptr)
    {
        noeud = new Noeud(element);
        return true;
    }
    if(element < noeud->element){
        if(inserer(element, noeud->gauche))
        {
        noeud->equilibre++;
        if(noeud->equilibre == 0) return false;
        if(noeud->equilibre == 1) return true;
        assert(noeud->equilibre==2);
        if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    }
    else if(noeud->element < element){
        if(inserer(element,noeud->droite))
        {
            noeud->equilibre--;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == -1) return true;
            assert(noeud->equilibre == -2);
            if(noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    }
    noeud->element = element;  
    return false;
}


template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    enlever(racine, element);
}

template <class T>
bool ArbreAVL<T>::enlever(const T& element,Noeud*& noeud)
{
    if(noeud == NULL)
        return false;

    if(element < noeud->element)
    {
        bool r = false;
        if(enlever(noeud->gauche, element))
        {
            noeud->equilibre--;
            if (noeud->equilibre == -1) return false;
            if (noeud->equilibre == 0) return true;
            r = noeud->droite->equilibre != 0;
            if (noeud->droite->equilibre == 1) rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);

        }
        return r;
    }
    else if( noeud->element < element)
    {

        bool l = false;
        if (enlever(noeud->droite,element))
        {
            noeud->equilibre++;
            if (noeud->equilibre == 1) return false;
            if (noeud->equilibre == 0) return true;
            l = noeud->gauche->equilibre != 0;
            if (noeud->gauche->equilibre == -1) rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return l;
    }

    Noeud* tmp = noeud;
    if (noeud->gauche == NULL)
    {
        noeud = noeud->droite;
        delete tmp;
        return true;
    }
    else if (noeud->droite == NULL)
    {
        noeud = noeud->gauche;
        delete tmp;
        return true;
    }
    noeud->element = max(noeud->gauche);
    noeud = tmp;
    return enlever(noeud->gauche, noeud->contenu);
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea>0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->droite;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = - (ea < 0 ? ea : 0) + 1 + eb;
    int  nea = ea + (neb > 0 ? neb : 0) + 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
bool ArbreAVL<T>::vide() 
{
    return racine==nullptr;
}

template <typename T>
bool ArbreAVL<T>::vide() const {
    return racine == nullptr;
}


template <class T>
void ArbreAVL<T>::vider(){
  vider(racine);
  //racine = nullptr;  
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud) { 
    if(noeud != nullptr) {
        vider(noeud->gauche);
        vider(noeud->droite);
        delete noeud; 
        noeud = nullptr;
    }
}

 template <class T>  
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const {
    if(source != nullptr) {
        noeud = new Noeud(source->element);
        noeud->equilibre = source->equilibre;
        copier(source->gauche, noeud->gauche);
        copier(source->droite, noeud->droite);
    }
}

template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
    ArbreAVL<T>::Iterateur iter = rechercher(element);
    return iter.courant!= NULL;
}

template <class T>
int  ArbreAVL<T>::hauteur() const{
    int hauteur = 0;
    Noeud* tmp = racine;
    while (tmp != NULL) {
        hauteur++;
        if (tmp->equilibre == -1)
        {
            tmp = tmp->droite;
        }
        else
        {
            tmp = tmp->gauche;
        }
    }

    return hauteur;
}


template <class T>
const T& ArbreAVL<T>::max(Noeud* n) const
{
    assert(n != NULL);
    if (n->droite == NULL) return n->element;
    return max(n->droite);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const {
    Iterateur iter(*this);
    iter.courant = this->racine;
    while(iter && iter.courant->gauche != nullptr) {
        iter.chemin.empiler(iter.courant);
        iter.courant = iter.courant->gauche;
    }
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const
{
    Iterateur iter(*this);
    Noeud* n = racine;
    while(n) {
        if (e < n->element) {
            iter.chemin.empiler(n);
            n = n->gauche;
        } else if (n->element < e) {
            n = n->droite;
        } else {
            iter.courant = n;
            return iter;
        }
    }

    iter.chemin.vider();
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
    Iterateur iter(*this);
    Noeud* n = racine, *dernier = NULL;
    while (n) {
        if (e < n->element) {
            dernier = n;
            n = n->gauche;
        } else if (n->element < e) {
            n = n->droite;
        } else {
            return rechercher(n->element);
        }
    }
    if (dernier != NULL) {
        return rechercher(dernier->element);
    }
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
{
    Iterateur iter(*this);
    Noeud *n = racine, *dernier = NULL;
    while(n) {
        if (e < n->element){
            n = n->gauche;
        } else if (n->element < e) {
            dernier = n;
            n = n->droite;
        } else {
            return rechercher(n->element);
        }
    }
    if (dernier != NULL)
        return rechercher(dernier->element);
    return Iterateur(*this);
}
template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->element;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->element;
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if(this==&autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

//-----------------------
template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
 : arbre_associe(a), courant(NULL)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
: arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

// Pres-increment 
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++()
{
    assert(courant != NULL);
    Noeud* suivant = courant->droite;
    while (suivant != NULL)
    {
        chemin.empiler(suivant);
        suivant = suivant->gauche;
    }
    if (!chemin.vide())
        courant = chemin.depiler();
    else
        courant = NULL;
    return *this;
}

// Post-increment
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const
{
    assert(courant!=NULL);
    return courant->element;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre) {
    
    if (this == &autre) {
        return *this;
    }

    assert(&arbre_associe == &autre.arbre_associe);

    if (this != &autre) {  
        courant = autre.courant;   
        chemin = autre.chemin;     
 return *this; 
    }
}

#endif


/* here just as needed to test Journal.h et journal.cpp 
 * but the professor will test with a non provided 
 * implementation the only methods or attribut that 
 * can be added should be in the private
 */

 #if !defined(__ARBREMAP_H__)
 #define __ARBREMAP_H__
 
 #include "arbreavl.h"
 
 using namespace std;
 
 template <class K, class V> class ArbreMap {
 
   class Entree {
   private:
     K cle;
     V valeur;
 
     Entree(const K _cle) : cle(_cle) {}
 
     bool operator==(const Entree &other) const { return cle == other.cle; }
 
     bool operator<(const Entree &other) const { return cle < other.cle; }
 
     template <typename T> friend class ArbreAVL;
 
     friend class ArbreMap;
   };
 
 public:
   class Iterateur {
   public:
     Iterateur(ArbreMap &a) : iter(a.entrees.debut()) {}
     Iterateur(typename ArbreAVL<Entree>::Iterateur i) : iter(i) {}
     // not sure if it's correct
     operator bool() const { return iter; };
     Iterateur &operator++() {
       ++iter;
       return *this;
     }
     const K &cle() const { return (*iter).cle; }
     V &valeur() { return (V &)(*iter).valeur; }
 
   private:
     typename ArbreAVL<Entree>::Iterateur iter;
   };
 
   Iterateur debut() { return Iterateur(*this); }
   Iterateur fin() { return Iterateur(entrees.fin()); }
   //
   Iterateur rechercher(const K &cle) {
     return Iterateur(rechercher(entree(cle)));
   }
   Iterateur rechercherEgalOuSuivant(const K &cle) {
     return Iterateur(rechercherEgalOuSuivant(entree(cle)));
   }
   Iterateur rechercherEgalOuPrecedent(const K &cle) {
     return Iterateur(rechercherEgalOuPrecedent(entree(cle)));
   }
 
   bool contient(const K &) const;
   void enlever(const K &);
   void vider();
   bool vide() const;
/*
   void afficher(
       ArbreMap<string, string> &dictionnaire); /// do we need it?
*/
   const V &operator[](const K &) const;
   V &operator[](const K &);
 
 private:
   ArbreAVL<Entree> entrees;
 };
 
 template <class K, class V> void ArbreMap<K, V>::vider() { entrees.vider(); }
 
 template <class K, class V> bool ArbreMap<K, V>::vide() const {
   return entrees.vide();
 }
 
 template <class K, class V> void ArbreMap<K, V>::enlever(const K &c) {
   entrees.enlever(Entree(c));
 }
 
 template <class K, class V> bool ArbreMap<K, V>::contient(const K &c) const {
   return entrees.contient(Entree(c));
 }
/*
 // maybe needed
 template <class K, class V>
 void ArbreMap<K, V>::afficher(ArbreMap<string, string> &dictionnaire) {
   ArbreMap<string, string>::Iterateur iter = dictionnaire.debut();
   for (; iter; ++iter) {
     cout << iter.cle() << ":" << iter.valeur() << endl;
   }
 }
 */
 template <class K, class V>
 const V &ArbreMap<K, V>::operator[](const K &c) const {
   typename ArbreAVL<Entree>::Iterateur iter = entrees.rechercher(c);
   return (*entrees.rechercher(Entree(c))).valeur;
 }
 
 template <class K, class V> V &ArbreMap<K, V>::operator[](const K &c) {
   const Entree entreeTemplate = Entree(c);
   typename ArbreAVL<Entree>::Iterateur searched =
       entrees.rechercher(entreeTemplate);
   if (!searched) {
     entrees.inserer(Entree(entreeTemplate));
     searched = entrees.rechercher(entreeTemplate);
   }
   return entrees[searched].valeur;
 }
 
 #endif
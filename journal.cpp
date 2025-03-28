/* to be implemented without any signature modification 
 *  and without any new include  nor adding a new method
 */
#include "journal.h"

void Journal::enregistrerService(const Date& date, const string& nompersonne, const string& nomcocktail)
{
   // Chercher ou insérer un nouvel arbre pour la date donnée
   ArbreMap<string, ArbreAVL<string>>& cocktailsParPersonne = services[date];

   // Search or insert a tree for the cocktails ordered by each person
   ArbreAVL<string>& cocktails = cocktailsParPersonne[nompersonne];

   // Add a cocktail to a personne
   cocktails.inserer(nomcocktail);
}


ArbreMap<string, int> Journal::requeteQui(const Date& debut, const Date& fin, const ArbreAVL<string>& cocktails) const {
    ArbreMap<std::string, int> resultat;

    // Copy services into a local non-constant variable
    auto copieServices = services;

    for (auto iter = copieServices.debut(); iter; ++iter) {
        const Date& dateService = iter.cle();

        if (dateService > debut && dateService <= fin) {
            // Copy cocktailsParPersonne into a local variable
            auto copieCocktailsParPersonne = iter.valeur();

            for (auto pIter = copieCocktailsParPersonne.debut(); pIter; ++pIter) {
                const std::string& nompersonne = pIter.cle();
                auto copieCocktailsPersonne = pIter.valeur();

                int count = 0;

                if (cocktails.vide()) { 
                    // If no specific cocktail is given, count all
                    for (auto cIter = copieCocktailsPersonne.debut(); cIter; ++cIter) {
                        count++;
                    }
                } else {  
                    // Otherwise, count only those listed in cocktails.
                    for (auto cIter = copieCocktailsPersonne.debut(); cIter; ++cIter) {
                        if (cocktails.contient(*cIter)) {
                            count++;
                        }
                    }
                }

                if (count > 0) { // Add only if at least one cocktail is counted.
                    resultat[nompersonne] += count;
                }
            }
        }
    }

    return resultat;
}


ArbreMap<string, int> Journal::requeteQuoi(const Date& debut, const Date& fin, const ArbreAVL<std::string>& buveurs) const {
    ArbreMap<std::string, int> resultat;

    auto copieServices = services;
    // Iterates on services to check the dates
    for (auto iter = copieServices.debut(); iter; ++iter) {
        const Date& dateService = iter.cle();

        // If the date is within the interval, continue.
        if (dateService > debut && dateService <= fin) {
            const ArbreMap<std::string, ArbreAVL<std::string>>& cocktailsParPersonne = iter.valeur();
            
            auto copieCocktailsParPersonne = iter.valeur();
            // Iterate through people who received cocktails.
            for (auto pIter = copieCocktailsParPersonne.debut(); pIter; ++pIter) {
                const std::string& nompersonne = pIter.cle();

               // Iterate through the cocktails associated with each person.
for (auto cIter = pIter.valeur().debut(); cIter; ++cIter) {
    const std::string& cocktail = *cIter;  // Retrieve the value directly (std::string).
    
    // Count the number of times each cocktail is consumed.
    resultat[cocktail]++;
}

            }
        }
    }

    return resultat;
}

ArbreAVL<string> Journal::requetePresences(const Intervalle& i) const {
    ArbreAVL<std::string> presences;
    auto copieServices = services;
    // Iterates on all services.
    for (auto iter = copieServices.debut(); iter; ++iter) {
        const Date& dateService = iter.cle();

        // Check if the service date is within the interval.
        if (i.debut <= dateService && dateService <= i.fin) {
            const ArbreMap<std::string, ArbreAVL<std::string>>& cocktailsParPersonne = iter.valeur();
            auto copieCocktailsParPersonne = iter.valeur();
            // Add all people to the ArbreAVL (AVLTree) of attendees
            for (auto pIter = copieCocktailsParPersonne.debut(); pIter; ++pIter) {
                presences.inserer(pIter.cle());
            }
        }
    }

    return presences;
}


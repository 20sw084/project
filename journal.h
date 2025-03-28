/*  to be implemented without any signature modification 
 *  and without any new include  nor adding a new 
 *  method / stucture only underprivate
 */
 
#if !defined(__JOURNAL_H__)
#define __JOURNAL_H__

#include <string>
#include "arbremap.h"
#include "date.h"

using namespace std;

class Journal{
  public:
    void                  enregistrerService(const Date& date, const string& nompersonne, const string& nomcocktail);
    ArbreMap<string, int> requeteQui(const Date& debut, const Date& fin, const ArbreAVL<string>& cocktails) const;
    ArbreMap<string, int> requeteQuoi(const Date& debut, const Date& fin, const ArbreAVL<string>& buveurs) const;
    ArbreAVL<string>  	  requetePresences(const Intervalle& i) const;

  private:
    //if we need to something it should be here
    ArbreMap<Date, ArbreMap<string, ArbreAVL< string > > > services;

};

#endif

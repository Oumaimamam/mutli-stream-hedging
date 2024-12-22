#include <cmath>
#include "ConditionalBasket.hpp"



double ConditionalBasket::payoff(const PnlMat* path,double r) const {
   int m = 0;
   
   while (m < dates_->size) {
       // Calcul de la moyenne des actifs
       double basket = 0.0;
       for(int d = 0; d < size_; d++) {
           basket += MGET(path, m, d);
       }
       basket /= size_;
       
       // Calcul du flux à la date m
       double flux = std::max(basket - GET(strikes_, m), 0.0);
       
       // Si le flux est non nul, on retourne le flux et l'indice m
       if (flux > 0) {
            
           return cap(flux, GET(dates_, m), 0,r);
       }
       
       m++;
   }
   
   // Aucun flux non nul trouvé
   return 0.0;
}






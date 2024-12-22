#include <cmath>
#include "ConditionalMax.hpp"


double ConditionalMax::payoff(const PnlMat* path,double r) const  {
   double totalFlux = 0.0;
   bool previousFluxNull = true;

   for(int m = 0; m < dates_->size; m++) {
       // Trouve le max parmi les actifs
       double maxValue = MGET(path, m, 0);
       for(int d = 1; d < size_; d++) {
           maxValue = std::max(maxValue, MGET(path, m, d));
       }
       
       // Calcul du flux
       double flux = std::max(maxValue - GET(strikes_, m), 0.0);
       // Si flux positif et condition précédente OK
       if (flux!=0.0 && previousFluxNull) { 
           previousFluxNull = false;
       }else{
            flux=0.0;
            previousFluxNull = true;
       }
       totalFlux += cap(flux, GET(dates_, m), 0, r);

       
   }
   return totalFlux;
}

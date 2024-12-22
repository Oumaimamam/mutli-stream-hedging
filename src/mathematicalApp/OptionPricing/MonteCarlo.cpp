#include "MonteCarlo.hpp"
#include <cmath>

void MonteCarloPricer::price(const PnlMat* past,PnlMat* path, double t, double& price,double& priceStdv,bool isMonitoringDate) {
    double sum = 0.0;
    double sum_squared = 0.0;

    // Récupérer les dates de paiement depuis l'option
    
    const PnlVect *dates = opt_->getDates();
    for (long i = 0; i < mod_->samples(); ++i) {
        // std::cout <<"hi montecarlo"<< std::endl;
        
        // Simuler le chemin à partir du temps t
        mod_->asset_path(path, past, t,isMonitoringDate);
        // std::cout << "Path matrix (" << path->m << " rows x " << path->n << " cols):" << std::endl;
        
        double payoff = opt_->payoff(path,mod_->r());
        sum += payoff;
        sum_squared += payoff * payoff;
        
    }

   

    // Calcul du prix actualisé
    price = (sum / mod_->samples()) * exp(mod_->r() * t);
    
    // Calcul de l'intervalle de confiance
    double variance = fabs((sum_squared / mod_->samples() - pow(sum / mod_->samples(), 2))) / mod_->samples();
    priceStdv=sqrt(variance) * exp(mod_->r() *t)/sqrt(mod_->samples()); 
    /* ::cout << "HERE\n";
    pnl_mat_print(path);
    std::cout << "HERE\n";
    */
    pnl_mat_free(&path);
}

void MonteCarloPricer::price_at_0(const PnlVect* spots, double& price, double& priceStdv) {
    double sum = 0.0;
    double sum_squared = 0.0;

    // Récupérer les dates de paiement depuis l'option
    const PnlVect* dates = opt_->getDates();


    for (long i = 0; i < mod_->samples(); ++i) {
        PnlMat* path = pnl_mat_create(dates->size+1, opt_->getSize());
        
        // Simuler le chemin complet depuis t=0
        mod_->asset_path_at_0(path, spots);
        
        
        double payoff = opt_->payoff(path,mod_->r());
        sum += payoff;
        sum_squared += payoff * payoff;
        
        pnl_mat_free(&path);
    }

   

    // Calcul du prix actualisé
    price = exp(-mod_->r() * GET(dates,dates->size-1))* sum / mod_->samples() ;
    // Calcul de l'intervalle de confiance
    double variance = (sum_squared / mod_->samples() - pow(sum / mod_->samples(), 2)) / mod_->samples();
    priceStdv=sqrt(variance) * exp(-mod_->r() * GET(dates,dates->size-1))/sqrt(mod_->samples()) ;
    
}

void MonteCarloPricer::delta(const PnlMat* past,double t, PnlVect* deltas, PnlVect* deltas_std,bool isMonitoringDate) {
    int size = opt_->getSize();
    int nSamples = mod_->samples();
    
    PnlVect *sum_squares = pnl_vect_create_from_zero(size);
    const PnlVect* dates = opt_->getDates();
    PnlMat *path = pnl_mat_create(dates->size, size);
    PnlMat *shifted = pnl_mat_create(dates->size, size);
    for(int i = 0; i < nSamples; i++) {
        // Simuler un chemin
         
        int from_idx = 0.0;
        

        mod_->asset_path(path, past,t,isMonitoringDate);
        for (int idx = 0; idx < dates->size; idx++) {
            if (t < GET(dates, idx)) {
                break;
            }
            from_idx++;
        }
        from_idx = from_idx - 1 * isMonitoringDate;
        if (from_idx == -1) {
            from_idx = 0;
        }

        // Pour chaque actif
        for(int d = 0; d < size; d++) {
            // Un seul chemin shifté à la fois
            
            mod_->shift_asset_path(shifted, path, d,true,from_idx);
            double payoff_up = opt_->payoff(shifted,mod_->r());
            mod_->shift_asset_path(shifted, path, d,false,from_idx);  
            double payoff_down = opt_->payoff(shifted,mod_->r());
            double diff = payoff_up - payoff_down;
            
            // Mise à jour des sommes
            LET(deltas, d) = GET(deltas, d) + diff;
            LET(sum_squares, d) = GET(sum_squares, d) + diff * diff;
            
        }
    }
    
    // Calcul final des deltas et écarts-types
    double T = GET(dates, dates->size - 1);
    
    for(int d = 0; d < size; d++) {
        double S_d = MGET(past, past->m - 1, d);  // Spot price actuel
        double fdStep = mod_->fdStep();
        double discount = exp(mod_->r() * t);
        double mean = GET(deltas, d) / (2.0 * fdStep * nSamples);
        double var = fabs(GET(sum_squares, d)/(4.0 * fdStep * fdStep * nSamples) - mean * mean);
        

        // Delta final
        LET(deltas, d) = mean *discount  / S_d;
        
        // Écart-type
        LET(deltas_std, d) = sqrt(var) * discount  / (S_d * sqrt(nSamples)) ;
    }
    
    pnl_vect_free(&sum_squares);
}

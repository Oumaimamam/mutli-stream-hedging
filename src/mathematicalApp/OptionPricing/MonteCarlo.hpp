#pragma once
#include "../Options/base/option.hpp"
#include "../model/BSModel.hpp"


class MonteCarloPricer {
private:
    BSModel* mod_;              // Pointeur vers le modèle Black-Scholes
    Option* opt_;               // Pointeur vers l'option
    

public:
    MonteCarloPricer(
        BSModel* mod, 
        Option* opt 
    ) : mod_(mod), opt_(opt){}

    void price(const PnlMat* past,PnlMat* path,  double t, double& price, double& priceStdv,bool isMonitoringDate);
    void price_at_0(const PnlVect* spots, double& price,double& priceStdv);
    void delta(const PnlMat* past,double t, PnlVect* deltas, PnlVect* deltas_std,bool isMonitoringDate);
};
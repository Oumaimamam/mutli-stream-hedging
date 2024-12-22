#include <fstream>
#include "pricer.hpp"
#include "../OptionPricing/MonteCarlo.hpp"
#include "../Options/base/optionFactory.hpp"
#include "../RandomGeneration/PnlRandomGeneration.hpp"

BlackScholesPricer::BlackScholesPricer(const nlohmann::json &jsonParams) {
    // Parse le JSON en MathParams
    params_ = MathParamsParser::parse(jsonParams);
    
    // Initialise les composants
    rng_ = new PnlRandomGeneration();
    model_ = new BSModel(params_, rng_);
    option_ = OptionFactory::createOption(*params_);
    mc_ = new MonteCarloPricer(model_, option_);
}

BlackScholesPricer::~BlackScholesPricer() {
    delete option_;
}

void BlackScholesPricer::priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate,
                                      double &price, double &priceStdDev,
                                      PnlVect* &deltas, PnlVect* &deltasStdDev) {
    int size = option_->getSize();
    price = 0.0;
    priceStdDev = 0.0;
    deltas = pnl_vect_create(size);
    deltasStdDev = pnl_vect_create(size);
    
    const PnlVect *dates = option_->getDates();
    PnlMat *path = pnl_mat_create(dates->size, option_->getSize());
    mc_->price(past,path, currentDate, price, priceStdDev,isMonitoringDate);
    mc_->delta(past, currentDate, deltas, deltasStdDev,isMonitoringDate);
        
}
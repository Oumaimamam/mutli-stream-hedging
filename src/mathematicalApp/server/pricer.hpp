#pragma once

#include <nlohmann/json.hpp>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "../model/BSModel.hpp"
#include "../OptionPricing/MonteCarlo.hpp"
#include "../Options/base/option.hpp"
#include "../Options/base/optionFactory.hpp"
#include "../RandomGeneration/RandomGeneration.hpp"
#include "../Parsers/MathParamParser.hpp"

class BlackScholesPricer {
private:
    BSModel* model_;
    Option* option_;
    MonteCarloPricer* mc_;
    RandomGeneration* rng_;
    MathParams* params_;

public:
    BlackScholesPricer(const nlohmann::json &jsonParams);
    ~BlackScholesPricer();
    
    void priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, 
                       double &price, double &priceStdDev, 
                       PnlVect* &deltas, PnlVect* &deltasStdDev);
                       
    // Getters for heartbeat
    double getInterestRate() const { return params_->rate(); }
    double getFdStep() const { return params_->fdStep(); }
    int getSampleNb() const { return params_->samples(); }
    
    void print() const { if(params_) params_->print(); }
};
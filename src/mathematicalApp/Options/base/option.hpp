#pragma once
#include <iostream>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "../../Parsers/MathParamParser.hpp"

class Option {
protected:
    PnlVect* strikes_;    
    PnlVect* dates_;      
    int size_;            
    std::string type_;    

public:
    // Constructeur à partir de MathParams
    Option(const MathParams& params) {
        strikes_ = pnl_vect_copy(params.strikes());
        dates_ = pnl_vect_copy(params.dates());
        size_ = params.volatility()->m;
        type_ = params.type();
    }
    
    
    virtual ~Option() {
        pnl_vect_free(&strikes_);
        pnl_vect_free(&dates_);
    }
    
    double cap(double payoff, double t, double T,double r) const {
        double temps = t;
        return payoff * exp(-r * temps);
    }

    virtual double payoff(const PnlMat* path,double r) const = 0;
    
    
    const PnlVect* getStrikes() const { return strikes_; }
    const PnlVect* getDates() const { return dates_; }
    int getSize() const { return size_; }
    std::string type() const { return type_; }
};

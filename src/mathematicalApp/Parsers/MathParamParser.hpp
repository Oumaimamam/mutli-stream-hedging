#pragma once
#include <memory>
#include <iostream>
#include <nlohmann/json.hpp>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "json_reader.hpp"



class MathParams {
private:
    PnlMat* vol_;
    PnlVect* dates_;
    PnlVect* strikes_;
    double r_;
    double fdStep_;
    int samples_;
    std::string type_; 

public:
    MathParams() : vol_(nullptr), dates_(nullptr), strikes_(nullptr), 
                  r_(0.0), fdStep_(0.0), samples_(0) , type_("") {}
    
    ~MathParams() {
        pnl_mat_free(&vol_);
        pnl_vect_free(&dates_);
        pnl_vect_free(&strikes_);
    }

    // Getters
    PnlMat* volatility() const { return vol_; }
    PnlVect* dates() const { return dates_; }
    PnlVect* strikes() const { return strikes_; }
    double rate() const { return r_; }
    double fdStep() const { return fdStep_; }
    int samples() const { return samples_; }
    std::string type() const { return type_; }

    // Setters
    void setVolatility(PnlMat* vol) { vol_ = vol; }
    void setDates(PnlVect* dates) { dates_ = dates; }
    void setStrikes(PnlVect* strikes) { strikes_ = strikes; }
    void setRate(double r) { r_ = r; }
    void setFdStep(double step) { fdStep_ = step; }
    void setSamples(int samples) { samples_ = samples; }
    void setType(const std::string& type) { type_ = type; }

    // Affichage
    void print() const {
        std::cout << "=== Math Parameters ===" << std::endl;
        std::cout << "Interest rate: " << r_ << std::endl;
        std::cout << "Finite diff step: " << fdStep_ << std::endl;
        std::cout << "Number of samples: " << samples_ << std::endl;
        
        std::cout << "\nPayment dates: " << std::endl;
        pnl_vect_print(dates_);
        
        std::cout << "\nStrikes: " << std::endl;
        pnl_vect_print(strikes_);
        
        std::cout << "\nVolatility matrix: " << std::endl;
        pnl_mat_print(vol_);
        
        std::cout << "\nPayoff type: ";

        if (type_== "VanillaCall") {
            std::cout << "Vanilla Call" << std::endl;
        }
        else if (type_ == "ConditionalBasket") {
            std::cout << "Conditional Basket" << std::endl;
        }
        else {
            std::cout << "Conditional Max" << std::endl;
        }
        
    
        
        std::cout << "======================" << std::endl;
    }
};

class MathParamsParser {
public:
    static MathParams* parse(const nlohmann::json& jsonParams);
    
  
};


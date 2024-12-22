#pragma once
#include <iostream>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"
#include "../RandomGeneration/RandomGeneration.hpp"
#include "../Parsers/MathParamParser.hpp"

class BSModel {
private:
    int size_;              
    double r_;
    int SampleNb_;             
    PnlMat* volChol_;      
    PnlVect* paymentDates_; 
    double fdStep_;    
    std::string type_;
    RandomGeneration* generator_;  // Nouveau générateur aléatoire comme attribut

    void simulateOneStep1D(PnlMat* path, int currentIdx, double dt,PnlVect*g) const;
    void simulateOneStepND(PnlMat* path, int currentIdx, double dt,PnlVect*g) const;

public:
    BSModel( MathParams* params, RandomGeneration* generator);  // Constructeur modifié
    ~BSModel();

    // Pour t=0: simulation depuis les spots initiaux
    void asset_path_at_0(PnlMat* path, const PnlVect* spots);
    
    // Pour t>0: simulation avec historique de marché
    void asset_path(PnlMat* path, const PnlMat* past, double t,bool isMonitoringDate);
    
    void shift_asset_path(PnlMat* shiftedPath, const PnlMat* path, int d, bool up,int from_idx);

    // Getters
    int size() const { return size_; }
    double r() const { return r_; }
    double fdStep() const { return fdStep_; }
    int samples() const { return SampleNb_;}
    const PnlMat* volatility() const { return volChol_; }
    const PnlVect* paymentDates() const { return paymentDates_; }
    std::string type() const { return type_; }
};


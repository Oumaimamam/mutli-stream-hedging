#pragma once
#include "../base/option.hpp"



class VanillaCall : public Option {
public:
    // Le constructeur prend toujours un vecteur de dates et strikes
    // mais on sait qu'ils n'auront qu'une seule valeur
    VanillaCall(const MathParams& params) : Option(params) {
        if (size_ != 1 || strikes_->size != 1 || dates_->size != 1) {
            throw std::runtime_error("VanillaCall requires size = 1 and single date/strike");
        }
    }
    
    double payoff(const PnlMat* path,double r) const override;;
};


#pragma once
#include "../base/option.hpp"

class ConditionalBasket : public Option {
public:
   ConditionalBasket(const MathParams& params) : Option(params) {}
   
   double payoff(const PnlMat* path,double r) const override;;
};


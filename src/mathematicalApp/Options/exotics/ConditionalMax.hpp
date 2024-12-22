#pragma once
#include "../base/option.hpp"

class ConditionalMax : public Option {
public:
   ConditionalMax(const MathParams& params) : Option(params) {}
   
   double payoff(const PnlMat* path,double r) const override;;
};



#pragma once
#include "option.hpp"
#include "../vanilla/VanillaCall.hpp"
#include "../exotics/ConditionalBasket.hpp"
#include "../exotics/ConditionalMax.hpp"


class OptionFactory {
public:
    static Option * createOption(const MathParams& params) {
        if (params.type() == "VanillaCall") {
            return new VanillaCall(params);
        }
        else if (params.type() == "ConditionalBasket") {
            return new ConditionalBasket(params);
        }
        else if (params.type() == "ConditionalMax") {
            return new ConditionalMax(params);
        }
        else {
            throw std::runtime_error("Type de payoff non supporté: " + params.type());
        }
    }
};
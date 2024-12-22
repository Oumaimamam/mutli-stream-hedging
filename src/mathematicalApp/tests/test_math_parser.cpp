#include <iostream>
#include "Parsers/MathParamParser.hpp"

int main() {
    try {
        auto params = MathParamsParser::parse("../../../tests/Basket_5_3_1/basket_5_3_1_math_param.json");
        params->print();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
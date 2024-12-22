#include <fstream>
#include "MathParamParser.hpp"


MathParams* MathParamsParser::parse(const nlohmann::json& jsonParams) {
    auto params = new MathParams();
    
    try {
        // Utilise le JsonHelper existant via from_json
        PnlMat* vol = nullptr;
        from_json(jsonParams.at("VolCholeskyLines"), vol);
        params->setVolatility(vol);

        PnlVect* dates = nullptr;
        from_json(jsonParams.at("MathPaymentDates"), dates);
        params->setDates(dates);

        PnlVect* strikes = nullptr;
        from_json(jsonParams.at("Strikes"), strikes);
        params->setStrikes(strikes);

        params->setRate(jsonParams.at("DomesticInterestRate").get<double>());
        params->setFdStep(jsonParams.at("RelativeFiniteDifferenceStep").get<double>());
        params->setSamples(jsonParams.at("SampleNb").get<int>());
        params->setType(jsonParams.at("PayoffType").get<std::string>());
    
        return params;
    } 
    catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
}



#include <iostream>
#include <fstream>
#include "Parsers/MathParamParser.hpp"
#include "Parsers/MarketDataParser.hpp"
#include "RandomGeneration/PnlRandomGeneration.hpp"
#include "model/BSModel.hpp"
#include "OptionPricing/MonteCarlo.hpp"
#include "Options/base/option.hpp"
#include "Options/base/optionFactory.hpp"
#include "Options/vanilla/VanillaCall.hpp"
#include "Options/exotics/ConditionalBasket.hpp"
#include "Options/exotics/ConditionalMax.hpp"

int main() {
    try {
        // 1. Parser les paramètres mathématiques
        // auto mathParams = MathParamsParser::parse("../../../tests/Basket_8_4_3/basket_8_4_3_math_param.json");
        // auto mathParams = MathParamsParser::parse("../../../tests/Basket_5_3_1/basket_5_3_1_math_param.json");
        // auto mathParams = MathParamsParser::parse("../../../tests/Max_5_3_1/max_5_3_1_math_param.json");
        auto mathParams = MathParamsParser::parse("../../../tests/Max_8_4_3/max_8_4_3_math_param.json");
        mathParams->print();

        // 2. Créer le générateur aléatoire
        PnlRandomGeneration randomGenerator;

        // 3. Créer l'option depuis les paramètres
        auto option = OptionFactory::createOption(*mathParams);
        std::cout << "Created option of type" <<  std::endl;
        // 4. Créer le modèle Black-Scholes
        BSModel bsModel(*mathParams, &randomGenerator);
        std::cout << "Created BS" << std::endl;
        // 5. Créer le pricer Monte Carlo
        MonteCarloPricer pricer(&bsModel, option.get());
        std::cout << "Created pricer" << std::endl;
        // 6. Construire le vecteur des spots initiaux multi-actifs
        PnlVect* spots = pnl_vect_create(option->getSize());
        // double spotValues[] = {13.0, 15.0, 17.0, 17.0, 14.0};
        // double spotValues[] = {17.0, 20.0, 15.0, 14.0, 13.0};
        // double spotValues[] = {12.0, 15.0, 11.0, 15.0, 16.0,10.0,11.0,11.0};
        // double spotValues[] = {12.0, 10.0, 16.0, 15.0, 14.0,18.0,19.0,17.0};
        // for (int i = 0; i < option->getSize(); i++) {
        //     pnl_vect_set(spots, i, spotValues[i]);
        // }
        // 7. Créer la matrice de passé avec les spots initiaux
        auto marketData = MarketDataParser::parse("../../../tests/Max_8_4_3/Max843.json");
        PnlMat* past = pnl_mat_copy(marketData->prices());
        PnlVect* past_dates = pnl_vect_copy(marketData->dates());
        

        // // 9. Calculer le prix à t=0
        // double price_t0, ic_t0, priceStdv_0;
        // pricer.price_at_0(spots, price_t0, ic_t0,priceStdv_0);
        // std::cout << "Prix à t=0 : " << price_t0 << " +/- " << ic_t0 <<"  stdv : "<< priceStdv_0<<std::endl;

        // 10. Calculer le prix à t=0.2619047619
        double t = 0.2619047619;
        double prix_t, ic_t,priceStdv_t;
        pricer.price(past, past_dates, t, prix_t, ic_t,priceStdv_t);
        std::cout << "Prix à t=" << t << " : " << prix_t << " +/- " << ic_t <<"  stdv : "<< priceStdv_t<<std::endl;

        // 11. Calculer les deltas
        PnlVect* deltas = pnl_vect_create(option->getSize());
        PnlVect* deltas_std = pnl_vect_create(option->getSize());
        pricer.delta(past, past_dates, t, deltas, deltas_std);

        std::cout << "\nDeltas :" << std::endl;
        pnl_vect_print(deltas);
        std::cout << "\nEcart-types des deltas :" << std::endl;
        pnl_vect_print(deltas_std);

        pnl_mat_print(past);
        // std::cout << past->n << std::endl;
        // std::cout << past->m  << std::endl;
        // Libération de la mémoire
        pnl_vect_free(&spots);
        pnl_mat_free(&past);
        pnl_vect_free(&past_dates);
        pnl_vect_free(&deltas);
        pnl_vect_free(&deltas_std);
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
#include <cmath>
#include "VanillaCall.hpp"



double VanillaCall::payoff(const PnlMat* path,double r) const  {
    double S_T = MGET(path, path->m - 1, 0); 
    double K = GET(strikes_, 0);               
    double flux = std::max(S_T - K, 0.0);
    return flux;
}


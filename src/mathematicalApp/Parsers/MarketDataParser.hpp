#pragma once
#include <memory>
#include <iostream>
#include <nlohmann/json.hpp>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "../Parsers/json_reader.hpp"



class MarketData {
private:
    PnlMat* prices_;
    PnlVect* dates_;

public:
    MarketData() : prices_(nullptr), dates_(nullptr) {}
    
    ~MarketData();

    // Getters
    PnlMat* prices() const;
    PnlVect* dates() const;

    // Setters
    void setPrices(PnlMat* prices);
    void setDates(PnlVect* dates);

    // Affichage
    void print() const;
};

class MarketDataParser {
public:
    static std::unique_ptr<MarketData> parse(const std::string& filename);
};

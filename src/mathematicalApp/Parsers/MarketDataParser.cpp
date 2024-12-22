
#include <fstream>
#include "Parsers/MarketDataParser.hpp"



MarketData::~MarketData() {
    pnl_mat_free(&prices_);
    pnl_vect_free(&dates_);
}

PnlMat* MarketData::prices() const {
    return prices_;
}

PnlVect* MarketData::dates() const {
    return dates_;
}

void MarketData::setPrices(PnlMat* prices) {
    prices_ = prices;
}

void MarketData::setDates(PnlVect* dates) {
    dates_ = dates;
}

void MarketData::print() const {
    std::cout << "=== Market Data ===" << std::endl;
    std::cout << "Prices matrix: " << std::endl;
    pnl_mat_print(prices_);
    
    std::cout << "\nDates: " << std::endl;
    pnl_vect_print(dates_);
    std::cout << "=================" << std::endl;
}

std::unique_ptr<MarketData> MarketDataParser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    nlohmann::json json = nlohmann::json::parse(file);
    auto data = std::make_unique<MarketData>();
    
    try {
        PnlMat* prices = nullptr;
        from_json(json.at("prices"), prices);
        data->setPrices(prices);

        PnlVect* dates = nullptr;
        from_json(json.at("mathDates"), dates);
        data->setDates(dates);
    
        return data;
    } 
    catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
}


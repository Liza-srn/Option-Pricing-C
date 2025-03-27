#include "domain/Option.hpp"
#include <stdexcept> // Pour std::invalid_argument

// Constructeur avec validation des paramètres
Option::Option(double spotPrice, double strikePrice, double riskFreeRate, double volatility, double timeToMaturity, const std::string& type)
    : spotPrice_(spotPrice), strikePrice_(strikePrice), riskFreeRate_(riskFreeRate), volatility_(volatility), timeToMaturity_(timeToMaturity), type_(type) {
    // Validation des paramètres
    if (spotPrice <= 0 || strikePrice <= 0 || volatility <= 0 || timeToMaturity <= 0) {
        throw std::invalid_argument("Invalid option parameters: spotPrice, strikePrice, volatility, and timeToMaturity must be positive");
    }
    if (type != "call" && type != "put") {
        throw std::invalid_argument("Invalid option type: must be 'call' or 'put'");
    }
}

// Getter pour le prix actuel de l'actif sous-jacent
double Option::getSpotPrice() const {
    return spotPrice_;
}

// Getter pour le prix d'exercice de l'option
double Option::getStrikePrice() const {
    return strikePrice_;
}

// Getter pour le taux d'intérêt sans risque
double Option::getRiskFreeRate() const {
    return riskFreeRate_;
}

// Getter pour la volatilité de l'actif sous-jacent
double Option::getVolatility() const {
    return volatility_;
}

// Getter pour le temps restant jusqu'à l'échéance
double Option::getTimeToMaturity() const {
    return timeToMaturity_;
}

// Getter pour le type d'option
std::string Option::getType() const {
    return type_;
}
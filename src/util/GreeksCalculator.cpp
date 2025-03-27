#include "util/GreeksCalculator.hpp"
#include <iostream>
#include <cmath>

/**
 * @brief Calcule la Delta par différences finies centrées.
 */
template<typename Model>
double GreeksCalculator::calculateDelta(const Model& model, const Option& option, double deltaS) {
    Option optionUp  (option.getSpotPrice() + deltaS,
                      option.getStrikePrice(),
                      option.getRiskFreeRate(),
                      option.getVolatility(),
                      option.getTimeToMaturity(),
                      option.getType());

    Option optionDown(option.getSpotPrice() - deltaS,
                      option.getStrikePrice(),
                      option.getRiskFreeRate(),
                      option.getVolatility(),
                      option.getTimeToMaturity(),
                      option.getType());
    
    double priceUp   = model.calculatePrice(optionUp);
    double priceDown = model.calculatePrice(optionDown);

    return (priceUp - priceDown) / (2.0 * deltaS);
}

/**
 * @brief Calcule la Gamma par différences finies centrées.
 */
template<typename Model>
double GreeksCalculator::calculateGamma(const Model& model, const Option& option, double deltaS) {
    Option optionUp  (option.getSpotPrice() + deltaS,
                      option.getStrikePrice(),
                      option.getRiskFreeRate(),
                      option.getVolatility(),
                      option.getTimeToMaturity(),
                      option.getType());

    Option optionDown(option.getSpotPrice() - deltaS,
                      option.getStrikePrice(),
                      option.getRiskFreeRate(),
                      option.getVolatility(),
                      option.getTimeToMaturity(),
                      option.getType());

    double priceUp     = model.calculatePrice(optionUp);
    double priceCenter = model.calculatePrice(option);
    double priceDown   = model.calculatePrice(optionDown);

    return (priceUp - 2.0 * priceCenter + priceDown) / (deltaS * deltaS);
}

/**
 * @brief Calcule la Vega par différences finies sur la volatilité.
 */
template<typename Model>
double GreeksCalculator::calculateVega(const Model& model, const Option& option, double deltaSigma) {
    Option optionUp  (option.getSpotPrice(),
                      option.getStrikePrice(),
                      option.getRiskFreeRate(),
                      option.getVolatility() + deltaSigma,
                      option.getTimeToMaturity(),
                      option.getType());

    Option optionDown(option.getSpotPrice(),
                      option.getStrikePrice(),
                      option.getRiskFreeRate(),
                      option.getVolatility() - deltaSigma,
                      option.getTimeToMaturity(),
                      option.getType());
    
    double priceUp   = model.calculatePrice(optionUp);
    double priceDown = model.calculatePrice(optionDown);

    return (priceUp - priceDown) / (2.0 * deltaSigma);
}

/**
 * @brief Calcule la Theta par différences finies sur le temps.
 *        Convention : Theta = - (P(T-Δt) - P(T)) / Δt
 */
template<typename Model>
double GreeksCalculator::calculateTheta(const Model& model, const Option& option, double deltaT) {
    if (option.getTimeToMaturity() <= deltaT) {
        return 0.0;
    }
    
    Option optionNext(option.getSpotPrice(),
                      option.getStrikePrice(),
                      option.getRiskFreeRate(),
                      option.getVolatility(),
                      option.getTimeToMaturity() - deltaT,
                      option.getType());
    
    double priceNext   = model.calculatePrice(optionNext);
    double priceCenter = model.calculatePrice(option);

    return -(priceNext - priceCenter) / deltaT;
}

/**
 * @brief Calcule la Rho par différences finies sur le taux d'intérêt.
 */
template<typename Model>
double GreeksCalculator::calculateRho(const Model& model, const Option& option, double deltaR) {
    Option optionUp  (option.getSpotPrice(),
                      option.getStrikePrice(),
                      option.getRiskFreeRate() + deltaR,
                      option.getVolatility(),
                      option.getTimeToMaturity(),
                      option.getType());

    Option optionDown(option.getSpotPrice(),
                      option.getStrikePrice(),
                      option.getRiskFreeRate() - deltaR,
                      option.getVolatility(),
                      option.getTimeToMaturity(),
                      option.getType());
    
    double priceUp   = model.calculatePrice(optionUp);
    double priceDown = model.calculatePrice(optionDown);

    return (priceUp - priceDown) / (2.0 * deltaR);
}

// ------------------------------------------
// Instanciations explicites (BlackScholesModel)
// ------------------------------------------
template double GreeksCalculator::calculateDelta<BlackScholesModel>(
    const BlackScholesModel&, const Option&, double);
template double GreeksCalculator::calculateGamma<BlackScholesModel>(
    const BlackScholesModel&, const Option&, double);
template double GreeksCalculator::calculateVega<BlackScholesModel>(
    const BlackScholesModel&, const Option&, double);
template double GreeksCalculator::calculateTheta<BlackScholesModel>(
    const BlackScholesModel&, const Option&, double);
template double GreeksCalculator::calculateRho<BlackScholesModel>(
    const BlackScholesModel&, const Option&, double);

// ------------------------------------------
// Instanciations explicites (FiniteDifferenceModel)
// ------------------------------------------
template double GreeksCalculator::calculateDelta<FiniteDifferenceModel>(
    const FiniteDifferenceModel&, const Option&, double);
template double GreeksCalculator::calculateGamma<FiniteDifferenceModel>(
    const FiniteDifferenceModel&, const Option&, double);
template double GreeksCalculator::calculateVega<FiniteDifferenceModel>(
    const FiniteDifferenceModel&, const Option&, double);
template double GreeksCalculator::calculateTheta<FiniteDifferenceModel>(
    const FiniteDifferenceModel&, const Option&, double);
template double GreeksCalculator::calculateRho<FiniteDifferenceModel>(
    const FiniteDifferenceModel&, const Option&, double);

// ------------------------------------------
// Instanciations explicites (AmericanOptionPricer)
// ------------------------------------------
template double GreeksCalculator::calculateDelta<AmericanOptionPricer>(
    const AmericanOptionPricer&, const Option&, double);
template double GreeksCalculator::calculateGamma<AmericanOptionPricer>(
    const AmericanOptionPricer&, const Option&, double);
template double GreeksCalculator::calculateVega<AmericanOptionPricer>(
    const AmericanOptionPricer&, const Option&, double);
template double GreeksCalculator::calculateTheta<AmericanOptionPricer>(
    const AmericanOptionPricer&, const Option&, double);
template double GreeksCalculator::calculateRho<AmericanOptionPricer>(
    const AmericanOptionPricer&, const Option&, double);

// ------------------------------------------
// Méthodes publiques : Black-Scholes
// ------------------------------------------
double GreeksCalculator::delta(const BlackScholesModel& model, const Option& option) {
    double deltaS = option.getSpotPrice() * 0.01;
    return calculateDelta(model, option, deltaS);
}

double GreeksCalculator::gamma(const BlackScholesModel& model, const Option& option) {
    double deltaS = 0.01;
    return calculateGamma(model, option, deltaS);
}

double GreeksCalculator::vega(const BlackScholesModel& model, const Option& option) {
    double deltaSigma = 0.0001;
    return calculateVega(model, option, deltaSigma);
}

double GreeksCalculator::theta(const BlackScholesModel& model, const Option& option) {
    double deltaT = 1.0 / 365.0;
    return calculateTheta(model, option, deltaT);
}

double GreeksCalculator::rho(const BlackScholesModel& model, const Option& option) {
    double deltaR = 0.0001;
    return calculateRho(model, option, deltaR);
}

// ------------------------------------------
// Méthodes publiques : FiniteDifferenceModel
// ------------------------------------------
double GreeksCalculator::delta(const FiniteDifferenceModel& model, const Option& option) {
    double S_max = 2.0 * option.getStrikePrice();
    int M = model.getAssetSteps();
    double ds = S_max / M;
    return calculateDelta(model, option, ds);
}

double GreeksCalculator::gamma(const FiniteDifferenceModel& model, const Option& option) {
    double S_max = 2.0 * option.getStrikePrice();
    int M = model.getAssetSteps();
    double ds = S_max / M;
    return calculateGamma(model, option, ds);
}

double GreeksCalculator::vega(const FiniteDifferenceModel& model, const Option& option) {
    double deltaSigma = 0.0001;
    return calculateVega(model, option, deltaSigma);
}

double GreeksCalculator::theta(const FiniteDifferenceModel& model, const Option& option) {
    double deltaT = 1.0 / 365.0;
    return calculateTheta(model, option, deltaT);
}

double GreeksCalculator::rho(const FiniteDifferenceModel& model, const Option& option) {
    double deltaR = 0.0001;
    return calculateRho(model, option, deltaR);
}

// ------------------------------------------
// Méthodes publiques : AmericanOptionPricer
// ------------------------------------------
double GreeksCalculator::delta(const AmericanOptionPricer& pricer, const Option& option) {
    double deltaS = option.getSpotPrice() * 0.01;
    return calculateDelta(pricer, option, deltaS);
}

double GreeksCalculator::gamma(const AmericanOptionPricer& pricer, const Option& option) {
    double deltaS = 0.01;
    return calculateGamma(pricer, option, deltaS);
}

double GreeksCalculator::vega(const AmericanOptionPricer& pricer, const Option& option) {
    double deltaSigma = 0.0001;
    return calculateVega(pricer, option, deltaSigma);
}

double GreeksCalculator::theta(const AmericanOptionPricer& pricer, const Option& option) {
    double deltaT = 1.0 / 365.0;
    return calculateTheta(pricer, option, deltaT);
}

double GreeksCalculator::rho(const AmericanOptionPricer& pricer, const Option& option) {
    double deltaR = 0.0001;
    return calculateRho(pricer, option, deltaR);
}
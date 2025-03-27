#ifndef GREEKS_CALCULATOR_HPP
#define GREEKS_CALCULATOR_HPP

#include "domain/Option.hpp"
#include "models/BlackScholesModel.hpp"
#include "models/FiniteDifferenceModel.hpp"
#include "models/AmericanOptionPricer.hpp" // Inclure AmericanOptionPricer

/**
 * @class GreeksCalculator
 * @brief Classe statique qui propose des méthodes pour calculer 
 *        Delta, Gamma, Vega, Theta, Rho par différences finies.
 */
class GreeksCalculator {
public:
    // Méthodes templates internes (calculateXXX)
    template<typename Model>
    static double calculateDelta(const Model& model, const Option& option, double deltaS);

    template<typename Model>
    static double calculateGamma(const Model& model, const Option& option, double deltaS);

    template<typename Model>
    static double calculateVega(const Model& model, const Option& option, double deltaSigma);

    template<typename Model>
    static double calculateTheta(const Model& model, const Option& option, double deltaT);

    template<typename Model>
    static double calculateRho(const Model& model, const Option& option, double deltaR);

public:
    // Méthodes publiques pour Black-Scholes
    static double delta(const BlackScholesModel& model, const Option& option);
    static double gamma(const BlackScholesModel& model, const Option& option);
    static double vega(const BlackScholesModel& model, const Option& option);
    static double theta(const BlackScholesModel& model, const Option& option);
    static double rho(const BlackScholesModel& model, const Option& option);

    // Méthodes publiques pour FiniteDifferenceModel
    static double delta(const FiniteDifferenceModel& model, const Option& option);
    static double gamma(const FiniteDifferenceModel& model, const Option& option);
    static double vega(const FiniteDifferenceModel& model, const Option& option);
    static double theta(const FiniteDifferenceModel& model, const Option& option);
    static double rho(const FiniteDifferenceModel& model, const Option& option);

    // Méthodes publiques pour AmericanOptionPricer
    static double delta(const AmericanOptionPricer& pricer, const Option& option);
    static double gamma(const AmericanOptionPricer& pricer, const Option& option);
    static double vega(const AmericanOptionPricer& pricer, const Option& option);
    static double theta(const AmericanOptionPricer& pricer, const Option& option);
    static double rho(const AmericanOptionPricer& pricer, const Option& option);
};

#endif // GREEKS_CALCULATOR_HPP
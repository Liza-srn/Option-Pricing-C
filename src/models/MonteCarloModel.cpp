#include "models/MonteCarloModel.hpp"
#include <omp.h>
#include <cmath>
#include <random>
#include <algorithm>

MonteCarloModel::MonteCarloModel(int numSimulations) : numSimulations_(numSimulations) {}

// Slower implementation 
// double MonteCarloModel::calculatePrice(const Option& option) const {
//     std::default_random_engine generator;
//     std::normal_distribution<double> distribution(0.0, 1.0);

//     double S = option.getSpotPrice();
//     double K = option.getStrikePrice();
//     double T = option.getTimeToMaturity();
//     double r = option.getRiskFreeRate();
//     double sigma = option.getVolatility();

//     double sumPayoff = 0.0;
//     for (int i = 0; i < numSimulations_; ++i) {
//         double Z = distribution(generator); // Random draw from a standard normal distribution
//         double St = S * exp((r - 0.5 * sigma * sigma) * T + sigma * sqrt(T) * Z); // Simulated asset price at maturity
//         double payoff = std::max(St - K, 0.0); // Payoff for a call option
//         sumPayoff += payoff;
//     }

//     double optionPrice = exp(-r * T) * (sumPayoff / numSimulations_); // Discounted average payoff
//     return optionPrice;
// }


double MonteCarloModel::calculatePrice(const Option& option) const {
    // Paramètres de l'option
    double S = option.getSpotPrice();
    double K = option.getStrikePrice();
    double T = option.getTimeToMaturity();
    double r = option.getRiskFreeRate();
    double sigma = option.getVolatility();
    std::string optionType = option.getType(); // Récupération du type d'option (call/put)

    double sumPayoff = 0.0;

    // Pré-calcul des termes constants
    double drift = exp((r - 0.5 * sigma * sigma) * T);
    double vol = sigma * sqrt(T);

    // Générateur aléatoire par thread
    #pragma omp parallel reduction(+:sumPayoff)
    {
        std::default_random_engine generator(omp_get_thread_num()); // Un générateur par thread
        std::normal_distribution<double> distribution(0.0, 1.0);

        #pragma omp for
        for (int i = 0; i < numSimulations_; ++i) {
            double Z = distribution(generator); // Tirage aléatoire
            double St = S * drift * exp(vol * Z); // Prix simulé à maturité

            // Calcul du paiement en fonction du type d'option
            double payoff = (optionType == "call") ? std::max(St - K, 0.0) : std::max(K - St, 0.0);
            sumPayoff += payoff;
        }
    }

    // Moyenne des paiements et actualisation
    double optionPrice = exp(-r * T) * (sumPayoff / numSimulations_);
    return optionPrice;
}

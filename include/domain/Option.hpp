#ifndef OPTION_HPP
#define OPTION_HPP

#include <string>
#include <stdexcept> // Pour std::invalid_argument

class Option {
public:
    // Constructeur avec validation des paramètres
    Option(double spotPrice, double strikePrice, double riskFreeRate, double volatility, double timeToMaturity, const std::string& type);

    // Destructeur virtuel par défaut
    virtual ~Option() = default;

    // Getters pour les attributs de l'option
    double getSpotPrice() const;
    double getStrikePrice() const;
    double getRiskFreeRate() const;
    double getVolatility() const;
    double getTimeToMaturity() const;
    std::string getType() const;

protected:
    // Attributs protégés pour permettre l'héritage
    double spotPrice_;         // Prix actuel de l'actif sous-jacent
    double strikePrice_;       // Prix d'exercice de l'option
    double riskFreeRate_;      // Taux d'intérêt sans risque
    double volatility_;        // Volatilité de l'actif sous-jacent
    double timeToMaturity_;    // Temps restant jusqu'à l'échéance (en années)
    std::string type_;         // Type d'option : "call" ou "put"
};

#endif // OPTION_HPP
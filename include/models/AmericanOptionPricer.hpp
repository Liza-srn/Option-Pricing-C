#ifndef AMERICAN_OPTION_PRICER_HPP
#define AMERICAN_OPTION_PRICER_HPP

#include <string>
#include "domain/Option.hpp" // Inclure Option.hpp pour utiliser la classe Option

class AmericanOptionPricer {
public:
    AmericanOptionPricer(double S_now, double K, double T, double Vol, double r, int I, double delta_t, std::string optionType);

    // Méthode pour obtenir le prix de l'option
    double Value() const;

    // Méthode pour calculer le prix de l'option (compatible avec les templates)
    double calculatePrice(const Option& option) const;

    // Destructeur
    ~AmericanOptionPricer();

private:
    // Méthode interne pour calculer le prix de l'option américaine
    double calculatePriceImpl() const;

    // Attributs
    double S_now, K, T, Vol, r;
    int I;
    double delta_t;
    std::string optionType;
    double Price; // Ajout de l'attribut Price
};

#endif // AMERICAN_OPTION_PRICER_HPP
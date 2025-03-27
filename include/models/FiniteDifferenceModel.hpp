#ifndef FINITE_DIFFERENCE_MODEL_HPP
#define FINITE_DIFFERENCE_MODEL_HPP

#include "OptionPricingModel.hpp"
#include "domain/Option.hpp"

class FiniteDifferenceModel : public OptionPricingModel {
public:
    // Constructeur avec des valeurs par défaut pour les pas de temps et d'actif
    FiniteDifferenceModel(int timeSteps = 100, int assetSteps = 100);

    // Implémentation de la méthode calculatePrice pour les options européennes
    virtual double calculatePrice(const Option& option) const override;

    // Getters pour les pas de temps et d'actif
    int getAssetSteps() const;
    int getTimeSteps() const;

private:
    int timeSteps_;   // Nombre de pas de temps
    int assetSteps_;  // Nombre de pas pour le prix de l'actif
};

#endif // FINITE_DIFFERENCE_MODEL_HPP
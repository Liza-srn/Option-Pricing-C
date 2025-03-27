#ifndef PRICING_MODEL_FACTORY_HPP
#define PRICING_MODEL_FACTORY_HPP

#include "models/OptionPricingModel.hpp"
#include <memory>
#include <string>

class PricingModelFactory {
public:
    // Méthode statique pour créer un modèle de pricing en fonction du nom
    static std::unique_ptr<OptionPricingModel> createModel(const std::string& modelName);

private:
    // Constructeur privé pour empêcher l'instanciation de la factory
    PricingModelFactory() = delete;
};

#endif // PRICING_MODEL_FACTORY_HPP
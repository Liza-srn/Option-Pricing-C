#include "Factory/PricingModelFactory.hpp"
#include "models/BlackScholesModel.hpp"
#include "models/BinomialModel.hpp"
#include "models/MonteCarloModel.hpp"
#include "models/FiniteDifferenceModel.hpp"
#include <stdexcept>
#include <functional>
#include <unordered_map> // Pour une gestion plus efficace des modèles

// Méthode pour créer un modèle de pricing en fonction du nom
std::unique_ptr<OptionPricingModel> PricingModelFactory::createModel(const std::string& modelName) {
    // Utilisation d'une map pour associer les noms de modèles à leurs constructeurs
    static const std::unordered_map<std::string, std::function<std::unique_ptr<OptionPricingModel>()>> modelMap = {
        {"BlackScholes", []() { return std::make_unique<BlackScholesModel>(); }},
        {"Binomial", []() { return std::make_unique<BinomialModel>(); }},
        {"MonteCarlo", []() { return std::make_unique<MonteCarloModel>(); }},
        {"FiniteDifference", []() { return std::make_unique<FiniteDifferenceModel>(); }}
    };

    // Recherche du modèle dans la map
    auto it = modelMap.find(modelName);
    if (it != modelMap.end()) {
        return it->second(); // Retourne une instance du modèle
    }

    // Si le modèle n'est pas trouvé, lance une exception
    throw std::runtime_error("Unknown model name: " + modelName);
}
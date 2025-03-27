#include "Factory/PricingModelFactory.hpp"
#include "domain/Option.hpp"
#include "models/BlackScholesModel.hpp"
#include "models/FiniteDifferenceModel.hpp"
#include "util/GreeksCalculator.hpp"
#include "util/OptionDataExporter.hpp"
#include "models/AmericanOptionPricer.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <cstdlib> // Pour system("mkdir -p ...") sous Linux/Mac

/**
 * @brief Vérifie que l'optionType est "call" ou "put".
 */
bool isOptionTypeValid(const std::string& optionType) {
    return optionType == "call" || optionType == "put";
}

/**
 * @brief Fonction principale pour calculer le prix des options et les Greeks.
 */
int main(int argc, char* argv[]) {
    // Paramètres par défaut
    double spotPrice = 100.0;
    double strikePrice = 100.0;
    double riskFreeRate = 0.05;
    double volatility = 0.2;
    double timeToMaturity = 1.0;
    std::string optionType = "put";

    // ====================================================================
    // Récupération des paramètres en ligne de commande (si fournis)
    // ====================================================================
    if (argc > 6) {
        try {
            spotPrice = std::stod(argv[1]);
            strikePrice = std::stod(argv[2]);
            riskFreeRate = std::stod(argv[3]);
            volatility = std::stod(argv[4]);
            timeToMaturity = std::stod(argv[5]);
            optionType = argv[6];
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors de l'analyse des arguments : " << e.what() << std::endl;
            return 1;
        }
    }

    // Validation des paramètres
    if (spotPrice <= 0 || strikePrice <= 0 || volatility <= 0 ||
        timeToMaturity <= 0 || !isOptionTypeValid(optionType)) {
        std::cerr << "Paramètres invalides : "
                  << "valeurs positives requises et type d'option valide ('call' ou 'put')."
                  << std::endl;
        return 1;
    }

    // Création de l'Option
    Option option(spotPrice, strikePrice, riskFreeRate, volatility, timeToMaturity, optionType);

    // Crée un répertoire pour l'export éventuel des résultats
    system("mkdir -p output");

    // ====================================================================
    // Calcul du prix d'une option américaine avec Crank-Nicolson
    // ====================================================================
    std::cout << "Prix de l'option américaine (Crank-Nicolson):" << std::endl;
    try {
        // Paramètres spécifiques au modèle
        int stockSteps = 120;       // Nombre de divisions sur l'axe du prix du sous-jacent
        double deltaT = 0.005;      // Taille du pas de temps

        // Instanciation du pricer américain
        AmericanOptionPricer americanPricer(spotPrice, strikePrice, timeToMaturity, volatility, riskFreeRate, stockSteps, deltaT, optionType);

        // Calcul du prix
        double americanPrice = americanPricer.Value();
        std::cout << "  Prix (Crank-Nicolson): " << americanPrice << "\n" << std::endl;

        // Export des données de l'option américaine
        std::string americanCsvFilename = "output/american_option_data.csv";
        OptionDataExporter::exportToCSV(americanPricer, option, americanCsvFilename);
        std::cout << "Données de l'option américaine exportées vers : " << americanCsvFilename << "\n" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du calcul de l'option américaine : " << e.what() << std::endl;
    }

    // ====================================================================
    // Calcul des Greeks pour l'option américaine
    // ====================================================================
    std::cout << "Greeks pour l'option américaine (Crank-Nicolson):" << std::endl;
    try {
        // Paramètres spécifiques au modèle
        int stockSteps = 120;       // Nombre de divisions sur l'axe du prix du sous-jacent
        double deltaT = 0.005;      // Taille du pas de temps

        // Instanciation du pricer américain
        AmericanOptionPricer americanPricer(spotPrice, strikePrice, timeToMaturity, volatility, riskFreeRate, stockSteps, deltaT, optionType);

        // Calcul des Greeks
        auto delta_am = GreeksCalculator::delta(americanPricer, option);
        auto gamma_am = GreeksCalculator::gamma(americanPricer, option);
        auto theta_am = GreeksCalculator::theta(americanPricer, option);
        auto rho_am   = GreeksCalculator::rho(americanPricer, option);
        auto vega_am  = GreeksCalculator::vega(americanPricer, option);

        std::cout << "  Delta: " << delta_am
                << "\n  Gamma: " << gamma_am
                << "\n  Theta: " << theta_am
                << "\n  Rho:   " << rho_am
                << "\n  Vega:  " << vega_am
                << "\n-----------------------------------------\n" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du calcul des Greeks pour l'option américaine : " << e.what() << std::endl;
    }

    // ====================================================================
    // Calcul et affichage des Greeks via le modèle Black-Scholes
    // ====================================================================
    try {
        BlackScholesModel bsModel;
        std::cout << "Black-Scholes Greeks:" << std::endl;
        auto delta_bs = GreeksCalculator::delta(bsModel, option);
        auto gamma_bs = GreeksCalculator::gamma(bsModel, option);
        auto theta_bs = GreeksCalculator::theta(bsModel, option);
        auto rho_bs = GreeksCalculator::rho(bsModel, option);
        auto vega_bs = GreeksCalculator::vega(bsModel, option);

        std::cout << "  Delta: " << delta_bs
                  << "\n  Gamma: " << gamma_bs
                  << "\n  Theta: " << theta_bs
                  << "\n  Rho:   " << rho_bs
                  << "\n  Vega:  " << vega_bs
                  << "\n-----------------------------------------\n" << std::endl;

        // Export des données de Black-Scholes
        std::string bsCsvFilename = "output/black_scholes_data.csv";
        OptionDataExporter::exportToCSV(bsModel, option, bsCsvFilename);
        std::cout << "Données de Black-Scholes exportées vers : " << bsCsvFilename << "\n" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du calcul des Greeks avec Black-Scholes : " << e.what() << std::endl;
    }

    // ====================================================================
    // Boucle sur différents modèles : BlackScholes, MonteCarlo, FiniteDifference
    // ====================================================================
    for (const std::string modelName : {"BlackScholes", "MonteCarlo", "FiniteDifference"}) {
        try {
            auto model = PricingModelFactory::createModel(modelName);
            if (!model) {
                std::cerr << "Impossible de créer le modèle \"" << modelName << "\"." << std::endl;
                continue;
            }

            double price = model->calculatePrice(option);
            std::cout << std::setw(20) << modelName << ":\t " << price << std::endl;

            if (modelName == "FiniteDifference") {
                auto* fdModel = dynamic_cast<FiniteDifferenceModel*>(model.get());
                if (fdModel) {
                    // Calcul des Greeks pour le modèle de différences finies
                    std::cout << "\nCrank-Nicolson Greeks:" << std::endl;
                    auto delta_fd = GreeksCalculator::delta(*fdModel, option);
                    auto gamma_fd = GreeksCalculator::gamma(*fdModel, option);
                    auto theta_fd = GreeksCalculator::theta(*fdModel, option);
                    auto rho_fd = GreeksCalculator::rho(*fdModel, option);
                    auto vega_fd = GreeksCalculator::vega(*fdModel, option);

                    std::cout << "  Delta: " << delta_fd
                              << "\n  Gamma: " << gamma_fd
                              << "\n  Theta: " << theta_fd
                              << "\n  Rho:   " << rho_fd
                              << "\n  Vega:  " << vega_fd << std::endl;

                    // Export des données de différences finies
                    std::string fdCsvFilename = "output/finite_difference_data.csv";
                    OptionDataExporter::exportToCSV(*fdModel, option, fdCsvFilename);
                    std::cout << "Données de différences finies exportées vers : " << fdCsvFilename << "\n" << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du calcul avec le modèle \"" << modelName << "\": " << e.what() << std::endl;
        }
    }

    return 0;
}
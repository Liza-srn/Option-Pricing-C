#include "util/OptionDataExporter.hpp"
#include <iostream>
#include <iomanip>

// Export for FiniteDifferenceModel
void OptionDataExporter::exportToCSV(const FiniteDifferenceModel& model,
                                   const Option& option,
                                   const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    file << std::fixed << std::setprecision(6);
    
    // En-tête
    file << "Spot,Price,Delta,Gamma,Theta,Rho,Vega\n";
    
    // Points de données
    double K = option.getStrikePrice();
    double spotMin = K * 0.5;
    double spotMax = K * 1.5;
    int points = 100;
    double dS = (spotMax - spotMin) / points;
    
    for (double s = spotMin; s <= spotMax; s += dS) {
        Option tmpOption(s, option.getStrikePrice(), 
                        option.getRiskFreeRate(), option.getVolatility(),
                        option.getTimeToMaturity(), option.getType());
        
        file << s << ","
             << model.calculatePrice(tmpOption) << ","
             << GreeksCalculator::delta(model, tmpOption) << ","
             << GreeksCalculator::gamma(model, tmpOption) << ","
             << GreeksCalculator::theta(model, tmpOption) << ","
             << GreeksCalculator::rho(model, tmpOption) << ","
             << GreeksCalculator::vega(model, tmpOption) << "\n";
    }
    file.close();
}

// Export for AmericanOptionPricer
void OptionDataExporter::exportToCSV(const AmericanOptionPricer& pricer,
                                   const Option& option,
                                   const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    file << std::fixed << std::setprecision(6);
    
    // En-tête
    file << "Spot,Price,Delta,Gamma,Theta,Rho,Vega\n";
    
    // Points de données
    double K = option.getStrikePrice();
    double spotMin = K * 0.5;
    double spotMax = K * 1.5;
    int points = 100;
    double dS = (spotMax - spotMin) / points;
    
    for (double s = spotMin; s <= spotMax; s += dS) {
        Option tmpOption(s, option.getStrikePrice(), 
                        option.getRiskFreeRate(), option.getVolatility(),
                        option.getTimeToMaturity(), option.getType());
        
        file << s << ","
             << pricer.calculatePrice(tmpOption) << ","
             << GreeksCalculator::delta(pricer, tmpOption) << ","
             << GreeksCalculator::gamma(pricer, tmpOption) << ","
             << GreeksCalculator::theta(pricer, tmpOption) << ","
             << GreeksCalculator::rho(pricer, tmpOption) << ","
             << GreeksCalculator::vega(pricer, tmpOption) << "\n";
    }
    file.close();
}

// Export for BlackScholesModel
void OptionDataExporter::exportToCSV(const BlackScholesModel& model,
                                   const Option& option,
                                   const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    file << std::fixed << std::setprecision(6);
    
    // En-tête
    file << "Spot,Price,Delta,Gamma,Theta,Rho,Vega\n";
    
    // Points de données
    double K = option.getStrikePrice();
    double spotMin = K * 0.5;
    double spotMax = K * 1.5;
    int points = 100;
    double dS = (spotMax - spotMin) / points;
    
    for (double s = spotMin; s <= spotMax; s += dS) {
        Option tmpOption(s, option.getStrikePrice(), 
                        option.getRiskFreeRate(), option.getVolatility(),
                        option.getTimeToMaturity(), option.getType());
        
        file << s << ","
             << model.calculatePrice(tmpOption) << ","
             << GreeksCalculator::delta(model, tmpOption) << ","
             << GreeksCalculator::gamma(model, tmpOption) << ","
             << GreeksCalculator::theta(model, tmpOption) << ","
             << GreeksCalculator::rho(model, tmpOption) << ","
             << GreeksCalculator::vega(model, tmpOption) << "\n";
    }
    file.close();
}


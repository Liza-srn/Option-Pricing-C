#ifndef OPTION_DATA_EXPORTER_HPP
#define OPTION_DATA_EXPORTER_HPP
#include "models/FiniteDifferenceModel.hpp"
#include "models/BlackScholesModel.hpp"  // Assurez-vous d'inclure ce fichier d'en-tête
#include "domain/Option.hpp"
#include "util/GreeksCalculator.hpp"
#include <string>
#include <fstream>
class OptionDataExporter {
public:
    // Exporte les données de l'option vers un fichier CSV
    static void exportToCSV(const FiniteDifferenceModel& model, 
                          const Option& option,
                          const std::string& filename);
    static void exportToCSV(const AmericanOptionPricer& pricer, 
                          const Option& option,
                          const std::string& filename);
    // Ajoutez cette déclaration
    static void exportToCSV(const BlackScholesModel& model,
                          const Option& option,
                          const std::string& filename);
    // Exports benchmark data with Black-Scholes
    static void exportBenchmarkToCSV(const Option& option,
                                   const std::string& filename);
    // Génère le script VBA pour créer les graphiques
    static void writeVBAScript(const std::string& filename);
private:
    // Helper method for creating chart scripts
    static void writePriceChartScript(std::ofstream& file);
    static void writeDeltaChartScript(std::ofstream& file);
};
#endif // OPTION_DATA_EXPORTER_HPP
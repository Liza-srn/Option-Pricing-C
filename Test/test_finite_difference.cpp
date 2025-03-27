// Au lieu de CATCH_CONFIG_MAIN, utilisez :
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

int main(int argc, char* argv[]) {
    return Catch::Session().run(argc, argv);
}

#include "../include/models/FiniteDifferenceModel.hpp"
#include "../include/domain/Option.hpp"

TEST_CASE("Test du constructeur FiniteDifferenceModel", "[FiniteDifferenceModel]") {
    REQUIRE_NOTHROW(FiniteDifferenceModel(50, 100)); // Construction valide
    REQUIRE_THROWS_AS(FiniteDifferenceModel(-10, 100), std::invalid_argument); // Mauvaise valeur
    REQUIRE_THROWS_AS(FiniteDifferenceModel(50, -20), std::invalid_argument);
}

TEST_CASE("Calcul du prix avec Crank-Nicolson", "[FiniteDifferenceModel]") {
    FiniteDifferenceModel model(50, 100);
    Option callOption(100.0, 100.0, 0.05, 0.2, 1.0, "call");
    Option putOption(100.0, 100.0, 0.05, 0.2, 1.0, "put");
    
    double callPrice = model.calculatePrice(callOption);
    double putPrice = model.calculatePrice(putOption);
    
    REQUIRE(callPrice > 0.0);
    REQUIRE(putPrice > 0.0);
    REQUIRE(callPrice != putPrice);
}
